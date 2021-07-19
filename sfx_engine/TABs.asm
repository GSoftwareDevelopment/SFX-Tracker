; TAB PLAY ROUTINE
;
TAB_set
         ldy SFX_CHANNELS_ADDR+_tabOfs,x        ; get TAB offset

check_TAB_offset
         cpy #TAB_OFF                           ; check TAB offset
         bne fetch_TAB_pointer
         jmp end_player_tick                    ; $ff = no Tab; end Play routine for this channel

fetch_TAB_pointer
         lda SFX_CHANNELS_ADDR+_tabPtrLo,x      ; get TAB pointer
         sta dataPtr
         lda SFX_CHANNELS_ADDR+_tabPtrHi,x
         sta dataPtr+1

fetch_TAB_row
         lda (dataPtr),y                         ; get current TAB Note
         sta TABParam
         iny                                    ; shift TAB Offset to order value
         lda (dataPtr),y                         ; get current TAB Order
         sta TABOrder

; Current TAB Order check
         bmi check_TAB_Function                       ; check for function (7th bit indicates the function)

         icl 'TABs-Play.asm'
         icl 'TABs-Func.asm'

next_player_tick
         iny
         bne store_TAB_offset                   ; if TAB offset is wrap?
         jmp TABFn_TAB_END                      ; process TRACK step

store_TAB_offset
         tya
         sta SFX_CHANNELS_ADDR+_tabOfs,x        ; store current TAB offset in Channels register
