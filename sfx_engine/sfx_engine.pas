unit SFX_Engine;

interface
type
   byteArray=array[0..0] of byte;
   wordArray=array[0..0] of word;

const
   SFX_NameLength    = 14;
   TAB_NameLength		= 8;

{$i sfx_engine.conf.inc} // import SFX-Engine configuration
{$r sfx_engine.rc}

// SFX-Engine Constants

{$i sfx_engine/sfx_engine_const.inc}

var
   SONGData:byteArray absolute SONG_ADDR;              // table for SONG data
   SFXModMode:byteArray absolute SFX_MODE_SET_ADDR;    // indicates the type of modulation used in the SFX.
   SFXNoteSetOfs:byteArray absolute SFX_NOTE_SET_ADDR; //
   SFXPtr:wordArray absolute SFX_TABLE_ADDR;           // heap pointers to SFX definitions
   TABPtr:wordArray absolute TAB_TABLE_ADDR;           // heap pointera to TAB definitions
// note_val:array[0..0] of byte;

   song_lpb:byte absolute SFX_REGISTERS+$00;
   SONG_Tick:byte absolute SFX_REGISTERS+$01;
	SONG_TrackPos:array[0..0] of byte absolute SFX_REGISTERS+$02;

   channels:array[0..63] of byte absolute SFX_CHANNELS_ADDR;
	currentNoteTableOfs:byte;

procedure INIT_SFXEngine();
procedure SFX_Start();
procedure SFX_ChannelOff(channel:byte);
procedure SFX_Off();
procedure SFX_Note(channel,note,SFXId:byte);
procedure SFX_Freq(channel,freq,SFXId:byte);
procedure SFX_SetTAB(channel,TABId:byte);
procedure SFX_PlayTAB(channel,TABId:byte);
procedure SFX_PlaySONG(startPos:byte);
procedure SFX_End();

implementation
var
   NMIEN:byte absolute $D40E;
   oldVBL:pointer;
{$IFNDEF SFX_SYNCAUDIOOUT}
   AUDIO:array[0..0] of byte absolute $D200;
{$ELSE}
   AUDIO:array[0..0] of byte absolute AUDIO_BUFFER_ADDR;
{$ENDIF}
   AUDCTL:byte absolute $D208;
   SKCTL:byte absolute $D20F;

   __chn:byte;
   __cOfs:byte;

procedure INIT_SFXEngine;
begin
	SONG_Tick:=$80;
   AUDCTL:=%00000000;
   SKCTL:=%00; SKCTL:=%11;
	currentNoteTableOfs:=$FF;

   __cOfs:=0;
   repeat
      channels[__cOfs+ _sfxPtrLo]:=$ff;   // SFX address lo
      channels[__cOfs+ _sfxPtrHi]:=$ff;   // SFX address hi
      channels[__cOfs+ _sfxNoteTabOfs]:=$00;  // SFX address lo
      channels[__cOfs+ _chnOfs]:=$ff;  // SFX offset
      channels[__cOfs+ _chnNote]:=$00; // SFX Note
      channels[__cOfs+ _chnFreq]:=$00; // SFX frequency
      channels[__cOfs+ _chnMode]:=$00; // SFX modulation Mode

{$IFDEF SFX_previewChannels}
      channels[__cOfs+ _chnModVal]:=$00;  // SFX modulation Value
      channels[__cOfs+ _chnCtrl]:=$00; // SFX distortion & volume
{$ENDIF}

{$IFDEF SFX_PLAYBACK}
		channels[__cOfs+ _trackOfs]:=$ff;	// SONG Track offset

      channels[__cOfs+ _tabPtrLo]:=$ff;   // TAB address lo
      channels[__cOfs+ _tabPtrHi]:=$ff;   // TAB address hi
      channels[__cOfs+ _tabOfs]:=$ff;  // TAB offset
      channels[__cOfs+ _tabRep]:=$ff;  // TAB repeat counter
{$ENDIF}
      __cOfs:=__cOfs+$10;
   until __cOfs=$40;
end;

procedure SFX_tick(); Assembler; Interrupt;
asm
sfx_engine_start

 .print "SFX-ENGINE START: ", *

   icl 'sfx_engine/sfx_engine.asm'

 .print "SFX-ENGINE SIZE: ", *-sfx_engine_start

end;

procedure SFX_Start;
begin
	INIT_SFXEngine();
	NMIEN:=%00000000;
	GetIntVec(iVBL, oldVBL);
	SetIntVec(iVBL, @SFX_tick);
	NMIEN:=%01000000;
end;

procedure SFX_ChannelOff;
begin
   __cOfs:=channel*$10;
{$IFDEF SFX_previewChannels}
   channels[__cOfs+ _chnCtrl]:=$00;		// SFX distortion @ volume
{$ENDIF}
   channels[__cOfs+ _chnOfs]:=$ff;		// SFX offset

{$IFDEF SFX_PLAYBACK}
	channels[__cOfs+ _trackOfs]:=$ff;	// SONG Track offset - don't process track information
													//
   channels[__cOfs+ _tabOfs]:=$ff;		// TAB offset
{$ENDIF}

   __cOfs:=1+channel*2;
   AUDIO[__cOfs]:=0;							// mute audio channel
end;

