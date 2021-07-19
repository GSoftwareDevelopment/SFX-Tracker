; POKEY Initialize
.ifndef MAIN.@DEFINES.SFX_STEREOOUT
	lda #$00
	sta AUDCTL
	sta SKCTL
	lda #%11
	sta SKCTL
.else
	lda #$00
	sta AUDCTL
	sta AUDCTL2
	sta SKCTL
	sta SKCTL2
	lda #%11
	sta SKCTL
	sta SKCTL2
.endif

	lda #$80												; prevent SONG process
	sta SONG_TICK_COUNTER

	ldx #0

init_channels
	lda #$FF
	sta SFX_CHANNELS_ADDR+_sfxPtrLo,x
	sta SFX_CHANNELS_ADDR+_sfxPtrHi,x
	sta SFX_CHANNELS_ADDR+_chnOfs,x				; prevent SFX process

	sta SFX_CHANNELS_ADDR+_trackOfs,x			; prevent SONG track process
	sta SFX_CHANNELS_ADDR+_tabPtrLo,x
	sta SFX_CHANNELS_ADDR+_tabPtrHi,x
	sta SFX_CHANNELS_ADDR+_tabOfs,x				; prevent TAB process
	sta SFX_CHANNELS_ADDR+_tabRep,x

	lda #$00
	sta SFX_CHANNELS_ADDR+_sfxNoteTabOfs,x
	sta SFX_CHANNELS_ADDR+_chnNote,x
	sta SFX_CHANNELS_ADDR+_chnFreq,x
	sta SFX_CHANNELS_ADDR+_chnMode,x

.ifdef MAIN.@DEFINES.SFX_PLAYBACK
	sta SFX_CHANNELS_ADDR+_chnModVal,x
	sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

	txa
	clc
	adc #$10
	tax
	cmp #$40
	bne init_channels

	rts
