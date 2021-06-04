messages
msg_IO_DirPrompt
					dta 200,d'DEVICE:PATH',255,255			; IO->DIR prompt message
msg_IO_SavePrompt
					dta 200,d'SAVE AS:',255,255
msg_IO_LoadPrompt
					dta 200,d'LOAD:',255,255
msg_IO_noFiles
					dta 126,d'NO FILES',255,255				; IO->DIR message for empty disk
msg_IO_error
					dta 223,d'I/O ERROR #',255,255			; IO error message with it number
msg_IO_reading
					dta 20,d'READING...',255,255			; IO message for reading operation
msg_IO_writing
					dta 20,d'WRITING...',255,255			; IO message for writing operation
msg_IO_skipping
					dta 20,d'SKIPPING...',255,255			; IO message for skipping - ony IO->DIR when page is changing
msg_IO_Quit
					dta 42,07,d'ARE YOU LEAVE?',06,255,255	; quit ask message
but_IO_Quit
					dta 62,07,d'YES',255
					dta 82,07,d'NO',255
					dta 255
msg_Theme_Overwrite
					dta 44,07,d'OVERWRITE?',06,255
					dta 162,d'THIS ACTION WILL',255
					dta 183,d'OVERWRITE FILE',255
					dta 202,d'ON THE DISKETTE!',255
					dta 255
but_Theme_Overwrite
					dta 64,07,d'YES',255
					dta 84,07,d'NO',255
					dta 255
msg_SFX_ValuePrompt
					dta 220,d'VAL:____   ',6,d'________',255,255
msg_TAB_JumpToPrompt
					;     01234567890123456789
					dta 224,d'JUMP ROW  ',6,d'0____',255,255
msg_TAB_RepeatPrompt
					dta 224,d'REPEAT     ',6,d'1_63',255,255
msg_TAB_FreqPrompt
					dta 224,d'FREQ.DIV. ',6,d'0_255',255,255
msg_TAB_SFXIdPrompt
					dta 224,d'SFX ID     ',6,d'0_63',255,255
