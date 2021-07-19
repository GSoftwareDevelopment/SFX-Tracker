; input: none
; output: none

	lda #$FF									; stop SONG counter ticking
	sta SONG_TICK_COUNTER

	ldx #$00

off_loop
	jsr SFX_ChannelOff

	txa
	clc
	adc #$10
	tax
	cmp #$40
	beq off_loop

	rts
