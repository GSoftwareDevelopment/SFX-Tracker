         lda #$80
         sta SONG_TICK_COUNTER
         lda #$00
         sta audctl
         sta skctl
         lda #%11
         sta skctl

         lda #$ff
         sta SONG_Ofs

         ldy #$00

SFX_INIT_registers_loop
         lda #$ff
         sta SFX_CHANNELS_ADDR+_sfxPtrLo,y
         sta SFX_CHANNELS_ADDR+_sfxPtrHi,y
         sta SFX_CHANNELS_ADDR+_chnOfs,y
         sta SFX_CHANNELS_ADDR+_tabPtrLo,y
         sta SFX_CHANNELS_ADDR+_tabPtrHi,y
         sta SFX_CHANNELS_ADDR+_tabOfs,y
         sta SFX_CHANNELS_ADDR+_tabRep,y

         lda #$00
         sta SFX_CHANNELS_ADDR+_sfxNoteTabOfs,y
         sta SFX_CHANNELS_ADDR+_chnNote,y
         sta SFX_CHANNELS_ADDR+_chnFreq,y
         sta SFX_CHANNELS_ADDR+_chnMode,y
         sta SFX_CHANNELS_ADDR+_chnModVal,y
         sta SFX_CHANNELS_ADDR+_chnCtrl,y

         tya
         clc
         adc #$10
         tay

         cmp #$40
         bne SFX_INIT_registers_loop

         lda #%00000000
         sta audctl

         rts
