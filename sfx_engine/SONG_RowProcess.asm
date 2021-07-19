         ldx #$00                            ; set loop counter; step by $10
                                             ; Must be counting, since the data is sequentially
                                             ; arranged for tracks 1 through 4

set_SONG_track
; in A register is SONG track entry
         cmp #$40                            ; check, if track entry is TAB Id#
         bpl check_track_off                 ; if no, check other options

set_TABId_in_Track
; TAB index is in the A registry
         sty _regTemp                        ; store current SONG offset in temporary register

         jsr SFX_setTABinChannel

         ldy _regTemp                        ; restore SONG offset

         jmp process_next_track

; other options in track entry

check_track_off
         cmp #trkOff                         ; check TRACK_OFF option
         beq SONGFn_TrackOff                 ;

; play current TAB again

         lda SFX_CHANNELS_ADDR+_tabOfs,x
         cmp #TAB_OFF
         beq dont_reset_TAB
         lda #$00                            ; reset TAB offset and repeat counter
         sta SFX_CHANNELS_ADDR+_tabOfs,x
         sta SFX_CHANNELS_ADDR+_tabRep,x

dont_reset_TAB
         jmp process_next_track

;TRACK OFF - turns off current track playback
SONGFn_TrackOff
         lda #$FF
         sta SFX_CHANNELS_ADDR+_tabOfs,x
         sta SFX_CHANNELS_ADDR+_chnOfs,x

         lda #$00
         sta chnCtrl
.ifdef MAIN.@DEFINES.SFX_previewChannels
         sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

         jsr turn_off_Audio_channel

process_next_track

         iny                                 ; increase SONG offset to next track

         txa                                 ; change current channel
         clc
         adc #$10
         tax

         cmp #$40
         beq SONG_EndRowProcess

         lda SONG_ADDR,y                     ; get track data
         jmp set_SONG_track

SONG_EndRowProcess
         rts
