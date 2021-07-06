; This file is a part of sfx_engine.pas
; will not work on its own unless you adapt it!
;
         icl 'definitions.asm'

         phr
.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
         dec $D301                           ; turn off ROM
.endif
.endif

.ifdef MAIN.@DEFINES.SFX_PLAYBACK
; SONG CLOCK CONTROL

			lda SONG_TICK_COUNTER
			bmi dont_reset_tick_counter				; if SONG_TICK>=128 then don't countdown
         dec SONG_TICK_COUNTER
         bpl dont_reset_tick_counter
; reset tick counter
         lda SONG_TEMPO                         ; otherwise, set SONG_TICK as SONG_TEMPO
         sta SONG_TICK_COUNTER
.endif

dont_reset_tick_counter
tick_start
         ldx #$30                           ; set channel offset to last channel

channel_set

.ifdef MAIN.@DEFINES.SFX_PLAYBACK
 .print "PLAYBACK START: ", *
         lda SONG_TICK_COUNTER
         beq TAB_set
         jmp end_player_tick

 .print "TAB PLAYBACK START: ", *
         icl 'TABs.asm'

end_player_tick
.endif
 .print "SFX PLAYBACK START: ", *

         icl 'SFXs.asm'

next_channel
         txa                                 ; shift offset to next channel
         sec
         sbc #$10
         bmi end_tick                        ; is it last channel?
         tax                                 ; no, go fetching next channel data
         jmp channel_set
end_tick

.ifdef MAIN.@DEFINES.SFX_SYNCAUDIOOUT
; move audio buffer data, direct to audio registers
         ldx #7
audio_loop
         lda AUDIOBUF,x
         sta audf,x
         dex
         bpl audio_loop
.endif

.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
         inc $D301                           ; turn on ROM
.endif
.endif
         plr
         jmp xitvbl
         rts

;
; input: X register - channel offset from main loop (means, multiply by 8, means $00, $10, $20, $30)
turn_off_Audio_channel
         stx _regTemp
         txa                                 ; transfer channel offset (X reg) to A reg
         lsr @                               ; divide channel offset by 8
         lsr @                               ; to calculate AUDIO offset
         lsr @
         tax                                 ; set AUDIO offset in X register

         lda #$00                            ; silent Audio channel

.ifdef MAIN.@DEFINES.SFX_SYNCAUDIOOUT
         sta AUDIOBUF+1,x
.else
         sta audc,x                          ; store direct to POKEY register
.endif

         ldx _regTemp                        ; restore current channel offset
         rts

;
; subroutine to process SONG entry

			icl 'SONG.asm'

;
; subroutine for reset all tracks

reset_all_tracks
			ldx #$30

reset_TRACKS
			lda #$FF										; disable the playback...
			sta SFX_CHANNELS_ADDR+_tabOfs,x		; ... of TABs
         sta SFX_CHANNELS_ADDR+_chnOfs,x		; ... and SFXs

.ifdef MAIN.@DEFINES.SFX_previewChannels
			lda #00
         sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

			jsr turn_off_Audio_channel

			txa											; change current channel
			sec
			sbc #$10
			tax
			bpl reset_TRACKS

			rts
