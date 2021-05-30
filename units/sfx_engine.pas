unit SFX_Engine;

interface
type
	byteArray=array[0..0] of byte;
	wordArray=array[0..0] of word;

const
	SFX_CHANNELS_ADDR	= $6E0;

var
(* SFX Mod Modes:
	0 - HFD - High Freq. Div.     - relative modulation of the frequency divider in the range of +/- 127
											- without the possibility of looping the SFX
											- Full backwards compatibility with the original SFX engine
	1 - MFD - Middle Freq. Div.   - relative modulation of the frequency divider in the range of +/- 63
											- SFX looping possible
	2 - LFD/NLM - Low Freq Div.	- note level modulation in relative range of +/- 32 half tones;
											- relative modulation of freq. divider in the range of +/- 32
											- SFX looping possible
	3 - DSD - Direct Set Div.		- direct set of the frequency divider - without looping possible
*)
	SFXModMode:byteArray;	// indicates the type of modulation used in the SFX.
	SFXPtr:wordArray;			// heap pointers to SFX definitions
	TABPtr:wordArray;			// heap pointera to TAB definitions
	SONGData:byteArray;		// table for SONG data

	song_tact,song_beat,song_lpb:byte;

	channels:array[0..31] of byte absolute SFX_CHANNELS_ADDR;

procedure INIT_SFXEngine(_SFXModModes,_SFXList,_TABList,_SONGData:word);
procedure SetNoteTable(_note_val:word);
procedure SFX_Start();
procedure SFX_ChannelOff(channel:byte);
procedure SFX_Off();
procedure SFX_Note(channel,note,modMode:byte; SFXAddr:word);
procedure SFX_End();

implementation
var
	note_val:array[0..0] of byte;
	NMIEN:byte absolute $D40E;
	oldVBL:pointer;
	AUDIO:array[0..0] of byte absolute $D200;
	AUDCTL:byte absolute $D208;
	SKCTL:byte absolute $D20F;

	__chn:byte;
	__chnOfs:byte;

procedure INIT_SFXEngine;
begin
	AUDCTL:=%00000001;
	SKCTL:=%00; SKCTL:=%11;

	SFXModMode:=pointer(_SFXModModes);
	SFXPtr:=pointer(_SFXList);
	TABPtr:=pointer(_TABList);
	SONGData:=pointer(_SONGData);

	__chnOfs:=0;
	repeat
		channels[__chnOfs+0]:=$ff;	// SFX address lo
		channels[__chnOfs+1]:=$ff;	// SFX address hi
		channels[__chnOfs+2]:=$ff;	// SFX offset
		channels[__chnOfs+3]:=$00;	// SFX modulation Mode
		channels[__chnOfs+4]:=$00;	// SFX Note
		channels[__chnOfs+5]:=$00;	// SFX frequency
		channels[__chnOfs+6]:=$00;	// SFX modulation Value
		channels[__chnOfs+7]:=$00;	// SFX distortion & volume
		__chnOfs:=__chnOfs+8;
	until __chnOfs>31;
end;

procedure SetNoteTable;
begin
	note_val:=pointer(_note_val);
end;

procedure SFX_tick(); Assembler; Interrupt;
asm
	icl 'sfx-engine/sfx_engine.asm'
end;

procedure SFX_Start;
begin
	NMIEN:=$00;
	GetIntVec(iVBL, oldVBL);
	SetIntVec(iVBL, @SFX_tick);
	NMIEN:=$40;
end;

procedure SFX_ChannelOff;
begin
	__chnOfs:=channel*8;
	channels[__chnOfs+2]:=$ff;	// SFX offset
	__chnOfs:=1+channel*2;
	AUDIO[__chnOfs]:=0;
end;

procedure SFX_Off;
begin
	for __chn:=0 to 3 do SFX_ChannelOff(__chn);
end;

procedure SFX_Note;
begin
	__chnOfs:=channel*8;
	channels[__chnOfs+0]:=lo(SFXAddr);	// SFX address lo
	channels[__chnOfs+1]:=hi(SFXAddr);	// SFX address hi
	channels[__chnOfs+2]:=$00;	// SFX offset
	channels[__chnOfs+3]:=ModMode;	// SFX modulation Mode
	channels[__chnOfs+4]:=note;	// SFX Note
	channels[__chnOfs+5]:=note_val[note];	// SFX frequency
end;

procedure SFX_End;
begin
	NMIEN:=$00;
	SetIntVec(iVBL, oldVBL);
	NMIEN:=$40;
	SFX_Off();
end;

end.
