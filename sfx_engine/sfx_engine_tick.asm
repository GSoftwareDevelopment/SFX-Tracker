.ifdef MAIN.@DEFINES.SFX_PLAYBACK
; SONG CLOCK CONTROL

         lda SONG_TICK_COUNTER
         bmi dont_reset_tick_counter            ; if SONG_TICK>=128 then don't countdown
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

         rts
