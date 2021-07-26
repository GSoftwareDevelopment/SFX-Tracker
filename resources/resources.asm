
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
	dta a(shortcutSFX)			;30

	dta a(val_menu_beatStep)	;31
	dta a(val_menu_tempo)		;32

	dta a(octaveShifts)        ;33
	dta a(pianoTuneOdd)			;34
	dta a(pianoTuneEven)			;35
	dta a(tuneIndChars)			;36

	dta a(themes_names_list)	;37

;message boxes
	dta a(msg_IO_DirPrompt)		;38
	dta a(msg_IO_SavePrompt)	;39
	dta a(msg_IO_LoadPrompt)	;40
	dta a(msg_IO_noFiles)		;41
	dta a(msg_IO_error)			;42
	dta a(msg_IO_reading)		;43
	dta a(msg_IO_writing)		;44
	dta a(msg_IO_skipping)		;45
	dta a(msg_IO_Quit)			;46
	dta a(but_YesNo)				;47
	dta a(msg_Theme_Overwrite)	;48
	dta a(msg_UnknownDefinition);49
	dta a(msg_SFX_ValuePrompt) ;50
	dta a(msg_TAB_JumpToPrompt);51
	dta a(msg_TAB_RepeatPrompt);52
	dta a(msg_TAB_FreqPrompt)	;53
	dta a(msg_TAB_SFXIdPrompt)	;54
	dta a(msg_BeatStepPrompt)	;55
	dta a(msg_SetTempoPrompt)	;56
	dta a(msg_pianoTuneInfo)	;57
	dta a(msg_newPrompt)			;58
	dta a(msg_ClipboardBadData);59
	dta a(msg_ClipboardEmpty)  ;60
	dta a(msg_ClipboardCopied) ;61
	dta a(msg_ClipboardPasted) ;62
	dta a(msg_mem_stats)       ;63

;app raw data
	dta a(app_logo)				;64
	dta a(app_virtual_piano)	;65

	dta a(dl_start)				;66
	dta a(DLI_color_schemas)	;67
	dta a(vis_tables)				;68
	dta a(charset)					;69
	dta a(chr_NoteShUp)			;70
	dta a(chr_NoteShDn)			;71
	dta a(chr_FreqShUp)			;72
	dta a(chr_FreqShDn)			;73

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
