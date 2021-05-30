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

tick_start ; $2bb8
         ldx #0  ; set channel offset to first channel

; fetching channel & sfx data
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
; get SFX modulation mode
         lda SFX_CHANNELS_ADDR+_chnMode,x
         sta chnMode

         cmp #3
         bmi DFD_Mod
         jmp next_channel	; check DFD Modulation mode
DFD_Mod
         bne modulators
;
; DFD - Direct Frequency Divider
; first becouse, must be fast as possible
         lda (sfxPtr),y    ; get MOD/VAL=freq.divisor
         jmp setPokey

;
; modulators section
; input: A register = modulation mode
; output: A register = frequency value
;
modulators
         cmp #2         ; check LFD/NLM
         bne check_MFD

			icl 'sfx_engine-LFD_NLM.asm'
			icl 'sfx_engine-MFD.asm'
			icl 'sfx_engine-HFD.asm'

;
; end modulator section
;


; current frequency in register A
getChannelFreq
         lda SFX_CHANNELS_ADDR+_chnFreq,x ; get current channel frequency
setChannelFreq
         sta chnFreq
setPokey
			stx _regX
         txa	; transfer channel offset (X reg) to A reg
         lsr @	; divide channel offset by 4
         lsr @ ; to get audf offset
         tax   ; transfer A reg to channel offset (X reg)

			lda chnFreq
         sta audf,x        ; store direct to POKEY register
; get distortion & volume
         iny               ; shift sfx offset to next byte of definition
         lda (sfxPtr),y    ; get SFX distortion & volume definition
         sta audc,x        ; store direct to POKEY register
         iny
			ldx _regX

next_SFX_Set
         tya   ; tranfer current SFX offset to A register
         sta SFX_CHANNELS_ADDR+_chnOfs,x ; store SFX offset in channel register

next_channel
         txa         ; shift offset to next channel
         clc
         adc #8      ; 8 bytes per channel
         cmp #$20    ; is it last channel?
         beq endtick ; yes, end tick

         tax         ; no, go to fetching data
         jmp channel_set

endtick	inc $D301	; turn on ROM

			plr
         jmp xitvbl
         rts
