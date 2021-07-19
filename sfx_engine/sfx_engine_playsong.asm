			lda #$80						; prevent SONG process
			sta SONG_TICK_COUNTER
			sty SONG_Ofs				; store SONG start position (offset! not row)

			jsr reset_all_tracks		;

			jsr process_SONG_row_data	; preset SONG

			lda #$00
			sta SONG_Rep				; reset repeat counter
			sta SONG_TICK_COUNTER	; run SONG process

			rts

;
;
;
; subroutine to process SONG entry

			icl 'SONG.asm'
