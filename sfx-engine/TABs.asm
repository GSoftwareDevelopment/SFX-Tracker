
			dec SONG_TICK
			beq TAB_set										; Play TAB line only if SONG_TICK is zero!

; reset tick counter
			lda SONG_LPB									; otherwise, set SONG_TICK as SONG_LPB
			sta SONG_TICK
			jmp end_player_tick							; and skip Play routine
;
; TAB PLAY ROUTINE
;

TAB_set
			ldy SFX_CHANNELS_ADDR+_tabOfs,x			; get TAB offset

check_TAB_offset
			cpy #TAB_OFF									; check TAB offset
			bne fetch_TAB_row
			jmp end_player_tick							; $ff = no Tab; end Play routine for this channel

fetch_TAB_pointer
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
			iny

; Current TAB Order check
			bmi TAB_Function								; check for function

			icl 'TABs-Play.asm'
			icl 'TABs-Func.asm'

end_player_tick
			tya
			sta SFX_CHANNELS_ADDR+_tabOfs,x			; store current TAB offset in Channels register
