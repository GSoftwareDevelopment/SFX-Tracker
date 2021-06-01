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

xitvbl      = $e462;
sysvbv      = $e45c;
audf        = $d200;
audc        = $d201;

; below variables is only for current channel
_sfxPtr     = 0
sfxPtr      = $f5      ; SFX Pointer (2 bytes)
_sfxPtrLo   = 0
_sfxPtrHi   = 1

_chnOfs     = 2
chnOfs      = $f7      ; SFX Offset in SFX definition

_chnMode    = 3
chnMode     = $f8      ; SFX Modulation Mode

_chnNote    = 4
chnNote     = $f9      ; SFX Note

_chnFreq    = 5
chnFreq     = $fa      ; SFX Frequency

_chnModVal  = 6
chnModVal   = $fb      ; SFX Modulator

_chnCtrl    = 7
chnCtrl     = $fc;     ; SFX Control (distortion & volume)

_regA       = $fd;
_regX       = $fe;
_regY       = $ff;

         phr
			dec $D301	; turn off ROM

tick_start
//         ldx #0  ; set channel offset to first channel
			ldx #$18		; set channel offset to last channel

; FETCHING CHANNEL & SFX DATA
; prepare SFX Engine registers

channel_set
         ldy SFX_CHANNELS_ADDR+_chnOfs,x  ; get SFX offset
         cpy #$ff ; check SFX offset
         bne fetch_SFX_data
         jmp next_channel ; $ff=no SFX
fetch_SFX_data
         sty chnOfs  ; propably, completly unnessesery

; get SFX pointer
         lda SFX_CHANNELS_ADDR+_sfxPtrLo,x
         sta sfxPtr
         lda SFX_CHANNELS_ADDR+_sfxPtrHi,x
         sta sfxPtr+1
; get SFX note
			lda SFX_CHANNELS_ADDR+_chnNote,x
			sta chnNote
; get SFX frequency
			lda SFX_CHANNELS_ADDR+_chnFreq,x
			sta chnFreq
; get SFX modulation mode
         lda SFX_CHANNELS_ADDR+_chnMode,x
         sta chnMode

			// $2ac9

         cmp #3
         bmi DFD_Mod
         jmp setPokey	; check DFD Modulation mode
DFD_Mod
         bne modulators
;
; DFD - Direct Frequency Divider
; first becouse, must be fast as possible
         lda (sfxPtr),y    ; get MOD/VAL=freq.divisor
         jmp setChannelFreq

;
; MODULATORS SECTIONS
; input: A register		modulation mode
; output: A register 	modulation value, on JMP change_freq, or
;								frequency value, on JMP setChannelFreq
;
; Y Register - can be changed, if need do jump in SFX range
; ATTENTION! The Engine does not check the jump ranges - IT CAN CRASH!
;
modulators
         cmp #2         ; check LFD/NLM
         bne check_MFD

			icl 'sfx_engine-LFD_NLM.asm'

check_MFD
         cmp #1         ; check MFD
         bne check_HFD

			icl 'sfx_engine-MFD.asm'

check_HFD
         cmp #0         ; check HFD mode
         bne modMode_notDefined

			icl 'sfx_engine-HFD.asm'


; modulate value in register A
change_freq
         clc
         adc SFX_CHANNELS_ADDR+_chnFreq,x

; current frequency in register A
setChannelFreq
         sta chnFreq

modMode_notDefined
;
; END OF MODULATOR SECTION
;

; this part is responsible for the modulator mode.
; THIS FUNCTIONALITY IS NOT DOCUMENTED AND NOT SUPPORTED BY SFXMM!
; When the 7th bit of the modulation mode is set, the modulation is in relative mode.
; Otherwise, the mode is absolute.
         lda chnMode
         and #$80
         bne setPokey
         lda chnFreq
         sta SFX_CHANNELS_ADDR+_chnFreq,x
			lda chnNote
			sta SFX_CHANNELS_ADDR+_chnNote,x

setPokey
			stx _regX
         txa	; transfer channel offset (X reg) to A reg
         lsr @	; divide channel offset by 4
         lsr @ ; to calculate AUDIO offset
         tax	; set AUDIO offset in X register

; get current frequency
			lda chnFreq
         sta audf,x        ; store direct to POKEY register
; get current distortion & volume
         iny               ; shift sfx offset to next byte of definition
         lda (sfxPtr),y    ; get SFX distortion & volume definition
         sta audc,x        ; store direct to POKEY register
         iny
			ldx _regX			; restore current channel offset
         sta SFX_CHANNELS_ADDR+_chnCtrl,x ; store SFX distortion & volume in channel register

next_SFX_Set
         tya	; tranfer current SFX offset to A register
         sta SFX_CHANNELS_ADDR+_chnOfs,x ; store SFX offset in channel register

next_channel
         txa         ; shift offset to next channel
//         clc
//         adc #8      ; 8 bytes per channel
//         cmp #$20    ; is it last channel?
//         beq end_tick ; yes, end tick

			sec
			sbc #8
			bmi end_tick
         tax         ; no, go to fetching data
         jmp channel_set

end_tick
			inc $D301	; turn on ROM

			plr
         jmp xitvbl
         rts
