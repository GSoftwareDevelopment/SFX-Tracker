data_pointers_list
;menus
	dta a(menu_top)				;0
   dta a(menu_sfx)				;1
   dta a(menu_sfx_options)		;2
   dta a(menu_note_tune)		;3
   dta a(menu_tune_options)	;4
   dta a(menu_IO_options)		;5
	dta a(menu_NoteDef)			;6
   dta a(menu_sfx_mode)			;7
   dta a(menu_HFD_mode)			;8
   dta a(menu_MFD_mode)			;9
   dta a(menu_LFD_NLM_mode)	;10
   dta a(menu_DFD_mode)			;11

   dta a(menu_tabs)				;12
	dta a(menu_tabs_option)		;13
	dta a(menu_tab_edit)			;14
	dta a(menu_GSD)				;15
	dta a(menu_Themes)			;16
	dta a(menu_song)				;17
	dta a(menu_song_option)		;18
	dta a(menu_song_edit)		;19
	dta a(menu_IO)					;20

;strings
	dta a(str_notDefined)		;21
	dta a(str_IO_nextPage)		;22
	dta a(str_IO_prevPage)		;23
	dta a(str_NoteNames)			;24
	dta a(str_EndSONGOrder)		;25
	dta a(wild_allFiles)			;26

	dta a(scan_to_scr)			;27
	dta a(scan_key_codes)		;28
	dta a(scan_piano_codes)		;29

	dta a(val_menu_beatStep)	;30
	dta a(val_menu_tempo)		;31

	dta a(pianoTuneOdd)			;32
	dta a(pianoTuneEven)			;33
	dta a(tuneIndChars)			;34

;message boxes
	dta a(msg_IO_DirPrompt)		;35
	dta a(msg_IO_SavePrompt)	;36
	dta a(msg_IO_LoadPrompt)	;37
	dta a(msg_IO_noFiles)		;38
	dta a(msg_IO_error)			;39
	dta a(msg_IO_reading)		;40
	dta a(msg_IO_writing)		;41
	dta a(msg_IO_skipping)		;42
	dta a(msg_IO_Quit)			;43
	dta a(but_IO_Quit)			;44
	dta a(msg_Theme_Overwrite)	;45
	dta a(but_Theme_Overwrite)	;46
	dta a(msg_UnknownDefinition);47
	dta a(msg_SFX_ValuePrompt) ;48
	dta a(msg_TAB_JumpToPrompt);49
	dta a(msg_TAB_RepeatPrompt);50
	dta a(msg_TAB_FreqPrompt)	;51
	dta a(msg_TAB_SFXIdPrompt)	;52
	dta a(msg_BeatStepPrompt)	;53
	dta a(msg_SetTempoPrompt)	;54
	dta a(msg_pianoTuneInfo)	;55

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
