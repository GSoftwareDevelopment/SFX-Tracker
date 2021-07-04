; TAB - PLAY NOTE
         sty _regTemp                           ; store current TAB offset

; get SFX settings
         and #%00111111                         ; extract SFX Id from Order
         tay												; transfer SFX Id to Y register
         lda SFX_MODE_SET_ADDR,y                ; get modulator mode for SFX
         sta SFX_CHANNELS_ADDR+_chnMode,x       ; and store it in current channel register

         lda SFX_NOTE_SET_ADDR,y         	      ; get SFX note table preset
         sta SFX_CHANNELS_ADDR+_sfxNoteTabOfs,x ; store in channels registers
			sta self_TABnoteAddr+1						; change the lower byte of the command address

         tya
         asl @                                  ; multiply SFX Id by 2 to get offset in SFXPtr offset table
         tay

; get SFX pointer from SFXPtr table and set in current channel
         lda SFX_TABLE_ADDR,y
         sta SFX_CHANNELS_ADDR+_SFXPtrLo,x
         lda SFX_TABLE_ADDR+1,y
         sta SFX_CHANNELS_ADDR+_SFXPtrHi,x

         lda #$00                               ; reset current SFX offset to the beginig of definition
         sta SFX_CHANNELS_ADDR+_chnOfs,x

; get Note frequency divider from NOTE_TABLE

         lda TABOrder                           ; get TAB Note value
         cmp #FN_NOTE_FREQ                      ; check for Note or Frequency Divider Set
         bpl TAB_FN_Freq                        ; <64 its Note Set

TAB_FN_Note
         ldy TABNote

.ifdef MAIN.@DEFINES.SFX_previewChannels
         tya
         sta SFX_CHANNELS_ADDR+_chnNote,x
.endif

self_TABnoteAddr  lda NOTE_TABLE_ADDR,y         ; get note frequency value from SFX Note Table
         jmp TAB_FN_setFreq

TAB_FN_Freq
         lda TABNote										; get note frequency from TAB row
TAB_FN_setFreq
         sta SFX_CHANNELS_ADDR+_chnFreq,x			; store frequency in current channels register
         ldy _regTemp                           ; restore current TAB Offset

         jmp next_player_tick
