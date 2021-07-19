SFX_setTABinChannel
; input:
; x - channel offset ($00, $10, $20, $30)
; a - TAB index

         asl @                               ; multiply TAB index by 2 to get offset in TABPtr table
         tay

; in the X register is an TAB ID offset of the TAB definition array
         lda TAB_TABLE_ADDR,y                ; get TAB definition address from TABPtr table
         sta SFX_CHANNELS_ADDR+_tabPtrLo,x   ; and set it in channels registers
         lda TAB_TABLE_ADDR+1,y
         sta SFX_CHANNELS_ADDR+_tabPtrHi,x

         lda #$00                            ; reset TAB offset and repeat counter
         sta SFX_CHANNELS_ADDR+_tabOfs,x
         sta SFX_CHANNELS_ADDR+_tabRep,x

         rts
