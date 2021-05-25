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
	dta a(str_SFX_notDefined)	;14
	dta a(str_TAB_types)			;15
	dta a(str_IO_nextPage)		;16
	dta a(str_IO_prevPage)		;17
	dta a(str_NoteNames)			;18

;message boxes
	dta a(msg_IO_DirPrompt)		;19
	dta a(msg_IO_SavePrompt)	;20
	dta a(msg_IO_LoadPrompt)	;21
	dta a(msg_IO_noFiles)		;22
	dta a(msg_IO_error)			;23
	dta a(msg_IO_reading)		;24
	dta a(msg_IO_writing)		;25
	dta a(msg_IO_skipping)		;26
	dta a(msg_IO_Quit)			;27
	dta a(but_IO_Quit)			;28
	dta a(msg_Theme_Overwrite)	;29
	dta a(but_Theme_Overwrite)	;30
	dta a(msg_SFX_ValuePrompt) ;31

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
