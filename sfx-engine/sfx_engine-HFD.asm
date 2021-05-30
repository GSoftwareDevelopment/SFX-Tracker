;
; HFD - High Frequency Divider Modulator
; only for compatibility with the original SFX engine

HFD_MODE                ; code for HFD
         lda (sfxPtr),y ; get modulate value
         sta chnModVal
         bne decode_HFD ; check modulation
         jmp getChannelFreq   ; if 0, means no modulation
decode_HFD
			cmp #$80
			bne HFD_modulate

HFD_SFXEND
         ldy #$ff 		; end of SFX definition
         jmp next_SFX_Set

HFD_modulate
         clc
         adc SFX_CHANNELS_ADDR+_chnFreq,x
         jmp setChannelFreq
