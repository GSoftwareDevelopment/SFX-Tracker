;
; Low Frequency Divider Modulator/Note Value Modulator
LFD_NLM_mode
         lda (sfxPtr),y          ; get modulate value
         sta chnModVal           ; store in loop register
         bne decode_LFD_NLM      ; check modulation value
         jmp modMode_notDefined  ; if =0, means no modulation
decode_LFD_NLM
         bmi LFD_NLM_JumpTO      ; jump to position in SFX definition, if 7th bit is set

         cmp #MODFN_NLM_NOTE
         bpl LFD_NLM_note_mod

; frequency modulation
         cmp #MODFN_NLM_NOTE     ; VAL<32 means positive value, otherwise negative
         bmi LFD_NLM_inc_freq

         ora #%11100000          ; set 7th-5th bit to get oposite value
LFD_NLM_inc_freq
         jmp change_freq         ; return frequency in register A

; note modulation
LFD_NLM_note_mod
         and #%00111111
         cmp #32                 ; VAL<32 means positive value, otherwise negative
         bmi LFD_NLM_inc_note

         ora #%11100000          ; set 7th-5th bit to get oposite value
LFD_NLM_inc_note
         clc
         adc chnNote
         sta chnNote

; get frequency representation of note
         sty _regTemp
         tay

			lda SFX_CHANNELS_ADDR+_sfxNoteTabLo,x	; copy channels set note table pointer
			sta sfxNotePtr									; to current channel note table pointer
			lda SFX_CHANNELS_ADDR+_sfxNoteTabHi,x
			sta sfxNotePtr+1

//         lda NOTE_TABLE_ADDR,y
			lda (sfxNotePtr),y
         ldy _regTemp
         jmp setChannelFreq

; Jump to
LFD_NLM_JumpTo
         and #%01111111          ; clear 7th bit
         bne LFD_NLM_setSFXofs
         ldy #SFX_OFF            ; end of SFX definition
         jmp SFX_Set_Offset
LFD_NLM_setSFXofs
         asl @
         tay                     ; set value to SFX offset register
         jmp LFD_NLM_mode        ; one more iteration
