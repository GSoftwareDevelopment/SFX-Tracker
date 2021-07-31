unit SFX_API;

interface
type
   byteArray=array[0..0] of byte;
   wordArray=array[0..0] of word;

const
{$i sfx_engine.conf.inc} // import SFX-Engine configuration

// SFX-Engine Constants

{$i sfx_engine/sfx_engine_const.inc}

var
   SONGData:byteArray absolute SONG_ADDR;              // table for SONG data
   SFXModMode:byteArray absolute SFX_MODE_SET_ADDR;    // indicates the type of modulation used in the SFX.
   SFXNoteSetOfs:byteArray absolute SFX_NOTE_SET_ADDR; //
   SFXPtr:wordArray absolute SFX_TABLE_ADDR;           // heap pointers to SFX definitions
   TABPtr:wordArray absolute TAB_TABLE_ADDR;           // heap pointera to TAB definitions

   SONG_Tempo:byte absolute SFX_REGISTERS+$00;
   SONG_Tick:byte absolute SFX_REGISTERS+$01;
   SONG_Ofs:byte absolute SFX_REGISTERS+$02;
   SONG_RepCount:byte absolute SFX_REGISTERS+$03;

   channels:array[0..0] of byte absolute SFX_CHANNELS_ADDR;

procedure INIT_SFXEngine(); Assembler;
procedure SFX_Start();
procedure SFX_ChannelOff(channel:byte); Assembler;
procedure SFX_Off(); Assembler;
procedure SFX_Note(channel,note,SFXId:byte); Assembler;
procedure SFX_Freq(channel,freq,SFXId:byte); Assembler;
procedure SFX_PlayTAB(channel,TABId:byte); Assembler;
procedure SFX_PlaySONG(startPos:byte); Assembler;
procedure SFX_End();

implementation
var
   NMIEN:byte absolute $D40E;
   oldVBL:pointer;

procedure INIT_SFXEngine; Assembler;
asm
sfx_engine_start

 .print "SFX-ENGINE START: ", *

   icl 'sfx_engine/sfx_engine.asm'

 .print "SFX-ENGINE SIZE: ", *-sfx_engine_start
end;

procedure SFX_tick(); Assembler; Interrupt;
asm
xitvbl      = $e462
sysvbv      = $e45c
wsync			= $d40a
portb       = $d301

         phr

.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
			lda portb
			pha
			lda #$FE
			sta portb
.endif
.endif

			lda #$06
			sta wsync
			sta $D01a

         jsr INIT_SFXEngine.SFX_MAIN_TICK

			lda #$00
			sta $D01a

.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
			pla
			sta portb
.endif
.endif

         plr
         rti
         jmp xitvbl
end;

procedure SFX_Start;
begin
   INIT_SFXEngine();
   NMIEN:=%00000000;
   GetIntVec(iDLI, oldVBL);
   SetIntVec(iDLI, @SFX_tick);
   NMIEN:=%11000000;
end;

procedure SFX_ChannelOff; Assembler;
asm
	asl @
	asl @
	asl @
	asl @
   tax
   clc
   jsr INIT_SFXEngine.SFX_OFF_CHANNEL
end;

procedure SFX_Off; Assembler;
asm
   jsr INIT_SFXEngine.SFX_OFF_ALL
end;

procedure SFX_Note; Assembler;
asm
   lda channel
   asl @
   asl @
   asl @
   asl @
   tax
   ldy SFXId
   lda note
   clc         ; accu has note index
   jsr INIT_SFXEngine.SFX_PLAY_NOTE
end;

procedure SFX_Freq; Assembler;
asm
   lda channel
   asl @
   asl @
   asl @
   asl @
   tax
   ldy SFXId
   lda freq
   sec         ; accu has frequency value
   jsr INIT_SFXEngine.SFX_PLAY_NOTE
end;

procedure SFX_PlayTab; Assembler;
asm
   lda #$FF
   sta SONG_Tick

   lda channel
   asl @
   asl @
   asl @
   asl @
   tax
   lda TABId

   jsr INIT_SFXEngine.SFX_PLAY_TAB

   lda #$00
   sta SONG_Tick
end;

procedure SFX_PlaySONG; Assembler;
asm
   tay
   jsr INIT_SFXEngine.SFX_PLAY_SONG
end;

procedure SFX_End;
begin
   SFX_Off();
   if oldVBL<>nil then
   begin
      NMIEN:=%00000000;
      SetIntVec(iDLI, oldVBL);
      NMIEN:=%01000000;
      oldVBL:=nil;
   end;
end;

end.
