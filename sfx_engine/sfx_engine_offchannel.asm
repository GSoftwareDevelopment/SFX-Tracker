; Turn off playback (SFX and TAB) on the specified channel.
; Note: If a song is playing, the indicated channel will be activated when it is used in the song track!
;
; input:
; x - channel offset ($00, $10, $20, $30)

         lda #$FF                            ; disable processing...
         bcc off_only_sfx
         sta SFX_CHANNELS_ADDR+_tabOfs,x     ; ... of TABs
off_only_sfx
         sta SFX_CHANNELS_ADDR+_chnOfs,x     ; ... and SFXs

.ifdef MAIN.@DEFINES.SFX_previewChannels
         lda #00
         sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

; Mutes only the Audio channel.
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
