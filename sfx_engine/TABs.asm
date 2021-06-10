
			lda SONG_TICK
			beq TAB_set
			jmp end_player_tick

;
; TAB PLAY ROUTINE
;
TAB_set
			ldy SFX_CHANNELS_ADDR+_tabOfs,x			; get TAB offset

check_TAB_offset
			cpy #TAB_OFF									; check TAB offset
			bne fetch_TAB_pointer
			jmp end_player_tick							; $ff = no Tab; end Play routine for this channel

fetch_TAB_pointer
; $29bc			brk

         lda SFX_CHANNELS_ADDR+_tabPtrLo,x		; get TAB pointer
         sta TABPtr
         lda SFX_CHANNELS_ADDR+_tabPtrHi,x
         sta TABPtr+1

fetch_TAB_row
			lda (TABPtr),y									; get current TAB Note
			sta TABNote
			iny												; shift TAB Offset to order value
			lda (TABPtr),y									; get current TAB Order
			sta TABOrder

; Current TAB Order check
			bmi TAB_Function								; check for function (7th bit indicates the function)

			icl 'TABs-Play.asm'
			icl 'TABs-Func.asm'

next_player_tick
			iny
			tya
			sta SFX_CHANNELS_ADDR+_tabOfs,x			; store current TAB offset in Channels register
end_player_tick
