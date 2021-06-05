TAB_Function
; in A register is current order
;$2a05			brk
			cmp #$FF
			bne TAB_not_end
TAB_FN_TABEnd
			ldy #00
			jmp fetch_TAB_row

TAB_not_end
			cmp #$C0											;
			beq TAB_FN_Blank_NoteOff

TAB_FN_Loop
			and #%00111111									; extract value from order
			beq TAB_FN_JumpTo								; =0 its JUMP TO

TAB_FN_Repeat
			sta _regTemp									; temporary store repeat value

; check current loop
			lda SFX_CHANNELS_ADDR+_tabRep,x			; get current repeat value
			beq TAB_FN_RepeatSet							; =0 set loop
TAB_FN_ContinueLoop
			dec SFX_CHANNELS_ADDR+_tabRep,x			; decrase current repeat value
			bne TAB_FN_JumpTo								; if current repeat value <>0 jump to position...
			iny												; increment current TAB offset to next row
			jmp fetch_TAB_row								; ...otherwise, fetch next TAB row - end of loop
TAB_FN_RepeatSet
			lda _regTemp
			sta SFX_CHANNELS_ADDR+_tabRep,x

TAB_FN_JumpTo
			lda TABNote										; set jump position
			asl @												; multiply by 2 to get TAB offest
			tay												; store in TAB offset register
			jmp fetch_TAB_row								; get new TAB line

TAB_FN_Blank_NoteOff
			lda TABNote										; get row value
			bpl TAB_FN_Blank								; check type

TAB_FN_NoteOff
			lda #SFX_OFF									; turn off current SFX
			sta SFX_CHANNELS_ADDR+_chnOfs,x

			lda #$00
			sta chnCtrl
.ifdef MAIN.@DEFINES.SFX_previewChannels
			sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

turn_off_Audio_channel
			stx _regTemp
         txa                                 ; transfer channel offset (X reg) to A reg
         lsr @                               ; divide channel offset by 8
         lsr @                               ; to calculate AUDIO offset
         lsr @
         tax                                 ; set AUDIO offset in X register

         lda #$00			                     ; silent Audio channel
         sta audc,x                          ; store direct to POKEY register
         ldx _regTemp								; restore current channel offset

			jmp next_player_tick

TAB_FN_Blank
; This function does nothing with registers
