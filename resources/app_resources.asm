data_pointers_list
;menus
	dta a(menu_top)				;0
   dta a(menu_sfx)				;1
   dta a(menu_sfx_options)		;2
   dta a(menu_sfx_mode)			;3
   dta a(menu_HFD_mode)			;4
   dta a(menu_MFD_mode)			;5
   dta a(menu_LFD_NLM_mode)	;6
   dta a(menu_DFD_mode)			;7

   dta a(menu_tabs)				;8
	dta a(menu_tabs_option)		;9
	dta a(menu_tab_edit)			;10
	dta a(menu_GSD)				;11
	dta a(menu_Themes)			;12
	dta a(menu_IO)					;13

;strings
	dta a(str_notDefined)		;14
	dta a(str_IO_nextPage)		;15
	dta a(str_IO_prevPage)		;16
	dta a(str_NoteNames)			;17

;message boxes
	dta a(msg_IO_DirPrompt)		;18
	dta a(msg_IO_SavePrompt)	;19
	dta a(msg_IO_LoadPrompt)	;20
	dta a(msg_IO_noFiles)		;21
	dta a(msg_IO_error)			;22
	dta a(msg_IO_reading)		;23
	dta a(msg_IO_writing)		;24
	dta a(msg_IO_skipping)		;25
	dta a(msg_IO_Quit)			;26
	dta a(but_IO_Quit)			;27
	dta a(msg_Theme_Overwrite)	;28
	dta a(but_Theme_Overwrite)	;29
	dta a(msg_SFX_ValuePrompt) ;30

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
