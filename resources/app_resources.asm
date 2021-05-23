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

;strings
	dta a(str_SFX_notDefined)	;10
	dta a(str_TAB_types)			;11
	dta a(str_IO_nextPage)		;12
	dta a(str_IO_prevPage)		;13
	dta a(str_NoteNames)			;14

;message boxes
	dta a(msg_Credits)			;15
	dta a(msg_IO_Prompt)			;16
	dta a(msg_IO_noFiles)		;17
	dta a(msg_IO_error)			;18
	dta a(msg_IO_reading)		;19
	dta a(msg_IO_writing)		;20
	dta a(msg_IO_skipping)		;21
	dta a(msg_IO_Quit)			;22
	dta a(but_IO_Quit)			;23
	dta a(msg_Theme_Overwrite)	;24
	dta a(but_Theme_Overwrite)	;25

; tables
	dta a(color_themes)			;26

	icl 'app_menus.asm'
	icl 'app_strings.asm'
	icl 'app_messages.asm'
	icl 'app_tables.asm'

; SUMMARY LOG

 .print "DATA SIZE: ", *-data_pointers_list
 .print "TABLE OF POINTERS SIZE: ",menus - data_pointers_list
 .print "MENUS : ", menus, "..", strings-1
 .print "STRINNGS : ", strings, "..", tables-1
 .print "STRINNGS : ", tables, "..", messages-1
 .print "TABLES : ", messages, '..', *-1
