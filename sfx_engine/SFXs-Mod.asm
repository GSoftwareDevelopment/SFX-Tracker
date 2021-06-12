;
; MODULATORS SECTIONS
; input: A register     modulation mode
; output: A register    modulation value, on JMP change_freq, or
;                       frequency value, on JMP setChannelFreq
;
; Y Register - can be changed, if need do jump in SFX range
; ATTENTION! The Engine does not check the jump ranges - IT CAN CRASH!
modulators
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

         lda chnMode

.ifdef MAIN.@DEFINES.DFD_MOD // .or .def MAIN.@DEFINES.USE_ALL_MODULATORS
         cmp #MODMODE_DFD                    ; check DFD Modulation mode
         bmi DFD_Mod
         jmp setPokey
DFD_Mod
         bne LFD_NLM_Mod
;
; DFD - Direct Frequency Divider
; first becouse, must be fast as possible
         lda (sfxPtr),y                      ; get MOD/VAL as frequency divider
         jmp setChannelFreq
.endif

.ifdef MAIN.@DEFINES.LFD_NLM_MOD // .or .def MAIN.@DEFINES.USE_ALL_MODULATORS
LFD_NLM_Mod
         cmp #MODMODE_LFD_NVM                ; check LFD/NLM
         bne check_MFD

         icl 'SFXs-Mod-LFD_NLM.asm'
.endif

.ifdef MAIN.@DEFINES.MFD_MOD //  .or MAIN.@DEFINES.USE_ALL_MODULATORS
check_MFD
         cmp #MODMODE_MFD                    ; check MFD
         bne check_HFD

         icl 'SFXs-Mod-MFD.asm'
.endif

.ifdef MAIN.@DEFINES.HFD_MOD // .or MAIN.@DEFINES.USE_ALL_MODULATORS
check_HFD
         cmp #MODMODE_HFD                    ; check HFD mode
         bne modMode_notDefined

         icl 'SFXs-Mod-HFD.asm'
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
         lda chnFreq
         sta SFX_CHANNELS_ADDR+_chnFreq,x
         lda chnNote
         sta SFX_CHANNELS_ADDR+_chnNote,x
