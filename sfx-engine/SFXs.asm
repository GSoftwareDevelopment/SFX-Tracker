; FETCHING CHANNEL & SFX DATA
; prepare SFX Engine registers
         ldy SFX_CHANNELS_ADDR+_chnOfs,x     ; get SFX offset

check_offset
         cpy #SFX_OFF                        ; check SFX offset
         bne fetch_SFX_data
         jmp next_channel                    ; $ff=no SFX

fetch_SFX_data

         lda SFX_CHANNELS_ADDR+_chnMode,x    ; get SFX modulate type
         sta chnMode


; this feature is not used in SFXMM
; %1sfxsize - if bit 7th is set, rest of bits of modulate type, indicate size of definition
; %0000type - otherwise, MUST be set STOP SFX function in SFX definition!
.ifdef MAIN.@DEFINES.USE_MODULATORS
;.or MAIN.@DEFINES.USE_ALL_MODULATORS
         bpl continue_fetch
         and %01111111
         sta chnMode

         cpy chnMode                         ; check SFX length
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

         cpy chnMode                         ; check SFX length
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
         lda SFX_CHANNELS_ADDR+_sfxPtrLo,x   ; get SFX pointer
         sta sfxPtr
         lda SFX_CHANNELS_ADDR+_sfxPtrHi,x
         sta sfxPtr+1

         lda SFX_CHANNELS_ADDR+_chnNote,x    ; get SFX note
         sta chnNote

         lda SFX_CHANNELS_ADDR+_chnFreq,x    ; get SFX frequency
         sta chnFreq

.ifdef MAIN.@DEFINES.USE_MODULATORS
;.or MAIN.@DEFINES.USE_ALL_MODULATORS

			icl 'SFXs-Mod.asm'

.endif

setPokey
         stx _regTemp
         txa                                 ; transfer channel offset (X reg) to A reg
         lsr @                               ; divide channel offset by 4
         lsr @                               ; to calculate AUDIO offset
         tax                                 ; set AUDIO offset in X register

; get current frequency
         lda chnFreq
         sta audf,x                          ; store direct to POKEY register
; get current distortion & volume
         lda (sfxPtr),y                      ; get SFX distortion & volume definition
         sta audc,x                          ; store direct to POKEY register
         ldx _regTemp								; restore current channel offset

.ifdef MAIN.@DEFINES.SFX_previewChannels
         sta SFX_CHANNELS_ADDR+_chnCtrl,x    ; store SFX distortion & volume in channel register
.endif

next_SFX_Set
         iny											; increase SFX offset
SFX_Set_Offset
         tya                                 ; tranfer current SFX offset to A register
         sta SFX_CHANNELS_ADDR+_chnOfs,x     ; store SFX offset in channel register
