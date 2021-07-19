	icl 'definitions.asm'

SFX_ENGINE
	jmp SFX_Init			; initialize SFX Engine
	jmp SFX_PlayNote		; Play SFX on a preset note
	jmp SFX_PlayFreq		; Play SFX on a preset frequency
	jmp SFX_ChannelOff	; Stop playing on preset channel
	jmp SFX_Off				; Stop playing on all channels
	jmp SFX_PlayTab		; Play TAB on a preset channel
	jmp SFX_PlaySONG		; Play SONG

SFX_Init			icl 'lib/sfx_init.asm'
SFX_PlayNote	icl 'lib/sfx_playnote.asm'		; x - channel; y - sfx index; a - note
SFX_PlayFreq	icl 'lib/sfx_playfreq.asm' 	; x - channel; y - sfx index; a - frequency
SFX_ChannelOff	icl 'lib/sfx_chaneloff.asm'	; x - channel;
SFX_Off			icl 'lib/sfx_off.asm'			;
SFX_SetTAB		icl 'lib/sfx_settab.asm'		; x - channel; y - tab index
SFX_PlayTAB		icl 'lib/sfx_playtab.asm'		; x - channel; y - tab index
SFX_PlaySONG	icl 'lib/sfx_playsong.asm'		; y - SONG row
