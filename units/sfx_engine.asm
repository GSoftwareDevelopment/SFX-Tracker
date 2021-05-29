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

tick_start 		; $2ba6
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

;
; Low Frequency Divider Modulator/Note Value Modulator
LFD_NLM_mode            ; code for LFD_NLM
         lda (sfxPtr),y ; get modulate value
         sta chnModVal  ; store in loop register
         bne decode_LFD_NLM   ; check modulation value
         jmp getChannelFreq   ; if =0, means no modulation
decode_LFD_NLM
         bmi LFD_NLM_JumpTO   ; jump to position in SFX definition, if 7th bit is set
         and #%01000000 ; check 6th bit
         bne LFD_NLM_note_mod
; frequency modulation
         lda chnModVal
         cmp #32        ; VAL<32 means positive value, otherwise negative
         bmi LFD_NLM_inc_freq
         ora #%11100000 ; set 7th-5th bit to get oposite value
LFD_NLM_inc_freq
         clc
         adc SFX_CHANNELS_ADDR+_chnFreq,x
         sta chnFreq
         jmp setPokey   ; return frequency in register A

; note modulation
LFD_NLM_note_mod
         lda chnModVal
         cmp #32        ; VAL<32 means positive value, otherwise negative
         bmi LFD_NLM_inc_note
         ora #%11100000 ; set 7th-5th bit to get oposite value
LFD_NLM_inc_note
         clc
         adc SFX_CHANNELS_ADDR+_chnNote,x
         sta chnNote

; get frequency representation of note
         sty _regY
         tay
         lda adr.note_val,y
         sta chnFreq
         ldy _regY
         jmp setPokey

; Jump to
LFD_NLM_JumpTo
         and #%01111111 ; clear 7th bit
         bne LFD_NLM_setSFXofs
         ldy #$ff ; end of SFX definition
         jmp next_SFX_Set
LFD_NLM_setSFXofs
         tay ; set value to SFX offset register
         jmp LFD_NLM_mode  ; one more iteration


;
; MFD - Medium Frequency Divider Modulator
check_MFD
         cmp #1         ; check MFD
         bne check_HFD

MFD_mode                ; code for MFD
         lda (sfxPtr),y ; get modulate value
         sta chnModVal
         bne decode_MFD ; check modulation
         jmp getChannelFreq   ; if 0, means no modulation

decode_MFD
         bmi MFD_JumpTO ; jump to position in SFX definition, if 7th bit is set
         cmp #64        ; VAL<64 means positive value, otherwise negative
         bmi MFD_incFreq  ; VAL is positive
         ora #%11000000 ; set 7th & 6th bit; VAL is negative
MFD_incFreq
         clc
         adc SFX_CHANNELS_ADDR+_chnFreq,x
         sta chnFreq
         jmp setPokey   ; return frequency in register A

; Jump To
MFD_JumpTo
         and #%01111111 ; clear 7th bit
         bne MFD_setSFXofs
         ldy #$ff ; end of SFX definition
         jmp next_SFX_Set
MFD_setSFXofs
         tay ; set value to SFX offset register
         jmp MFD_mode   ; one more iteration

;
; HFD - High Frequency Divider Modulator
; only for compatibility with the original SFX engine
check_HFD
         cmp #0         ; check HFD mode
         bne getChannelFreq

HFD_MODE                ; code for HFD
         lda (sfxPtr),y ; get modulate value
         sta chnModVal
         bne decode_HFD ; check modulation
         jmp getChannelFreq   ; if 0, means no modulation
decode_HFD
			cmp #$80
			bne HFD_modulate

HFD_SFXEND
         ldy #$ff 		; end of SFX definition
         jmp next_SFX_Set

HFD_modulate
         clc
         adc SFX_CHANNELS_ADDR+_chnFreq,x
         jmp setPokey

;
; end modulator section
;


; current frequency in register A
getChannelFreq
         lda SFX_CHANNELS_ADDR+_chnFreq,x ; get current channel frequency
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
