
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
			bne fetch_TAB_data
			jmp end_player_tick							; $ff = no Tab; end Play routine for this channel

fetch_TAB_pointer
         lda SFX_CHANNELS_ADDR+_tabPtrLo,x		; get TAB pointer
         sta TABPtr
         lda SFX_CHANNELS_ADDR+_tabPtrHi,x
         sta TABPtr+1

fetch_TAB_line
			lda (TABPtr),y									; get current TAB Note
			sta TABNote
			iny												; shift TAB Offset to order value
			lda (TABPtr),y									; get current TAB Order
			sta TABOrder
			iny

; Current TAB Order check
			bmi TAB_Function								; check for function

; PLAY NOTE

			cmp #FN_NOTE_FREQ								; check for Note or Frequency Divider Set
			bpl TAB_FN_Freq								; <64 its Note Set

; get note freq value
			and %00111111									; extract SFX Id from Order
			sty _regTemp									; store current TAB offset
			asl @												; multiply SFX Id by 2 to get offset in SFXPtr offset table
			tay

; get SFX pointer offset from SFXPtr table and set in current channel
			lda (adr.SFXPtr),y
.ifndef CALC_ABS_ADDR
			clc
			adc adr.dataAddr
.endif
			sta SFX_CHANNELS_ADDR+_SFXPtrLo,x
			iny
			lda (adr.SFXPtr),y
.ifndef CALC_ABS_ADDR
			adc adr.dataAddr+1
.endif
			sta SFX_CHANNELS_ADDR+_SFXPtrHi,x

			lda #$00											; reset current SFX offset to the beginig of definition
			sta SFX_CHANNELS_ADDR+_SFXOfs,x

; get Note frequency divider from NOTE_TABLE
         lda TABNote										; get TAB Note value
         tay
         lda NOTE_TABLE_ADDR,y						; get note frequency value from NOTE_TABLE
         ldy _regTemp									; restore current TAB Offset

TAB_FN_Freq
			sta SFX_CHANNELS_ADDR+_chnFreq,x
			tya

; set freq
			sta SFX_CHANNELS_ADDR+_chnNote,x

			jmp end_player_tick

TAB_Function
; in A register is current order

			and #%01111111									; extract value from order
			beq TAB_FN_JumpTo								; =0 its JUMP TO

			lda SFX_CHANNELS_ADDR+_tabRep,x			; get current repeat value
			bne dont_countdown							; <>0 its alredy repeated
			tay												; decrase current repeat value
			dey

dont_countdown
			sta SFX_CHANNELS_ADDR+_tabRep,x			; store current repeat value

TAB_FN_JumpTo
			ldy TAB_Note									; set jump position
			jmp fetch_TAB_line							; get new TAB line

end_player_tick
