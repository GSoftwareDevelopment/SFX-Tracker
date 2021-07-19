; input:
; x - channel offset ($00, $10, $20, $30)
; y - SFX index
; a - frequency/note_index
; C flag - (1)frequency/(0)note_index

         pha                                 ; temporary put freq/note_index to stack

         lda #SFX_OFF
         sta SFX_CHANNELS_ADDR+_chnOfs,x     ; prevent SFX playback

         lda SFX_MODE_SET_ADDR,y
         sta SFX_CHANNELS_ADDR+_chnMode,x    ; set SFX modulator mode

         bcc PlayNote_getNoteTableOfs
         lda #$FF                            ; set non-note table
         bcs PlayNote_setNoteTableOfs
PlayNote_getNoteTableOfs
         lda SFX_NOTE_SET_ADDR,y             ; set note table
PlayNote_setNoteTableOfs
         sta SFX_CHANNELS_ADDR+_sfxNoteTabOfs,x
         sta self_getNoteFromNoteTable+1

         tya
         asl @
         tay

         lda SFX_TABLE_ADDR,y                ; set SFX data pointer
         sta SFX_CHANNELS_ADDR+_sfxPtrLo,x
         lda SFX_TABLE_ADDR+1,Y
         sta SFX_CHANNELS_ADDR+_sfxPtrHi,x

         lda SFX_CHANNELS_ADDR+_sfxNoteTabOfs,x
         cmp #$FF
         bne PlayNote_setNote

PlayNote_setFreq
         pla
         sta SFX_CHANNELS_ADDR+_chnFreq,x

         lda #$00
         sta SFX_CHANNELS_ADDR+_chnOfs,x

         rts

PlayNote_setNote
         pla                                 ; get freq/note_index from stack
         sta SFX_CHANNELS_ADDR+_chnNote,x    ; set in channels register
         tay
self_getNoteFromNoteTable
         lda NOTE_TABLE_ADDR,y               ; get note frequency value
         sta SFX_CHANNELS_ADDR+_chnFreq,x    ; set in channels register

         lda #$00
         sta SFX_CHANNELS_ADDR+_chnOfs,x     ; play SFX

         rts
