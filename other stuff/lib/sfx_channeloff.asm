; input:
; X - channel
; output:
; Y - audio channel

	lda #$FF										; prevent play
	sta SFX_CHANNELS_ADDR+_chnOfs,x		; SFX
	sta SFX_CHANNELS_ADDR+_tabOfs,x		; TAB

.ifdef MAIN.@DEFINES.SFX_previewChannels
	lda #$00
	sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

	txa
	lsr @
	lsr @
	lsr @
	tay
	lda #$00										; immediatly mute audio channel
	sta AUDIO_BUFFER_ADDR,y
	sta AUDIO_BUFFER_ADDR+1,y

	rts
