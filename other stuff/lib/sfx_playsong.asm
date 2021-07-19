;input:
; Y - SONG offset (must point to the first track of the SONG row)

SONG_row_process
	lda SONG_ADDR,y 						; get first entry from SONG row
	bmi SONG_check_functions			; if 7th bit is set, it is SONG function

	ldx #$00									; channel

SONG_set_loop
	tya											; set current SONG Offset in
	sta SFX_CHANNELS_ADDR+_trackOfs,x	; current channel register
	pha											; store SONG offset on stack

	jsr SONG_TRACK_process

SONG_next_entry
	pla											; restore SONG offset from stack
	tay
	iny											; increase to next entry

	txa											; increase to next channel
	clc
	adc #$10
	tax
	cmp #$40										; check, if last channel
	bne SONG_set_loop

	rts

SONG_next_row_process
	tya
	clc
	adc #4

	jmp SONG_row_process

SONG_check_functions
	cmp #trkOrd_SetTempo
	bne SONGFn_check_Repeat

; SET TEMPO

SONGFn_SetTempo
	lda SONG_Addr+1,y							; get function parameter; tempo value
	sta SONG_TEMPO								; set tempo
	jmp SONG_next_row_process				; process next SONG row


SONGFn_check_Repeat
	cmp #trkOrd_Repeat
	bne check_SONGFn_JumpTo

; REPEAT

SONGFn_Repeat
	lda SONG_Rep
	bne SONG_is_in_loop						; if current SONG_Rep is not zero, means Song Is In Loop

	lda SONG_Addr+1,y							; get function parameter; repeat times value
	sta SONG_Rep								; set SONG repeat counter
	jmp SONG_Rep_JumpTo

SONG_is_in_loop
	dec SONG_Rep								; decrease SONG repeat counter
	bne SONG_Rep_JumpTo						; if SONG_Rep is not zero, Jump to position

SONG_is_out_of_loop
	jmp SONG_next_row_process				; if SONG_Rep is zero, process next SONG row

SONG_Rep_JumpTo
	lda SONG_Addr+2,y							; get secound function parameter; jump position value
	jmp SONGFn_JumpTo

; JUMP TO

check_SONGFn_JumpTo
	cmp #trkOrd_JumpTo
	bne check_SONGFn_SONG_END

	lda SONG_Addr+1,y							; get function parameter; jump position value
SONGFn_JumpTo
	asl @											; multiply this vale by 4 to get SONG offset
	asl @
	tay

	sty SFX_CHANNELS_ADDR+$00+_trackOfs
	iny
	sty SFX_CHANNELS_ADDR+$10+_trackOfs
	iny
	sty SFX_CHANNELS_ADDR+$20+_trackOfs
	iny
	sty SFX_CHANNELS_ADDR+$30+_trackOfs
	iny

	tay
	jmp SONG_row_process

check_SONGFn_SONG_END
	cmp #trkOrd_EndSong
	bne SONGFn_SONG_END

SONGFn_notRecognized
	jmp SONG_next_row_process

SONGFn_SONG_END
	jmp SFX_Off

; ---
;
;input:
; Y - SONG track offset
SONG_TRACK_process
	lda SONG_ADDR,y			; get SONG entry
	cmp #trkBlank
	beq SONG_TRACK_exit 		; if entry is BLANK/NOP then doo nothing - exit from subrutine
	bpl SONG_channel_off		; if entry is CHANNEL OFF then, turn off channel - call subroutine

	jmp SFX_SetTAB				; if none of the above, then set a TAB on the track,
									; SFX_SetTAB subroutine return from SONG_TRACK_process
SONG_TRACK_exit
   rts
SONG_channel_off
	jmp SFX_ChannelOff		; SFX_ChannelOff subroutine return from SONG_TRACK_process
