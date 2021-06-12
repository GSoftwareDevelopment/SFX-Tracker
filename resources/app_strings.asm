strings
str_notDefined
					dta '- FREE -      ',255		; "not defined" SFX string

str_IO_nextPage
					dta d'PAGE >>>',255				; IO->DIR entry for next list page
str_IO_prevPage
					dta d'<<< PAGE',255				; IO->DIR entry for previous list page
str_NoteNames
;                    0         1         2         3         4         5         6         7
;                    012345678901234567890123456789012345678901234567890123456789012345678901234567890
					dta d'C-C#D-D#E-F-F#G-G#A-A#H-  >__>___ENDTAB___ __\\\   '
str_EndSONGOrder
					dta d'END-SONG'
;
wild_allFiles
					dta '*.*'

scan_to_scr
					dta d'0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ. !#%*-/:<>?_'
scan_key_codes
					dta 50,31,30,26,24,29,27,51,53,48	// 0-9
					dta 63,21,18,58,42,56,61,57,13,1,5,0,37,35,8,10,47,40,62,45,11,16,46,22,43,23	// A-Z
					dta 34	// . (dot)
					dta 33	// space
					dta 95	// exclamation mark (!)
					dta 90	// hash (#)
					dta 93	// percent (%)
					dta 7		// star (*)
					dta 14	// hypen (-)
					dta 38	// slash (/)
					dta 66	// colon (:)
					dta 54	// less sign (<)
					dta 55	// more sign (>)
					dta 102	// question mark (?)
					dta 78	// underscore mark (_)

scan_piano_codes
;NOTE     C  C# D D# E  F   F# G G# A A# B | C  C# D D# E  F   F# G G# A A# B   C  C# D D# E |
;OCTAVE   1                                | 2                                | 3            |
;       |   |  | |  |  |   |  | |  | |  |  |   |2 | |3 |  |   |5 | |6 | |7 |  |   |9 | |0 |  |
;       |   |S | |D |  |   |G | |H | |J |  |   |L | |; |  |   |  | |  | |  |  |   |  | |  |  |
;       |    |    |    |    |    |    |    | Q  | W  | E  | R  | T  | Y  | U  | I  | O  | P  |
;       | Z  | X  | C  | V  | B  | N  | M  | ,  | .  | /  |    |    |    |    |    |    |    |

;      				 Z  S  X  D  C  V  G  B  H  N  J  M  ,  L  .  ;  /
					dta 23,62,22,58,18,16,61,21,57,35, 1,37,32, 0,34, 2,38
;      				 Q  2  W  3  E  R  5  T  6  Y  7  U  I  9  O  0  P
					dta 47,30,46,26,42,40,29,45,27,43,51,11,13,48, 8,50,10

pianoTuneOdd
					dta $7,$b,$0,$b,$0,$7,$b,$0,$b,$0,$b,$6
pianoTuneEven
					dta $7,$8,$0,$8,$0,$7,$8,$0,$8,$0,$8,$6
tuneIndChars
		dta $3b,$3b	// f +7
		dta $3c,$3b	// e +6
		dta $3d,$3b	// d +5
		dta $3e,$3b	// c +4
		dta $00,$3b	// b +3
		dta $00,$3c	// a +2
		dta $00,$3d	// 9 +1
		dta $00,$3e	// 8 +0
		dta $1b,$00	// 7 -1
		dta $1d,$00	// 6 -2
		dta $20,$00	// 5 -3
		dta $3b,$00	// 4 -4
		dta $3b,$1b	// 3 -5
		dta $3b,$1d	// 2 -6
		dta $3b,$20	// 1 -7
		dta $3b,$3b	// 0 -8
