;
; HFD - High Frequency Divider Modulator
; only for compatibility with the original SFX engine

HFD_MODE
         lda (sfxPtr),y				; get modulate value
         sta chnModVal
         bne decode_HFD				; check modulation
         jmp modMode_notDefined	; if 0, means no modulation
decode_HFD
			cmp #MODFN_SFX_STOP
			beq HFD_SFXEnd

         jmp change_freq

HFD_SFXEnd
         ldy #SFX_OFF				; end of SFX definition
         jmp next_SFX_Set
