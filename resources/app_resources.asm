data_pointers_list
;menus
	dta a(menu_top)				;0
   dta a(menu_sfx)				;1
   dta a(menu_sfx_options)		;2
   dta a(menu_tabs)				;3
	dta a(menu_tabs_option)		;4
	dta a(menu_tab_edit)			;5
	dta a(menu_GSD)				;6
	dta a(menu_Themes)			;7
	dta a(menu_ThemeEdit)		;8
	dta a(menu_IO)					;9
	dta a(menu_YesNo);			;10

;strings
	dta a(str_SFX_notDefined)	;11
	dta a(str_TAB_types)			;12
	dta a(str_Credits)			;13
	dta a(str_IO_Prompt)			;14
	dta a(str_IO_noFiles)		;15
	dta a(str_IO_nextPage)		;16
	dta a(str_IO_prevPage)		;17
	dta a(str_IO_error)			;18
	dta a(str_IO_reading)		;19
	dta a(str_IO_writing)		;20
	dta a(str_IO_skipping)		;21
	dta a(str_IO_Quit)			;22

; tables
	dta a(table_note_names)		;23
	dta a(color_themes)			;24

menus

; A menu or list of options to choose from. is defined by listing each option, starting with:
; - a line item on the screen
; - a string of characters (with screen codes) terminated by a $ff byte
; - the list ends with the $ff byte;

menu_top
					dta   1,d'"$',255		; GSD module
					dta   4,d'IO',255		; IO module
					dta   7,d'SFX',255	; SFX module
					dta  11,d'TAB',255	; TAB module
					dta  15,d'SONG',255	; SONG module
					dta 255

menu_sfx
					dta  20,d'#',255			; current SFX (number)
					dta  40,d'>>>',255		; edit SFX
					dta  60,d'PLY',255		; play SFX
					dta 120,d'VOL',255		; edit SFX Volume
					dta 140,d'DST',255		; edit SFX Distortion
					dta 160,d'MOD',255		; edit SFX Mod
					dta 180,d'VAL',255		; edit SFX Mod Value
					dta 200,d'OPT',255		; SFX Options
					dta 255

menu_sfx_options
					dta 164,d'SET NAME',255			; set SFX name
					dta 184,d'KEY NOTE',255			; set key note table for SFX
					dta 204,d'< BACK',255			; back to SFX menu bar
					dta 255

menu_tabs
					dta  20,d'#',255					; current TAB (number)
					dta  40,d'>>>',255				; edit current TAB
					dta 180,d'PLY',255				; play current TAB
					dta 200,d'OPT',255				; TAB options
					dta 220,d'#',255					; current SFX (number)
					dta 255

menu_tabs_option
					dta 184,d'SET NAME',255			; set TAB name
					dta 204,d'< BACK',255			; back to TAB menu bar
					dta 255
menu_tab_edit
					dta  46,7,d'END TAB',255		; flag indicating the end of a TAB definition
					dta  66,7,d'JUMP TO',255		; marker for absolute jump to line in current TAB
					dta  86,7,d'REPEAT',255			; marker that defines a repeat n times from the pos position.
					dta 106,7,d'NOTE VALUE',255	; flag specifying the value of the frequency divider for the played SFX
															; (direct value to the POKEY register, not from the note table)
					dta 126,7,d'SFX CHANGE',255	; A marker that changes the SFX currently being played, without changing the note/divider
					dta 146,7,d'NOP',255				; blank entry - no operation
					dta 166,7,d'BACK',255			; back to TAB edit
					dta 186,7,d'< EXIT',255			; back to TAB menu bar
					dta 255

menu_GSD
					dta 0,d'<',255						; back to main menu
					dta 2,d'CREDITS',255				; credits
					dta 10,d'THEME',255				; theme manager
					dta 16,d'FREE',255				; memory statistics
					dta 255

