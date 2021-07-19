; TAB - PLAY NOTE
         sty _regTemp                           ; store current TAB offset

; get SFX settings
         cmp #FN_NOTE_FREQ                      ; check type of note (Note numer or Frequency Divider Set)
                                                ; Carry flag determine SFX_PLAY_NOTE mode

         and #%00111111                         ; extract SFX Index from Order
         tay                                    ; transfer SFX Index to Y register

         lda TABParam                           ; get note_index

         jsr SFX_PLAY_NOTE

         ldy _regTemp                           ; restore current TAB offset

         jmp next_player_tick
