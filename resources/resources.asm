
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

;strings
	dta a(str_notDefined)		;20
	dta a(str_IO_Page)			;21
	dta a(str_NoteNames)			;22
	dta a(str_EndSONGOrder)		;23
	dta a(wild_allFiles)			;24

	dta a(scan_to_scr)			;25
	dta a(scan_key_codes)		;26
	dta a(scan_piano_codes)		;27
	dta a(shortcutSFX)			;28

	dta a(val_menu_beatStep)	;29
	dta a(val_menu_tempo)		;30

	dta a(octaveShifts)        ;31
	dta a(pianoTuneOdd)			;32
	dta a(pianoTuneEven)			;33
	dta a(tuneIndChars)			;34

	dta a(themes_names_list)	;35

;message boxes
	dta a(msg_IO_DirPrompt)		;36
	dta a(msg_IO_SavePrompt)	;37
	dta a(msg_IO_LoadPrompt)	;38
	dta a(msg_IO_noFiles)		;39
	dta a(msg_IO_error)			;40
	dta a(msg_IO_reading)		;41
	dta a(msg_IO_writing)		;42
	dta a(msg_IO_skipping)		;43
	dta a(msg_IO_Quit)			;44
	dta a(but_YesNo)				;45
	dta a(msg_Theme_Overwrite)	;46
	dta a(msg_UnknownDefinition);47
	dta a(msg_SFX_ValuePrompt) ;48
	dta a(msg_TAB_JumpToPrompt);49
	dta a(msg_TAB_RepeatPrompt);50
	dta a(msg_TAB_FreqPrompt)	;51
	dta a(msg_TAB_SFXIdPrompt)	;52
	dta a(msg_BeatStepPrompt)	;53
	dta a(msg_SetTempoPrompt)	;54
	dta a(msg_pianoTuneInfo)	;55
	dta a(msg_newPrompt)			;56
	dta a(msg_ClipboardBadData);57
	dta a(msg_ClipboardEmpty)  ;58
	dta a(msg_ClipboardCopied) ;59
	dta a(msg_ClipboardPasted) ;60
	dta a(msg_mem_stats)       ;61
	dta a(msg_SFX_FastKey_Assigned);62
	dta a(msg_SFX_FastKey_Cleared) ;63
	dta a(msg_TAB_StepChange); ;64

;app raw data
	dta a(app_logo)				;65
	dta a(app_virtual_piano)	;

	dta a(dl_start)				;
	dta a(DLI_color_schemas)	;
	dta a(vis_tables)				;
	dta a(charset)					;
	dta a(chr_NoteShUp)			;
	dta a(chr_NoteShDn)			;
	dta a(chr_FreqShUp)			;
	dta a(chr_FreqShDn)			;

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
