;input:
; X - channel
; Y - SFX index
; A - note
;output:
; Y - SFX pointer offset

; set note in current channel
	sta SFX_CHANNELS_ADDR+_chnNote,x

; set note table
	lda #NOTE_TABLE_PAGE
	sta self_note_table_addr+1
	lda SFX_NOTE_SET_ADDR,y
	sta SFX_CHANNELS_ADDR+_sfxNoteTabOfs,x
	sta self_note_table_addr

; set frequency from note table
	lda SFX_CHANNELS_ADDR+_chnNote,x
	tay

	lda $FFFF,y
self_note_table_addr *-2

set_channel_frequency
	sta SFX_CHANNELS_ADDR+_chnFreq,x

; set SFX modulator mode
	lda SFX_MODE_SET_ADDR,y
	sta SFX_CHANNELS_ADDR+_chnMode,x

set_sfx_pointer_and_play
; set SFX data pointer
	tay
	asl @
	lda SFX_TABLE_ADDR,y
	sta SFX_CHANNELS_ADDR+_sfxPtrLo,x
	lda SFX_TABLE_ADDR+1,y
	sta SFX_CHANNELS_ADDR+_sfxPtrHi,x

; set SFX offset
	lda #$00
	sta SFX_CHANNELS_ADDR+_chnOfs,x

	rts
