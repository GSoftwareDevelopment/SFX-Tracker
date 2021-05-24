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
;	dta a(menu_ThemeEdit)		;8
	dta a(menu_IO)					;8

;strings
	dta a(str_SFX_notDefined)	;9
	dta a(str_TAB_types)			;10
	dta a(str_IO_nextPage)		;11
	dta a(str_IO_prevPage)		;12
	dta a(str_NoteNames)			;13

;message boxes
	dta a(msg_Credits)			;14
	dta a(msg_IO_DirPrompt)		;15
	dta a(msg_IO_SavePrompt)	;16
	dta a(msg_IO_LoadPrompt)	;17
	dta a(msg_IO_noFiles)		;18
	dta a(msg_IO_error)			;19
	dta a(msg_IO_reading)		;20
	dta a(msg_IO_writing)		;21
	dta a(msg_IO_skipping)		;22
	dta a(msg_IO_Quit)			;23
	dta a(but_IO_Quit)			;24
	dta a(msg_Theme_Overwrite)	;25
	dta a(but_Theme_Overwrite)	;26

; tables
;	dta a(color_themes)			;27

	icl 'app_menus.asm'
	icl 'app_strings.asm'
	icl 'app_messages.asm'
;	icl 'app_tables.asm'

; SUMMARY LOG

 .print "DATA SIZE: ", *-data_pointers_list
 .print "TABLE OF POINTERS SIZE: ",menus - data_pointers_list
 .print "MENUS : ", menus, "..", strings-1
 .print "STRINNGS : ", strings, "..", messages-1
 .print "MESSAGES : ", messages, "..", *-1
; .print "TABLES : ", messages, '..', *-1
