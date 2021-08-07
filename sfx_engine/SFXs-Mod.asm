;
; MODULATORS SECTIONS
; input: A register     modulation mode
; output: A register    modulation value, on JMP change_freq, or
;                       frequency value, on JMP setChannelFreq
;
; Y Register - can be changed, if need do jump in SFX range
; ATTENTION! The Engine does not check the jump ranges - IT CAN CRASH!
modulators

			lda chnCtrl
			and #%00010000          ; the first bit of distortion set
			beq check_Mods          ; forces the DFD modulator to work

			lda chnCtrl
			and #%11101111
			sta chnCtrl

			lda (dataPtr),y
			sta chnFreq
			iny
			jmp setPokey

; get SFX modulation mode
; SFX Mod Modes:
;  0 - HFD - High Freq. Div.     - relative modulation of the frequency divider in the range of +/- 127
;                                - without the possibility of looping the SFX
;                                - Full backwards compatibility with the original SFX engine
;  1 - MFD - Middle Freq. Div.   - relative modulation of the frequency divider in the range of +/- 63
;                                - SFX looping possible
;  2 - LFD/NLM - Low Freq Div.   - note level modulation in relative range of +/- 32 half tones;
;                                - relative modulation of freq. divider in the range of +/- 32
;                                - SFX looping possible
;  3 - DSD - Direct Set Div.     - direct set of the frequency divider - without looping possible
;
check_Mods
         lda chnMode
         bpl check_DFD_Mod                   ; check 7 bit
         jmp setPokey                        ; is set means no modulator mode (not supported by SFXMM)
                                             ; The definition data does not contain MOD/VAL information.
                                             ; Only distortion and volume.
                                             ; Definition length is specified in SFX's MOD_MODE (bits 6 to 0)

check_DFD_Mod
.ifdef MAIN.@DEFINES.DFD_MOD // .or .def MAIN.@DEFINES.USE_ALL_MODULATORS
         cmp #MODMODE_DFD                    ; check DFD Modulation mode
         bne check_LFD_NLM_Mod

;
; DFD - Direct Frequency Divider
force_DFD_Mod
         lda (dataPtr),y                      ; get MOD/VAL as frequency divider
         jmp setChannelFreq

         ; In this modulation mode, given that the frequency is placed in the MOD/VAL value,
         ; there is no concept of note or SFX frequency.
         ; This mode is useful for sound synthesis.
         ; The end of the definition cannot be marked, hence SFX in this mode should have
         ; a full length of 128 bytes.
         ; The definition is looped!
.endif

         ; !!! Note on LFD_NLM, MFD and HFD modes !!!
         ;
         ; It is important that the definition has an ending placed in MOD/VAL,
         ; and its value has the 7th bit set.
         ;
         ; EXTREME CAUTION should be exercised when jumping as it is not controlled.
         ; This can lead to undesired behavior or even hanging of the computer!

check_LFD_NLM_Mod
.ifdef MAIN.@DEFINES.LFD_NLM_MOD // .or .def MAIN.@DEFINES.USE_ALL_MODULATORS
         cmp #MODMODE_LFD_NVM                ; check LFD/NLM
         bne check_MFD

         icl 'SFXs-Mod-LFD_NLM.asm'

         ; This is the most extensive modulation mode, allowing you to change
         ; the frequency and the note in range +/-31. Allows to loop SFX.
.endif

check_MFD
.ifdef MAIN.@DEFINES.MFD_MOD //  .or MAIN.@DEFINES.USE_ALL_MODULATORS
         cmp #MODMODE_MFD                    ; check MFD
         bne check_HFD

         icl 'SFXs-Mod-MFD.asm'
         ; Mode with frequency modulation capability of +/-63.
         ; Allows for looping.
.endif

check_HFD
.ifdef MAIN.@DEFINES.HFD_MOD // .or MAIN.@DEFINES.USE_ALL_MODULATORS
         cmp #MODMODE_HFD                    ; check HFD mode
         bne modMode_notDefined

         icl 'SFXs-Mod-HFD.asm'
         ; Mode with frequency modulation capability of +/-127.
         ; Allows for looping.
.endif

; modulate value in register A
change_freq
         clc
         adc SFX_CHANNELS_ADDR+_chnFreq,x

; current frequency in register A
setChannelFreq
         sta chnFreq

modMode_notDefined
         iny                                 ; shift SFX offset to distortion & volume definition

; this part is responsible for the modulator work mode.
; THIS FUNCTIONALITY IS NOT SUPPORTED BY SFXMM!
; When the 3rd bit of the modulation mode is set, the modulation is in relative mode.
; Otherwise, the mode is absolute.
         lda chnMode
         and #MODMODE_RELATIVE
         bne setPokey

; RELATIVE MODE
; The sound frequency/note is changed with reference to its previous value.
         lda chnFreq
         sta SFX_CHANNELS_ADDR+_chnFreq,x
         lda chnNote
         sta SFX_CHANNELS_ADDR+_chnNote,x

; ABSOLUTE MODE
; The frequency/note is always changed with reference to the SFX initiating value.
