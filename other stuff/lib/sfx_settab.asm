; input:
; X - channel
; A - TAB index

	asl @
	tay

SFX_SetTABbyOffset
	lda TAB_TABLE_ADDR,y
	sta SFX_CHANNELS_ADDR+_tabPtrLo,x
	lda TAB_TABLE_ADDR+1,y
	sta SFX_CHANNELS_ADDR+_tabPtrHi,x

	lda #$00
	sta SFX_CHANNELS_ADDR+_tabOfs,x
	sta SFX_CHANNELS_ADDR+_tabRep,x

	rts
