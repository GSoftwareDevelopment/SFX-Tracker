unit SFX_Engine;

interface
type
	byteArray=array[0..0] of byte;
	wordArray=array[0..0] of word;

{$i sfx_engine.conf.inc}

const
	SFX_NameLength		= 14;

var
	SONGData:byteArray;		// table for SONG data
	SFXModMode:byteArray;	// indicates the type of modulation used in the SFX.
	SFXPtr:wordArray;			// heap pointers to SFX definitions
	TABPtr:wordArray;			// heap pointera to TAB definitions
	dataAddr:word;				// base address to heap pointers
	note_val:array[0..0] of byte;

	song_lpb:byte absolute $f0;
	SONG_Tick:byte absolute $f1;

	channels:array[0..63] of byte absolute SFX_CHANNELS_ADDR;

procedure INIT_SFXEngine(_dataAddr,_SFXModModes,_SFXList,_TABList,_SONGData:word);
procedure SetNoteTable(_note_val:word);
procedure SFX_Start();
procedure SFX_ChannelOff(channel:byte);
procedure SFX_Off();
procedure SFX_Note(channel,note,SFXId:byte);
procedure SFX_PlayTAB(channel,TABId:byte);
procedure SFX_End();

implementation
var
	NMIEN:byte absolute $D40E;
	oldVBL:pointer;
{$IFNDEF SFX_SYNCAUDIOOUT}
	AUDIO:array[0..0] of byte absolute $D200;
{$ELSE}
	AUDIO:array[0..0] of byte absolute $E8;
{$ENDIF}
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
		channels[__cOfs+ 0]:=$ff;	// SFX address lo
		channels[__cOfs+ 1]:=$ff;	// SFX address hi
		channels[__cOfs+ 2]:=$ff;	// SFX offset
		channels[__cOfs+ 3]:=$00;	// SFX Note
		channels[__cOfs+ 4]:=$00;	// SFX frequency
		channels[__cOfs+ 5]:=$00;	// SFX modulation Mode

{$IFDEF SFX_previewChannels}
		channels[__cOfs+ 6]:=$00;	// SFX modulation Value
		channels[__cOfs+ 7]:=$00;	// SFX distortion & volume
{$ENDIF}

		channels[__cOfs+ 8]:=$ff;	// TAB address lo
		channels[__cOfs+ 9]:=$ff;	// TAB address hi
		channels[__cOfs+10]:=$ff;	// TAB offset
		channels[__cOfs+11]:=$ff;	// TAB repeat counter
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
	NMIEN:=%00000000;
	GetIntVec(iVBL, oldVBL);
	SetIntVec(iVBL, @SFX_tick);
	NMIEN:=%01000000;
end;

procedure SFX_ChannelOff;
begin
	__cOfs:=channel*$10;
{$IFDEF SFX_previewChannels}
	channels[__cOfs+7]:=$00; // SFX distortion @ volume
{$ENDIF}
	channels[__cOfs+2]:=$ff;	// SFX offset
	channels[__cOfs+10]:=$ff;	// SFX offset
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
{$ifndef DONT_CALC_ABS_ADDR}
	SFXAddr:=SFXPtr[SFXId]+dataAddr;
{$ifndef DONT_CALC_SFX_NAMES}
	SFXAddr:=SFXAddr+14; // skip first 14 bytes - length of SFX name
{$endif}
{$else}
	SFXAddr:=SFXPtr[SFXId];
{$endif}

	__cOfs:=channel*$10;

	channels[__cOfs+0]:=lo(SFXAddr);			// SFX address lo
	channels[__cOfs+1]:=hi(SFXAddr);			// SFX address hi
	channels[__cOfs+2]:=$00;					// SFX offset
	channels[__cOfs+3]:=note;					// SFX Note
	channels[__cOfs+4]:=note_val[note];		// SFX frequency
	channels[__cOfs+5]:=SFXModMode[SFXId];	// SFX modulation Mode
end;

procedure SFX_PlayTAB(channel,TABId:byte);
var
	TABAddr:word;

begin
	TABAddr:=TABPtr[TABId]+dataAddr;
	TABAddr:=TABAddr+8; // skip first 8 bytes - length of TAB name

	__cOfs:=channel*$10;

	channels[__cOfs+ 8]:=lo(TABAddr);			// TAB address lo
	channels[__cOfs+ 9]:=hi(TABAddr);			// TAB address hi
	channels[__cOfs+10]:=$00;						// TAB offset
	channels[__cOfs+11]:=$00;						// TAB repeat counter
	SONG_Tick:=$00;									// reset SONG tick counter
end;

procedure SFX_End;
begin
	SFX_Off();
	NMIEN:=%00000000;
	SetIntVec(iVBL, oldVBL);
	NMIEN:=%01000000;
end;

end.
