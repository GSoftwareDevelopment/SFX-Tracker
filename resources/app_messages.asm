messages
msg_IO_DirPrompt
               dta 20,d'SELECT FILE:',255,255         ; IO->DIR prompt message
msg_IO_SavePrompt
               dta 200,d'SAVE AS:',255,255
msg_IO_LoadPrompt
               dta 180,d'LOAD:',255
               dta 200,d'USE WILDCARD FOR DIR',255,255
msg_IO_noFiles
               dta 126,d'NO FILES',255,255            ; IO->DIR message for empty disk
msg_IO_error
               dta 223,d'I/O ERROR #',255,255         ; IO error message with it number
msg_IO_reading
               dta 220,d'READING...',255,255       ; IO message for reading operation
msg_IO_writing
               dta 220,d'WRITING...',255,255       ; IO message for writing operation
msg_IO_skipping
               dta 220,d'SKIPPING...',255,255         ; IO message for skipping - ony IO->DIR when page is changing
msg_IO_Quit
               dta 23,d'ARE YOU LEAVE?',255,255 ; quit ask message
but_YesNo
               dta 42,07,d'NO',255
               dta 62,07,d'YES',255
               dta 255
msg_Theme_Overwrite
               dta 25,d'OVERWRITE?',255
               dta 142,d'THIS ACTION WILL',255
               dta 163,d'OVERWRITE FILE',255
               dta 184,d'DEFAULTH.EME',255
               dta 202,d'ON THE DISKETTE!',255
               dta 255
msg_UnknownDefinition
               dta 221,d'UNKNOWN DEFINITION',255,255
msg_SFX_ValuePrompt
               dta 220,d'VAL:____   ',6,d'________',255,255
msg_TAB_JumpToPrompt
               dta 220,d'JUMP ROW',255
               dta 234,6,d'0____',255,255
msg_TAB_RepeatPrompt
               dta 220,d'REPEAT',255
               dta 235,6,d'1_63',255,255
msg_TAB_FreqPrompt
               dta 220,d'FREQ.DIV.',255
               dta 234,6,d'0_255',255,255
msg_TAB_SFXIdPrompt
               dta 220,d'SFX ID',255
               dta 235,6,d'0_63',255,255
msg_BeatStepPrompt
               dta 220,d'BEAT STEP',255
               dta 235,6,d'1_32',255,255
msg_SetTempoPrompt
               dta 220,d'TEMPO',255
               dta 234,6,d'1_255',255,255
msg_pianoTuneInfo
               dta 44,d'OCTAVE',255
               dta 54,d'012345',255
               dta 255
msg_newPrompt
               dta 23,d'ARE YOU SURE?',255,255

msg_ClipboardBadData
					dta 221,d'BAD CLIPBOARD DATA',255,255
msg_ClipboardEmpty
					dta 221,d'CLIPBOARD IS EMPTY',255,255
msg_ClipboardCopied
					dta 227,d'COPIED',255,255
msg_ClipboardPasted
					dta 227,d'PASTED',255,255

msg_mem_stats
					dta  61,d'RAW DATA',255
					dta  81,d'SFX',255
					dta 101,d'TAB',255
					dta 141,d'TOTAL',255
					dta 181,d'FREE',255
					dta 255

msg_SFX_FastKey_Assigned
               ;         01234567890123456789
					dta 220,d'FAST KEY _ > SFX# __',255,255
msg_SFX_FastKey_Cleared
					dta 220,d'FAST KEY _ - CLEARED',255,255
