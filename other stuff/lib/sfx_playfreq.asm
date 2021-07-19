;input:
; X - channel
; Y - SFX index
; A - frequency

; set frequency in current channel
	sta SFX_CHANNELS_ADDR+_chnFreq,x

; set note table
	lda SFX_NOTE_SET_ADDR,y
	sta SFX_CHANNELS_ADDR+_sfxNoteTabOfs,x

; set SFX modulator mode
	lda SFX_MODE_SET_ADDR,y
	sta SFX_CHANNELS_ADDR+_chnMode,x

	jmp set_sfx_pointer_and_play		; 'sfx_playnote.asm'

; without RTS, becouse 'sfx_playnote.asm' have it
