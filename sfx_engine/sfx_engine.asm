; This file is a part of sfx_engine.pas
; will not work on its own unless you adapt it!
;
; Introduction: How to adapt?
;
; Two tables are required to be defined:
; - SFX_CHANNELS_ADDR - is an array of engine registers; 32 bytes required
; - NOTE_VAL - an array of note values (can be the one from RMT :P)
;
; The SFX_CHANNELS_ADDR arrays should be previously filled with the start values ($ff,$ff,$ff,$ff,$ff,$00,$00,$00,$00)
;
; And that's it. Hook it up to the VBLANK interrupt and go :D (I guess :P)
;

         icl 'definitions.asm'

         phr
.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
         dec $D301                           ; turn off ROM
.endif
.endif

tick_start
         ldx #$30                           ; set channel offset to last channel

channel_set

.ifdef MAIN.@DEFINES.TAB_PLAYBACK
 .print "TAB PLAYBACK START: ", *

         icl 'TABs.asm'
.endif
 .print "SFX PLAYBACK START: ", *

         icl 'SFXs.asm'

next_channel
         txa                                 ; shift offset to next channel
         sec
         sbc #$10
         bmi end_tick
         tax                                 ; no, go fetching next channel data
         jmp channel_set
end_tick

.ifdef MAIN.@DEFINES.SFX_SYNCAUDIOOUT
         ldx #7
audio_loop
         lda AUDIOBUF,x
         sta audf,x
         dex
         bpl audio_loop
.endif

; SONG CLOCK CONTROL

			ldy SONG_TICK
			bmi dont_reset_tick_counter
         dey
         sty SONG_TICK
         bpl dont_reset_tick_counter
; reset tick counter
         lda SONG_LPB                           ; otherwise, set SONG_TICK as SONG_LPB
         sta SONG_TICK
dont_reset_tick_counter

.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
         inc $D301                           ; turn on ROM
.endif
.endif
         plr
         jmp xitvbl
         rts
