data_pointers_list
;menus
	dta a(menu_top)				;0
   dta a(menu_sfx)				;1
   dta a(menu_sfx_options)		;2
   dta a(menu_note_tune)		;3
   dta a(menu_tune_options)	;4
   dta a(menu_sfx_mode)			;5
   dta a(menu_HFD_mode)			;6
   dta a(menu_MFD_mode)			;7
   dta a(menu_LFD_NLM_mode)	;8
   dta a(menu_DFD_mode)			;9

   dta a(menu_tabs)				;10
	dta a(menu_tabs_option)		;11
	dta a(menu_tab_edit)			;12
	dta a(menu_GSD)				;13
	dta a(menu_Themes)			;14
	dta a(menu_song)				;15
	dta a(menu_song_option)		;16
	dta a(menu_song_edit)		;17
	dta a(menu_IO)					;18

;strings
	dta a(str_notDefined)		;19
	dta a(str_IO_nextPage)		;20
	dta a(str_IO_prevPage)		;21
	dta a(str_NoteNames)			;22
	dta a(str_EndSONGOrder)		;23
	dta a(wild_allFiles)			;24

	dta a(scan_to_scr)			;25
	dta a(scan_key_codes)		;26
	dta a(scan_piano_codes)		;27

	dta a(val_menu_beatStep)	;28
	dta a(val_menu_tempo)		;29

	dta a(pianoTuneOdd)			;30
	dta a(pianoTuneEven)			;31
	dta a(tuneIndChars)			;32

;message boxes
	dta a(msg_IO_DirPrompt)		;33
	dta a(msg_IO_SavePrompt)	;34
	dta a(msg_IO_LoadPrompt)	;35
	dta a(msg_IO_noFiles)		;36
	dta a(msg_IO_error)			;37
	dta a(msg_IO_reading)		;38
	dta a(msg_IO_writing)		;39
	dta a(msg_IO_skipping)		;40
	dta a(msg_IO_Quit)			;41
	dta a(but_IO_Quit)			;42
	dta a(msg_Theme_Overwrite)	;43
	dta a(but_Theme_Overwrite)	;44
	dta a(msg_UnknownDefinition);45
	dta a(msg_SFX_ValuePrompt) ;46
	dta a(msg_TAB_JumpToPrompt);47
	dta a(msg_TAB_RepeatPrompt);48
	dta a(msg_TAB_FreqPrompt)	;49
	dta a(msg_TAB_SFXIdPrompt)	;50
	dta a(msg_BeatStepPrompt)	;51
	dta a(msg_SetTempoPrompt)	;52

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
