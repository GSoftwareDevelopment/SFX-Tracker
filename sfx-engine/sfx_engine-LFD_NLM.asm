;
; Low Frequency Divider Modulator/Note Value Modulator
LFD_NLM_mode            ; code for LFD_NLM
         lda (sfxPtr),y ; get modulate value
         sta chnModVal  ; store in loop register
         bne decode_LFD_NLM   ; check modulation value
         jmp setPokey   ; if =0, means no modulation
decode_LFD_NLM
         bmi LFD_NLM_JumpTO   ; jump to position in SFX definition, if 7th bit is set
			cmp #64
         bpl LFD_NLM_note_mod

; frequency modulation
         cmp #32        ; VAL<32 means positive value, otherwise negative
         bmi LFD_NLM_inc_freq

         ora #%11100000 ; set 7th-5th bit to get oposite value
LFD_NLM_inc_freq
         jmp change_freq   ; return frequency in register A

; note modulation
LFD_NLM_note_mod
         and #%00111111
         cmp #32        ; VAL<32 means positive value, otherwise negative
         bmi LFD_NLM_inc_note

         ora #%11100000 ; set 7th-5th bit to get oposite value
LFD_NLM_inc_note
         clc
         adc chnNote
         sta chnNote

; get frequency representation of note
         sty _regY
         tay
         lda NOTE_TABLE_ADDR,y
         ldy _regY
         jmp setChannelFreq

; Jump to
LFD_NLM_JumpTo
         and #%01111111 ; clear 7th bit
         bne LFD_NLM_setSFXofs
         ldy #$ff ; end of SFX definition
         jmp next_SFX_Set
LFD_NLM_setSFXofs
			asl @
         tay ; set value to SFX offset register
         jmp LFD_NLM_mode  ; one more iteration
