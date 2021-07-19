; disable all playback (SFX/TAB/SONG)
;
; no input

			lda #$FF
			sta SONG_Ofs				;	prevent SONG processing
			lda #$80
			sta SONG_TICK_COUNTER	;	prevent playback processing
;
;
;
; subroutine for reset all tracks

reset_all_tracks
			ldx #$30

reset_TRACKS
			jsr SFX_OFF_CHANNEL

			txa											; change current channel
			sec
			sbc #$10
			tax
			bpl reset_TRACKS

			rts
