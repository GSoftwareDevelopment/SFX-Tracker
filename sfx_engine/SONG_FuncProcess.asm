			cmp #trkOrd_SetTempo
			bne check_SONGFn_Repeat

; SET TEMPO

SONGFn_SetTempo
			sty SONG_Ofs

			lda SONG_Addr+1,y							; get function parameter; tempo value
			sta SONG_TEMPO

			tya
			jmp process_next_SONG_row

check_SONGFn_Repeat
			cmp #trkOrd_Repeat
			bne check_SONGFn_JumpTo

; REPEAT

SONGFn_Repeat
			lda SONG_Rep
			bne SONG_is_in_loop						; if current SONG_Rep is not zero, means Song Is In Loop

			lda SONG_Addr+1,y							; get function parameter; repeat times value
			sta SONG_Rep
			jmp SONG_Rep_JumpTo

SONG_is_in_loop
			dec SONG_Rep								; decrease current SONG_Rep
			bne SONG_Rep_JumpTo						; if SONG_Rep is not zero, Jump to position

SONG_is_out_of_loop
			sty SONG_Ofs
			tya
			jmp process_next_SONG_row				; if SONG_Rep is zero, process next SONG row

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
			tay											; move calculated value to SONG offset register

; before making the jump, it is necessary to turn off all tracks
; as if they were playing from the beginning.

			jsr reset_all_tracks

			jmp process_SONG_row_data     	   ;

check_SONGFn_SONG_END
			cmp #trkOrd_EndSong
			bne SONGFn_notRecognized

			lda #$FF
			sta SONG_TICK_COUNTER

			jsr reset_all_tracks

			jmp end_tick

SONGFn_notRecognized
			sty SONG_Ofs
			tya
			jmp process_next_SONG_row
