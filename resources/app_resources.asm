data_pointers_list
;menus
	dta a(menu_top)				;0
   dta a(menu_sfx)				;1
   dta a(menu_sfx_options)		;2
   dta a(menu_note_tune)		;3
   dta a(menu_tune_options)	;4
	dta a(menu_NoteDef)			;5
   dta a(menu_sfx_mode)			;6
   dta a(menu_HFD_mode)			;7
   dta a(menu_MFD_mode)			;8
   dta a(menu_LFD_NLM_mode)	;9
   dta a(menu_DFD_mode)			;10

   dta a(menu_tabs)				;11
	dta a(menu_tabs_option)		;12
	dta a(menu_tab_edit)			;13
	dta a(menu_GSD)				;14
	dta a(menu_Themes)			;15
	dta a(menu_song)				;16
	dta a(menu_song_option)		;17
	dta a(menu_song_edit)		;18
	dta a(menu_IO)					;19

;strings
	dta a(str_notDefined)		;20
	dta a(str_IO_nextPage)		;21
	dta a(str_IO_prevPage)		;22
	dta a(str_NoteNames)			;23
	dta a(str_EndSONGOrder)		;24
	dta a(wild_allFiles)			;25

	dta a(scan_to_scr)			;26
	dta a(scan_key_codes)		;27
	dta a(scan_piano_codes)		;28

	dta a(val_menu_beatStep)	;29
	dta a(val_menu_tempo)		;30

	dta a(pianoTuneOdd)			;31
	dta a(pianoTuneEven)			;32
	dta a(tuneIndChars)			;33

;message boxes
	dta a(msg_IO_DirPrompt)		;34
	dta a(msg_IO_SavePrompt)	;35
	dta a(msg_IO_LoadPrompt)	;36
	dta a(msg_IO_noFiles)		;37
	dta a(msg_IO_error)			;38
	dta a(msg_IO_reading)		;39
	dta a(msg_IO_writing)		;40
	dta a(msg_IO_skipping)		;41
	dta a(msg_IO_Quit)			;42
	dta a(but_IO_Quit)			;43
	dta a(msg_Theme_Overwrite)	;44
	dta a(but_Theme_Overwrite)	;45
	dta a(msg_UnknownDefinition);46
	dta a(msg_SFX_ValuePrompt) ;47
	dta a(msg_TAB_JumpToPrompt);48
	dta a(msg_TAB_RepeatPrompt);49
	dta a(msg_TAB_FreqPrompt)	;50
	dta a(msg_TAB_SFXIdPrompt)	;51
	dta a(msg_BeatStepPrompt)	;52
	dta a(msg_SetTempoPrompt)	;53
	dta a(msg_pianoTuneInfo)	;54

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
