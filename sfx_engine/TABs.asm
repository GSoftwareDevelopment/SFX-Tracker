; TAB PLAY ROUTINE
;
TAB_set
			lda SFX_CHANNELS_ADDR+_tabOfs,x         ; get TAB row

check_TAB_offset
         cmp #TAB_OFF                           ; check TAB offset
         bne check_end_of_TAB
         jmp end_player_tick                    ; $ff = no Tab; end Play routine for this channel

check_end_of_TAB
			cmp #$80
			bne fetch_TAB_pointer
			jmp TABFn_TAB_END

fetch_TAB_pointer
			asl @                                   ; multiply by 2 to get offset
			tay                                     ; store in Y register

         lda SFX_CHANNELS_ADDR+_tabPtrLo,x       ; get TAB pointer
         sta dataPtr
         lda SFX_CHANNELS_ADDR+_tabPtrHi,x
         sta dataPtr+1

fetch_TAB_row
         lda (dataPtr),y                         ; get current TAB Note
         sta TABParam
         iny                                     ; shift TAB Offset to order value
         lda (dataPtr),y                         ; get current TAB Order
         sta TABOrder

; Current TAB Order check
         bmi check_TAB_Function                       ; check for function (7th bit indicates the function)

         icl 'TABs-Play.asm'
         icl 'TABs-Func.asm'

next_player_tick
         iny

store_TAB_offset
         tya
         lsr @                                  ; divide offest by 2 to get TAB row
         sta SFX_CHANNELS_ADDR+_tabOfs,x        ; store current TAB row in Channels register
