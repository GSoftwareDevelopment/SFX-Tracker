; this block processes the next line of the entry in SONG
; each time the TAB finishes playing

;
SONG_process
 .print "SONG PLAYBACK START: ", *

process_next_SONG_row
			clc
			adc #4	; shift SONG offset to next row
			tay

process_SONG_row_data
; first, check function in SONG row (always on 1st track)
			lda SONG_ADDR,y
			bmi check_SONG_order ; If the 7th bit is set, the function will be processed

;otherwise, process SONG row
			sty SONG_Ofs								; store SONG Offset

			icl 'SONG_RowProcess.asm'

check_SONG_order

			icl 'SONG_FuncProcess.asm'

exit_SONG_process                 ; end TRACK process
