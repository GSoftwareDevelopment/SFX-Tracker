; input registers
; x - current channel offset ($00, $10, $20, $30)
;
TRACK_process
			ldy SFX_CHANNELS_ADDR+_trackOfs,x    ; get SONG-Track offset
			bpl process_TRACK_data
			jmp no_TRACK_process                 ; exit from TRACK_process

 .print "SONG PLAYBACK START: ", *

process_TRACK_data
			iny	; shift track offset to next SONG-Track row
			iny
			iny
			iny
			sty _regTemp

; first, check function (always in 1st track) on current track offset
			tya
			and #%11111100
			tay
			lda SONG_ADDR,y
			bmi check_song_orders ; If the 7th bit is set, the function will be processed

;otherwise, process the value from the current track
			ldy _regTemp	; restore current TRACK offset
			tya
			sta SFX_CHANNELS_ADDR+_trackOfs,x	; store current TRACK offest in Channels registers
			lda SONG_ADDR,y							; get track data

			cmp #trkBlank
			bne check_track_off

track_blank
; track_blank does not change anything.
; If a TAB in a track was played, it will be played from the beginning
			jmp no_TRACK_process                 ; exit from TRACK_process

check_track_off
			cmp #trkOff
			bne set_TABId_in_Track

;track_off turns off track playback

			lda #TAB_OFF
			sta SFX_CHANNELS_ADDR+_tabOfs,x
			tay	; copy A to Y
			dey	; and decrement Y
			jmp TAB_FN_NoteOff						; turn off current SFX and mute audio channel

set_TABId_in_Track
; there is a TABa Id in the A registry
			stx _regTemp2	; store the offset value of the Channels register
								; for the duration of the address calculation

			asl @                               ; multiply by 2 to get TAB offset
			tax

; in the X register there is an TAB ID offset of the TAB definition array
.ifndef DONT_CALC_ABS_ADDR
; calulate absolute address for TAB definition
         lda TAB_TABLE_ADDR,x                  ; take low part of tab Pointer
         clc
         adc #<DATA_ADDR
         sta TABPtr
         lda TAB_TABLE_ADDR+1,x
         adc #>DATA_ADDR
         sta TABPtr+1

.ifndef DONT_CALC_SFX_NAMES
			lda TABPtr
			clc
         adc #TAB_NameLength                    ; incrase about TAB name length
         sta TABPtr
         bcc no_TAB_overflow
         inc TABPtr+1
no_TAB_overflow
.endif

.else
			lda TAB_TABLE_ADDR,x						; get TAB Offset definition
			sta TABPtr
			lda TAB_TABLE_ADDR+1,x
			sta TABPtr+1
.endif

			ldx _regTemp2	; restore channel register offset

; write the calculated address of the TAB definition to the channel register
			lda TABPtr
			sta SFX_CHANNELS_ADDR+_tabPtrLo,x
			lda TABPtr+1
			sta SFX_CHANNELS_ADDR+_tabPtrHi,x
			jmp no_TRACK_process

check_song_orders
			cmp #trkOrd_SetTempo
			bne check_song_JumpTo

song_fn_SetTempo
			jmp no_TRACK_process                 ; exit from TRACK_process

check_song_JumpTo
			cmp #trkOrd_JumpTo
			bne check_song_Repeat

song_fn_JumpTo
			jmp no_TRACK_process                 ; exit from TRACK_process

check_song_Repeat
			cmp #trkOrd_Repeat
			bne song_fn_End

song_fn_Repeat
			jmp no_TRACK_process                 ; exit from TRACK_process

song_fn_End

no_TRACK_process                 ; end TRACK process
