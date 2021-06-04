TAB_Function
; in A register is current order

			cmp #$C0											;
			beq TAB_FN_Blank_NoteOff

TAB_FN_Loop
			and #%00111111									; extract value from order
			beq TAB_FN_JumpTo								; =0 its JUMP TO

TAB_FN_Repeat
			lda SFX_CHANNELS_ADDR+_tabRep,x			; get current repeat value
			bne TAB_FN_JumpTo								; >0 its alredy in loop
			dec SFX_CHANNELS_ADDR+_tabRep,x			; decrase current repeat value
			bne TAB_FN_JumpTo								; if current repeat value =0...
;			iny												; increment current TAB offset
;			iny												; to next row
			jmp fetch_TAB_row								; ...fetch next TAB row - end of loop

TAB_FN_JumpTo
			ldy TABNote										; set jump position
			jmp fetch_TAB_row								; get new TAB line

TAB_FN_Blank_NoteOff
			lda TABNote										; get row value
			bpl TAB_FN_NoteOff							; check type
TAB_FN_NoteOff
			lda #SFX_OFF									; turn off current SFX
			sta SFX_CHANNELS_ADDR+_chnOfs,x

			lda #$00
			sta chnCtrl
.ifdef MAIN.@DEFINES.SFX_previewChannels
			sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif
			jmp setPokey

TAB_FN_Blank
; This function does nothing with registers
