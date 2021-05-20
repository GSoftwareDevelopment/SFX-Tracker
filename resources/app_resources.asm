data_pointers_list
;menus
	dta a(menu_top)				;0
   dta a(menu_sfx)				;1
   dta a(menu_sfx_options)		;2
   dta a(menu_tabs)				;3
	dta a(menu_tabs_option)		;4
	dta a(menu_tab_edit)			;5
	dta a(menu_GSD)				;6
	dta a(menu_ThemeEdit)		;7
	dta a(menu_IO)					;8
	dta a(menu_YesNo);			;9

;strings
	dta a(str_SFX_notDefined)	;10
	dta a(str_TAB_note_names)  ;11
	dta a(str_TAB_types)			;12
	dta a(str_Credits)			;13
	dta a(str_IO_Prompt)			;14
	dta a(str_IO_noFiles)		;15
	dta a(str_IO_Error)			;16
	dta a(str_IO_Quit)			;17

; tables
	dta a(color_themes)			;18

menus
menu_top
					dta   1,d'"$',255		; text start ofset; text data; terminated value 255
					dta   4,d'IO',255
					dta   7,d'SFX',255
					dta  11,d'TAB',255
					dta  15,d'SONG',255
					dta 255					; end of definition

menu_sfx
					dta  20,d'#',255
					dta  40,d'>>>',255
					dta 100,d'PLY',255
					dta 120,d'VOL',255
					dta 140,d'DST',255
					dta 160,d'MOD',255
					dta 180,d'VAL',255
					dta 200,d'OPT',255
					dta 255

menu_sfx_options
					dta 84,d'KEY NOTE',255
					dta 104,d'CLONE',255
					dta 124,d'INSERT',255
					dta 144,d'DELETE',255
					dta 164,d'STORE SET',255
					dta 184,d'< BACK',255
					dta 204,d'IO>SAVE SET',255
					dta 255

menu_tabs
					dta  20,d'#',255
					dta  40,d'>>>',255
					dta 180,d'PLY',255
					dta 200,d'OPT',255
					dta 220,d'#',255
					dta 255

menu_tabs_option
					dta 104,d'CLONE',255
					dta 124,d'INSERT',255
					dta 144,d'DELETE',255
					dta 164,d'STORE SET',255
					dta 184,d'< BACK',255
					dta 204,d'IO>SAVE SET',255
					dta 255
menu_tab_edit
					dta  46,7,d'END TAB',255
					dta  66,7,d'JUMP TO',255
					dta  86,7,d'REPEAT',255
					dta 106,7,d'NOTE VALUE',255
					dta 126,7,d'SFX CHANGE',255
					dta 146,7,d'NOP',255
					dta 166,7,d'< BACK',255
					dta 255

menu_GSD
					dta 0,d'<',255
					dta 2,d'CREDITS',255
					dta 10,d'THEME',255
					dta 16,d'FREE',255
					dta 255

menu_ThemeEdit
					dta  40,d'MENU AREA',255
					dta  60,d'ITEM',255
					dta  80,d'BACKGROUND',255
					dta 100,d'SELECTED',255
					dta 120,d'BORDER',255
					dta 180,d'SAVE',255
					dta 200,d'< BACK',255
					dta 255

menu_IO
					dta  0,d'<',255
					dta  2,d'DIR',255
					dta 6,d'LOAD',255
					dta 11,d'SAVE',255
					dta 16,d'QUIT',255
					dta 255

menu_YesNo
					dta 62,07,d'YES',255
					dta 82,07,d'NO',255
					dta 255

strings
str_SFX_notDefined
					dta ' - FREE SFX - ',255

str_TAB_note_names
//                   0         1         2         3         4         5         6         7
//                   012345678901234567890123456789012345678901234567890123456789012345678901234567890
					dta d'C-C#D-D#E-F-F#G-G#A-A#H-JT>R_>___ENDTAB___ _____<';

str_TAB_types
					dta 'FREE TAB',255

str_Credits
					dta 85,d'THIS PLACE',255
					dta 103,d'IS WAITING FOR',255
					dta 128,d'YOU!',255
					dta 180,d'SUPPORT THIS PROJECT',255
					dta 201,d'BIT.LY/SFX-TRACKER',255
					dta 255

str_IO_Prompt
					dta d'DEVICE:PATH',255
str_IO_noFiles
					dta d'NO FILES',255
str_IO_Error
					dta d'I/O ERROR #',255
str_IO_Quit
					dta 07,d'ARE YOU LEAVE?',06,255
tables
color_themes
					dta $0a,$e6,$68,$34,$00,d'LIGHT',255
					dta $00,$02,$04,$30,$08,d'DARK',255
					dta $00,$00,$00,$00,$00,d'CUSTOM 1',255
					dta $00,$00,$00,$00,$00,d'CUSTOM 2',255
					dta $00,$00,$00,$00,$00,d'CUSTOM 3',255
					dta $00,$00,$00,$00,$00,d'CUSTOM 4',255
					dta $00,$00,$00,$00,$00,d'CUSTOM 5',255
					dta $00,$00,$00,$00,$00,d'CUSTOM 6',255
					dta $00,$00,$00,$00,$00,d'CUSTOM 7',255
					dta 255

; SUMMARY LOG

 .print "DATA SIZE: ", *-data_pointers_list
 .print "TABLE OF POINTERS SIZE: ",menus - data_pointers_list
 .print "MENUS : ", menus, "..", strings-1
 .print "STRINNGS : ", strings, "..", tables-1
 .print "TABLES : ", tables, '..', *-1
