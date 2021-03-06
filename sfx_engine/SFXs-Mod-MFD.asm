;
; MFD - Medium Frequency Divider Modulator

MFD_mode
         lda (dataPtr),y          ; get modulate value
         sta chnModVal
         bne decode_MFD          ; check modulation
         jmp modMode_notDefined  ; if 0, means no modulation

decode_MFD
         bmi MFD_JumpTO          ; jump to position in SFX definition, if 7th bit is set

         cmp #MODFN_MFD_FREQ     ; VAL<64 means positive value, otherwise negative
         bmi MFD_incFreq         ; VAL is positive

         ora #%11000000          ; set 7th & 6th bit; VAL is negative
MFD_incFreq
         jmp change_freq         ; return frequency in register A

; Jump To
MFD_JumpTo
         and #%01111111          ; clear 7th bit
         bne MFD_setSFXofs
         ldy #SFX_OFF            ; end of SFX definition
         jmp SFX_Set_Offset
MFD_setSFXofs
         asl @
         tay                     ; set value to SFX offset register
         jmp modulators          ; one more iteration
