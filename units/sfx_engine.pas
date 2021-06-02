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
	SONGData:byteArray;		// table for SONG data
	SFXModMode:byteArray;	// indicates the type of modulation used in the SFX.
	SFXPtr:wordArray;			// heap pointers to SFX definitions
	TABPtr:wordArray;			// heap pointera to TAB definitions
	dataAddr:word;				// base address to heap pointers

	song_lpb:byte absolute $f0;

	channels:array[0..63] of byte absolute SFX_CHANNELS_ADDR;

procedure INIT_SFXEngine(_dataAddr,_SFXModModes,_SFXList,_TABList,_SONGData:word);
procedure SetNoteTable(_note_val:word);
procedure SFX_Start();
procedure SFX_ChannelOff(channel:byte);
procedure SFX_Off();
procedure SFX_Note(channel,note,SFXId:byte);
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
	__cOfs:byte;

procedure INIT_SFXEngine;
begin
	AUDCTL:=%00000000;
	SKCTL:=%00; SKCTL:=%11;

	dataAddr:=_dataAddr;
	SFXModMode:=pointer(_SFXModModes);
	SFXPtr:=pointer(_SFXList);
	TABPtr:=pointer(_TABList);
	SONGData:=pointer(_SONGData);

	__cOfs:=0;
	repeat
		channels[__cOfs+0]:=$ff;	// SFX address lo
		channels[__cOfs+1]:=$ff;	// SFX address hi
		channels[__cOfs+2]:=$ff;	// SFX offset
		channels[__cOfs+3]:=$00;	// SFX Note
		channels[__cOfs+4]:=$00;	// SFX frequency
		channels[__cOfs+5]:=$00;	// SFX modulation Mode

{$IFDEF SFX_previewChannels}
		channels[__cOfs+6]:=$00;	// SFX modulation Value
		channels[__cOfs+7]:=$00;	// SFX distortion & volume
{$ENDIF}
		__cOfs:=__cOfs+$10;
	until __cOfs=$40;
end;

procedure SetNoteTable;
begin
	note_val:=pointer(_note_val);
end;

procedure SFX_tick(); Assembler; Interrupt;
asm
sfx_engine_start

	icl 'sfx-engine/sfx_engine.asm'

 .print "SFX-ENGINE SIZE: ", *-sfx_engine_start

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
	__cOfs:=channel*$10;
// {$IFDEF SFX_previewChannels}	// its comment, becouse I dont know, why it won't WORK!
// The MP compiler does not generate code from this block, despite the label declaration.
	channels[__cOfs+7]:=$00; // SFX distortion @ volume
// {$ENDIF}
	channels[__cOfs+2]:=$ff;	// SFX offset
	__cOfs:=1+channel*2;
	AUDIO[__cOfs]:=0;
end;

procedure SFX_Off;
begin
	for __chn:=0 to 3 do SFX_ChannelOff(__chn);
end;

procedure SFX_Note;
var
	SFXAddr:word;

begin
	SFXAddr:=SFXPtr[SFXId]+dataAddr;
	SFXAddr:=SFXAddr+14; // skip first 14 bytes - length of SFX name

	__cOfs:=channel*$10;

	channels[__cOfs+0]:=lo(SFXAddr);			// SFX address lo
	channels[__cOfs+1]:=hi(SFXAddr);			// SFX address hi
	channels[__cOfs+2]:=$00;					// SFX offset
	channels[__cOfs+3]:=note;					// SFX Note
	channels[__cOfs+4]:=note_val[note];		// SFX frequency
	channels[__cOfs+5]:=SFXModMode[SFXId];	// SFX modulation Mode
end;

procedure SFX_End;
begin
	SFX_Off();
	NMIEN:=$00;
	SetIntVec(iVBL, oldVBL);
	NMIEN:=$40;
end;

end.
