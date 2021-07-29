check_TAB_Function
; current order is in A register
         cmp #$FF                               ; check TABEND function
         bne check_TABFn_REPEAT

TABFn_TAB_END

         lda SONG_Ofs                        ; get current SONG offset
         cmp #$FF                            ; If the offset points to a value other than $FF
         beq play_TAB_again                  ;

         jsr SONG_process                    ; ...it means that SONG is played.
         jmp tick_start

play_TAB_again
         ldy #0                              ; set TAB offset to begin
         jmp fetch_TAB_row

check_TABFn_REPEAT
         cmp #$C0                               ; check REPEAT function
         beq TAB_FN_Blank_NoteOff

TAB_FN_Loop
         and #%00111111                         ; extract value from order (repeat times)
         beq fetch_next_tab_row                 ; =0 REPEAT zero times? :| Can't possible
                                                ; originally, it was a conditional jump `BNE TAB_FN_JumpTo`,
                                                ; which means, the TAB function JUMP TO
TAB_FN_Repeat
         sta _regTemp                           ; temporary store value from order (repeat value)

; check current loop
         lda SFX_CHANNELS_ADDR+_tabRep,x        ; get current repeat value
         beq TAB_FN_RepeatSet                   ; =0 set loop
TAB_FN_ContinueLoop
         dec SFX_CHANNELS_ADDR+_tabRep,x        ; decrase current repeat value
         bne TAB_FN_JumpTo                      ; if current repeat value <>0 jump to position...
         iny                                    ; increment current TAB offset to next row

; end of loop

         bne fetch_next_TAB_row
         jsr SONG_process                       ; if TAB offset is wrap, process TRACK step
         jmp tick_start

fetch_next_TAB_row
         jmp fetch_TAB_row                      ; fetch next TAB row

TAB_FN_RepeatSet
         lda _regTemp
         sta SFX_CHANNELS_ADDR+_tabRep,x

TAB_FN_JumpTo
         lda TABParam                           ; set jump position
         asl @                                  ; multiply by 2 to get TAB offest
         tay                                    ; store in TAB offset register
         jmp fetch_TAB_row                      ; get new TAB line

TAB_FN_Blank_NoteOff
         lda TABParam                            ; get row value
         bpl TAB_FN_Blank                       ; check BLANK (positive value) or NOTE OFF (negative)

TAB_FN_NoteOff
         lda #SFX_OFF                           ; turn off current SFX
         sta SFX_CHANNELS_ADDR+_chnOfs,x

         lda #$00
         sta chnCtrl
.ifdef MAIN.@DEFINES.SFX_previewChannels
         sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

         jsr turn_off_Audio_channel
         jmp next_player_tick

TAB_FN_Blank
; This function does nothing with registers
