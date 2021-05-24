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


; menu_ThemeEdit
;					dta  40,d'MENU AREA',255		; color scheme definition
;					dta  60,d'ITEM',255				; color scheme definition
;					dta  80,d'BACKGROUND',255		; color scheme definition
;					dta 100,d'SELECTED',255			; color scheme definition
;					dta 120,d'BORDER',255			; color scheme definition
;					dta 180,d'STORE',255				; store modified/current color theme
;					dta 200,d'< BACK',255			; back to theme manager
;					dta 255

menu_IO
					dta  0,d'<',255					; back to main menu
					dta  2,d'DIR',255					; directory - list of files
					dta 6,d'LOAD',255					; load - SFX/TAB/SONG
					dta 11,d'SAVE',255				; sabe - SFX/TAB/SONF
					dta 16,d'QUIT',255				; quit to dos
					dta 255
