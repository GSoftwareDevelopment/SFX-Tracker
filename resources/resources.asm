
	opt h-
	org $C000

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
	dta a(menu_song)				;16
	dta a(menu_song_option)		;17
	dta a(menu_song_edit)		;18
	dta a(menu_IO)					;19

	dta a(menu_settings)       ;20

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

	dta a(octaveShifts)        ;32
	dta a(pianoTuneOdd)			;33
	dta a(pianoTuneEven)			;34
	dta a(tuneIndChars)			;35

	dta a(themes_names_list)	;36

;message boxes
	dta a(msg_IO_DirPrompt)		;37
	dta a(msg_IO_SavePrompt)	;38
	dta a(msg_IO_LoadPrompt)	;39
	dta a(msg_IO_noFiles)		;40
	dta a(msg_IO_error)			;41
	dta a(msg_IO_reading)		;42
	dta a(msg_IO_writing)		;43
	dta a(msg_IO_skipping)		;44
	dta a(msg_IO_Quit)			;45
	dta a(but_YesNo)				;46
	dta a(msg_Theme_Overwrite)	;47
	dta a(msg_UnknownDefinition);48
	dta a(msg_SFX_ValuePrompt) ;49
	dta a(msg_TAB_JumpToPrompt);50
	dta a(msg_TAB_RepeatPrompt);51
	dta a(msg_TAB_FreqPrompt)	;52
	dta a(msg_TAB_SFXIdPrompt)	;53
	dta a(msg_BeatStepPrompt)	;54
	dta a(msg_SetTempoPrompt)	;55
	dta a(msg_pianoTuneInfo)	;56
	dta a(msg_newPrompt)			;57
	dta a(msg_ClipboardBadData);58
	dta a(msg_ClipboardEmpty)  ;59
	dta a(msg_ClipboardCopied) ;60
	dta a(msg_ClipboardPasted) ;61
	dta a(msg_mem_stats)       ;62

;app raw data
	dta a(app_logo)				;63
	dta a(app_virtual_piano)	;64

	dta a(dl_start)
	dta a(DLI_color_schemas)
	dta a(vis_tables)
	dta a(charset)
	dta a(chr_NoteShUp)
	dta a(chr_NoteShDn)
	dta a(chr_FreqShUp)
	dta a(chr_FreqShDn)

	icl 'app_menus.asm'
	icl 'app_strings.asm'
	icl 'app_messages.asm'
	icl 'app_data.asm'

	icl "dlist.asm"
	icl "themes_colors.asm"
	icl "vis_table.asm"
	icl "charset.asm"

; SUMMARY LOG

 .print "DATA SIZE: ", *-data_pointers_list
 .print "TABLE OF POINTERS SIZE: ",menus - data_pointers_list
 .print "MENUS : ", menus, "..", strings-1
 .print "STRINNGS : ", strings, "..", messages-1
 .print "MESSAGES : ", messages, "..", app_data-1
 .print "DATA : ", app_data, "..", *-1
 .print "DLIST SIZE: ", DLI_color_schemas - dl_start