procedure SFX_Off;
begin
	SONG_Tick:=$80;
   for __chn:=0 to 3 do SFX_ChannelOff(__chn);
end;

procedure SFX_Note;
var
   SFXAddr:word;
   NoteTabOfs:byte;
   note_val:array[0..0] of byte;

begin
   if currentNoteTableOfs=$FF then
		NoteTabOfs:=SFXNoteSetOfs[SFXId]
	else
		NoteTabOfs:=currentNoteTableOfs;
	note_val:=pointer(NOTE_TABLE_ADDR+NoteTabOfs);

{$ifndef DONT_CALC_ABS_ADDR}
   SFXAddr:=SFXPtr[SFXId]+DATA_ADDR;
{$ifndef DONT_CALC_SFX_NAMES}
   SFXAddr:=SFXAddr+14; // skip first 14 bytes - length of SFX name
{$endif}
{$else}
   SFXAddr:=SFXPtr[SFXId];
{$endif}

   __cOfs:=channel*$10;

   channels[__cOfs+ _sfxPtrLo]:=lo(SFXAddr);       // SFX address lo
   channels[__cOfs+ _sfxPtrHi]:=hi(SFXAddr);       // SFX address hi
   channels[__cOfs+ _sfxNoteTabOfs]:=NoteTabOfs;   // SFX Note table address lo
   channels[__cOfs+ _chnNote]:=note;               // SFX Note
   channels[__cOfs+ _chnFreq]:=note_val[note];     // SFX frequency
   channels[__cOfs+ _chnMode]:=SFXModMode[SFXId];  // SFX modulation Mode
   channels[__cOfs+ _chnOfs]:=$00;                 // SFX offset
end;

procedure SFX_Freq;
var
   SFXAddr:word;
   note_val:array[0..0] of byte;
   note,i:byte;

begin
{$ifndef DONT_CALC_ABS_ADDR}
   SFXAddr:=SFXPtr[SFXId]+DATA_ADDR;
{$ifndef DONT_CALC_SFX_NAMES}
   SFXAddr:=SFXAddr+14; // skip first 14 bytes - length of SFX name
{$endif}
{$else}
   SFXAddr:=SFXPtr[SFXId];
{$endif}

   __cOfs:=channel*$10;

   channels[__cOfs+ _sfxPtrLo]:=lo(SFXAddr);       // SFX address lo
   channels[__cOfs+ _sfxPtrHi]:=hi(SFXAddr);       // SFX address hi
   channels[__cOfs+ _sfxNoteTabOfs]:=$FF;          // SFX Note table address hi
   channels[__cOfs+ _chnOfs]:=$00;                 // SFX offset
   note_val:=pointer(NOTE_TABLE_ADDR);
   for i:=0 to 63 do
      if freq<=note_val[i] then note:=i;
   channels[__cOfs+ _chnNote]:=note;               // SFX Note
   channels[__cOfs+ _chnFreq]:=freq;               // SFX frequency
   channels[__cOfs+ _chnMode]:=SFXModMode[SFXId];  // SFX modulation Mode
end;

procedure SFX_SetTAB(channel,TABId:byte);
var
   TABAddr:word;

begin
	if TABId<64 then
	begin
{$ifndef DONT_CALC_ABS_ADDR}
   TABAddr:=TABPtr[TABId]+DATA_ADDR;
{$ifndef DONT_CALC_SFX_NAMES}
   TABAddr:=TABAddr+8; // skip first 8 bytes - length of TAB name
{$endif}
{$else}
   TABAddr:=TABPtr[TABId];
{$endif}
	end
	else
	  TABAddr:=$FFFF;

   __cOfs:=channel*$10;

   channels[__cOfs+ _tabPtrLo]:=lo(TABAddr);       // TAB address lo
   channels[__cOfs+ _tabPtrHi]:=hi(TABAddr);       // TAB address hi
   if TABAddr<$FFFF then
   begin
		channels[__cOfs+ _tabOfs]:=$00;              // TAB offset
		channels[__cOfs+ _tabRep]:=$00;              // TAB repeat counter
	end
	else
	begin
		channels[__cOfs+ _tabOfs]:=$FF;              // TAB offset
		channels[__cOfs+ _tabRep]:=$FF;              // TAB repeat counter
	end;
end;

procedure SFX_PlayTAB(channel,TABId:byte);
begin
	SFX_SetTAB(channel,TABID);
   SONG_Tick:=$00;                                 // reset SONG tick counter - Play TAB
end;

procedure SFX_PlaySONG(startPos:byte);
begin
	SFX_Off();
   startPos:=startPos*4;
	for __chn:=0 to 3 do
	begin
		__cOfs:=__chn*$10;
		channels[__cOfs+ _trackOfs]:=startPos;	// set SONG Track offset to startPos
		SFX_SetTAB(__chn,SONGData[startPos]);
		startPos:=startPos+1;
	end;

   SONG_Tick:=$00;                                 // reset SONG tick counter - Play TAB
end;

procedure SFX_End;
begin
	SFX_Off();
	if oldVBL<>nil then
	begin
		NMIEN:=%00000000;
		SetIntVec(iVBL, oldVBL);
		NMIEN:=%01000000;
		oldVBL:=nil;
	end;
end;

end.
