; input:
; X - channel
; Y - TAB index

	lda #$00
	sta SONG_TICK_COUNTER

	jmp SFX_SetTAB