menu_Themes
					dta 163,d'SET AS DEFAULT',255						; option to set the current set of topics as the default
																				; - this set is saved as 'default.sth'.
																				; When starting the program, the existence of the file
																				; DEFAULT.THM' is checked on devices 'D:' and 'H:'.
					dta 183,d'LOAD THEME SET',255						; As the name suggests, it allows you to load a set of topics
					dta 203,d'SAVE THEME SET',255						; like above, but save
					dta 255

menu_ThemeEdit
					dta  40,d'MENU AREA',255		; color scheme definition
					dta  60,d'ITEM',255				; color scheme definition
					dta  80,d'BACKGROUND',255		; color scheme definition
					dta 100,d'SELECTED',255			; color scheme definition
					dta 120,d'BORDER',255			; color scheme definition
					dta 180,d'STORE',255				; store modified/current color theme
					dta 200,d'< BACK',255			; back to theme manager
					dta 255

menu_IO
					dta  0,d'<',255					; back to main menu
					dta  2,d'DIR',255					; directory - list of files
					dta 6,d'LOAD',255					; load - SFX/TAB/SONG
					dta 11,d'SAVE',255				; sabe - SFX/TAB/SONF
					dta 16,d'QUIT',255				; quit to dos
					dta 255

menu_YesNo
					dta 62,07,d'YES',255
					dta 82,07,d'NO',255
					dta 255

strings
str_SFX_notDefined
					dta ' - FREE SFX - ',255		; "not defined" SFX string

str_TAB_types
					dta 'FREE TAB',255				; "not defined" TAB string

str_Credits
					dta 25,d'THANKS TO:',255
					dta 48,d'PIN',255
					dta 61,d'FOR HARDWARE TESTS',255
					dta 105,d'PIN MONO',255
					dta 126,d'JHUSAK',255
					dta 143,d'FOR KNOWLEDGE',255
					dta 165,d'SUPPORTING',255

					dta 200,d'SUPPORT THIS PROJECT',255
					dta 221,d'BIT.LY/SFX-TRACKER',255
					dta 255

str_IO_Prompt
					dta d'DEVICE:PATH',255			; IO->DIR prompt message
str_IO_noFiles
					dta d'NO FILES',255				; IO->DIR message for empty disk
str_IO_nextPage
					dta d'PAGE >>>',255				; IO->DIR entry for next list page
str_IO_prevPage
					dta d'<<< PAGE',255				; IO->DIR entry for previous list page
str_IO_error
					dta d'I/O ERROR #',255			; IO error message with it number
str_IO_reading
					dta d'READING...',255			; IO message for reading operation
str_IO_writing
					dta d'WRITING...',255			; IO message for writing operation
str_IO_skipping
					dta d'SKIPPING...',255			; IO message for skipping - ony IO->DIR when page is changing
str_IO_Quit
					dta 07,d'ARE YOU LEAVE?',06,255	; quit ask message

tables
color_themes
					dta $0a,$e6,$68,$34,$00,d'LIGHT',255		; predefined theme set
					dta $00,$02,$04,$30,$08,d'DARK',255			; predefined theme set
					dta $00,$00,$00,$00,$00,d'CUSTOM 1',255	; custom theme set
					dta $00,$00,$00,$00,$00,d'CUSTOM 2',255
					dta $00,$00,$00,$00,$00,d'CUSTOM 3',255
					dta $00,$00,$00,$00,$00,d'CUSTOM 4',255
					dta 255

table_note_names
//                   0         1         2         3         4         5         6         7
//                   012345678901234567890123456789012345678901234567890123456789012345678901234567890
					dta d'C-C#D-D#E-F-F#G-G#A-A#H-JT>R_>___ENDTAB___ _____<';

; SUMMARY LOG

 .print "DATA SIZE: ", *-data_pointers_list
 .print "TABLE OF POINTERS SIZE: ",menus - data_pointers_list
 .print "MENUS : ", menus, "..", strings-1
 .print "STRINNGS : ", strings, "..", tables-1
 .print "TABLES : ", tables, '..', *-1
