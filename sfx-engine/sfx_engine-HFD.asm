;
; HFD - High Frequency Divider Modulator
; only for compatibility with the original SFX engine

HFD_MODE                ; code for HFD
         lda (sfxPtr),y ; get modulate value
         sta chnModVal
         bne decode_HFD ; check modulation
         jmp setPokey   ; if 0, means no modulation
decode_HFD
			cmp #$80
			beq HFD_SFXEnd

         jmp change_freq

HFD_SFXEnd
         ldy #$ff 		; end of SFX definition
         jmp next_SFX_Set
