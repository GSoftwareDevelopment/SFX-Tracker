; This file is a part of sfx_engine.pas
; will not work on its own unless you adapt it!
;
; Introduction: How to adapt?
;
; Two tables are required to be defined:
; - SFX_CHANNELS_ADDR - is an array of engine registers; 32 bytes required
; - NOTE_VAL - an array of note values (can be the one from RMT :P)
;
; The SFX_CHANNELS_ADDR arrays should be previously filled with the start values ($ff,$ff,$ff,$ff,$ff,$00,$00,$00,$00)
;
; And that's it. Hook it up to the VBLANK interrupt and go :D (I guess :P)
;

			icl 'sfx_engine-defs.asm'

         phr
.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
			dec $D301									; turn off ROM
.endif
.endif

tick_start
			ldx #$18										; set channel offset to last channel

; FETCHING CHANNEL & SFX DATA
; prepare SFX Engine registers
channel_set
         ldy SFX_CHANNELS_ADDR+_chnOfs,x  	; get SFX offset

check_offset
         cpy #SFX_OFF 								; check SFX offset
         bne fetch_SFX_data
         jmp next_channel 							; $ff=no SFX

fetch_SFX_data

         lda SFX_CHANNELS_ADDR+_chnMode,x 	; get SFX modulate type
         sta chnMode


; this feature is not used in SFXMM
; %1sfxsize - if bit 7th is set, rest of bits of modulate type, indicate size of definition
; %0000type - otherwise, MUST be set STOP SFX function in SFX definition!
.ifdef MAIN.@DEFINES.USE_MODULATORS
;.or MAIN.@DEFINES.USE_ALL_MODULATORS
			bpl continue_fetch
			and %01111111
			sta chnMode

			cpy chnMode									; check SFX length
			bne restore_offset_and_continue_fetch

restore_offset_and_go_to_next_channel
			ora %10000000
			sta chnMode
			jmp next_channel

restore_offset_and_continue_fetch
			ora %10000000
			sta chnMode
.else
			bmi check_SFX_length
			jmp next_channel

check_SFX_length
			and %01111111
			sta chnMode

			cpy chnMode									; check SFX length
			bne restore_offset_and_continue_fetch

restore_offset_and_go_to_next_channel
			ora %10000000
			sta chnMode
			jmp next_channel

restore_offset_and_continue_fetch
			ora %10000000
			sta chnMode
.endif

continue_fetch
         lda SFX_CHANNELS_ADDR+_sfxPtrLo,x	; get SFX pointer
         sta sfxPtr
         lda SFX_CHANNELS_ADDR+_sfxPtrHi,x
         sta sfxPtr+1

			lda SFX_CHANNELS_ADDR+_chnNote,x		; get SFX note
			sta chnNote

			lda SFX_CHANNELS_ADDR+_chnFreq,x		; get SFX frequency
			sta chnFreq

; $2ac9

.ifdef MAIN.@DEFINES.USE_MODULATORS
;.or MAIN.@DEFINES.USE_ALL_MODULATORS
;
; MODULATORS SECTIONS
; input: A register		modulation mode
; output: A register 	modulation value, on JMP change_freq, or
;								frequency value, on JMP setChannelFreq
;
; Y Register - can be changed, if need do jump in SFX range
; ATTENTION! The Engine does not check the jump ranges - IT CAN CRASH!
modulators
; get SFX modulation mode

			lda chnMode

.ifdef MAIN.@DEFINES.DFD_MOD
;.or MAIN.@DEFINES.USE_ALL_MODULATORS
			cmp #MODMODE_DFD							; check DFD Modulation mode
			bmi DFD_Mod
			jmp setPokey
DFD_Mod
			bne LFD_NLM_Mod
;
; DFD - Direct Frequency Divider
; first becouse, must be fast as possible
			lda (sfxPtr),y								; get MOD/VAL as frequency divider
			jmp setChannelFreq
.endif

.ifdef MAIN.@DEFINES.LFD_NLM_MOD
;.or MAIN.@DEFINES.USE_ALL_MODULATORS
LFD_NLM_Mod
			cmp #MODMODE_LFD_NVM						; check LFD/NLM
			bne check_MFD

			icl 'sfx_engine-LFD_NLM.asm'
.endif

.ifdef MAIN.@DEFINES.MFD
;.or MAIN.@DEFINES.USE_ALL_MODULATORS
check_MFD
			cmp #MODMODE_MFD							; check MFD
			bne check_HFD

			icl 'sfx_engine-MFD.asm'
.endif

.ifdef MAIN.@DEFINES.HFD
;.or MAIN.@DEFINES.USE_ALL_MODULATORS
check_HFD
			cmp #MODMODE_HFD							; check HFD mode
			bne modMode_notDefined

			icl 'sfx_engine-HFD.asm'
.endif

; modulate value in register A
change_freq
         clc
         adc SFX_CHANNELS_ADDR+_chnFreq,x

; current frequency in register A
setChannelFreq
         sta chnFreq

modMode_notDefined
         iny 											; shift SFX offset to distortion & volume definition

; this part is responsible for the modulator work mode.
; THIS FUNCTIONALITY IS NOT SUPPORTED BY SFXMM!
; When the 3rd bit of the modulation mode is set, the modulation is in relative mode.
; Otherwise, the mode is absolute.
         lda chnMode
         and #MODMODE_RELATIVE
         bne setPokey
         lda chnFreq
         sta SFX_CHANNELS_ADDR+_chnFreq,x
			lda chnNote
			sta SFX_CHANNELS_ADDR+_chnNote,x

.endif
;
; END OF MODULATOR SECTION
;

setPokey
			stx _regTemp
         txa											; transfer channel offset (X reg) to A reg
         lsr @											; divide channel offset by 4
         lsr @ 										; to calculate AUDIO offset
         tax											; set AUDIO offset in X register

; get current frequency
			lda chnFreq
         sta audf,x        						; store direct to POKEY register
; get current distortion & volume
         lda (sfxPtr),y    						; get SFX distortion & volume definition
         sta audc,x        						; store direct to POKEY register
         iny
			ldx _regTemp									; restore current channel offset

.ifdef MAIN.@DEFINES.SFX_previewChannels
         sta SFX_CHANNELS_ADDR+_chnCtrl,x 	; store SFX distortion & volume in channel register
.endif

next_SFX_Set
         tya											; tranfer current SFX offset to A register
         sta SFX_CHANNELS_ADDR+_chnOfs,x		; store SFX offset in channel register

next_channel
         txa         								; shift offset to next channel
			sec
			sbc #8
			bmi end_tick
         tax         								; no, go to fetching data
         jmp channel_set

end_tick
.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
			inc $D301									; turn on ROM
.endif
.endif
			plr
         jmp xitvbl
         rts
