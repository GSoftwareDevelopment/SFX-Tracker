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
	dta a(menu_song)				;13
	dta a(menu_song_option)		;14
	dta a(menu_song_edit)		;15
	dta a(menu_IO)					;16

;strings
	dta a(str_notDefined)		;17
	dta a(str_IO_nextPage)		;18
	dta a(str_IO_prevPage)		;19
	dta a(str_NoteNames)			;20
	dta a(str_EndSONGOrder)		;21
	dta a(wild_allFiles)			;22

	dta a(scan_to_scr)			;23
	dta a(scan_key_codes)		;24
	dta a(scan_piano_codes)		;25

	dta a(val_menu_beatStep)	;26
	dta a(val_menu_tempo)		;27

	dta a(pianoTuneOdd)			;28
	dta a(pianoTuneEven)			;29

;message boxes
	dta a(msg_IO_DirPrompt)		;30
	dta a(msg_IO_SavePrompt)	;31
	dta a(msg_IO_LoadPrompt)	;32
	dta a(msg_IO_noFiles)		;33
	dta a(msg_IO_error)			;34
	dta a(msg_IO_reading)		;35
	dta a(msg_IO_writing)		;36
	dta a(msg_IO_skipping)		;37
	dta a(msg_IO_Quit)			;38
	dta a(but_IO_Quit)			;39
	dta a(msg_Theme_Overwrite)	;40
	dta a(but_Theme_Overwrite)	;41
	dta a(msg_UnknownDefinition);42
	dta a(msg_SFX_ValuePrompt) ;43
	dta a(msg_TAB_JumpToPrompt);44
	dta a(msg_TAB_RepeatPrompt);45
	dta a(msg_TAB_FreqPrompt)	;46
	dta a(msg_TAB_SFXIdPrompt)	;47
	dta a(msg_BeatStepPrompt)	;48
	dta a(msg_SetTempoPrompt)	;49

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
