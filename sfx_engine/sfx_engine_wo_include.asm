; This file is a part of sfx_engine.pas
; will not work on its own unless you adapt it!
;

; ---------------------------------------------------------------------------
; SYSTEM SUBROUTINES FOR INTERRUPT SUPPORT & POKEY REGISTERS
; ---------------------------------------------------------------------------

xitvbl      = $e462
sysvbv      = $e45c

audf        = $d200	; POKEY audio frequency divider control
audc        = $d201	; POKEY autio control for distortion & volume

; ---------------------------------------------------------------------------
; CONSTANTS - offsets in channels registers table
; ---------------------------------------------------------------------------

_sfxPtr        = 0;     // pointer to SFX data definition
_sfxPtrLo      = 0;     //
_sfxPtrHi      = 1;     //
_sfxNoteTabOfs = 2;     //


_chnOfs        = 3;     // SFX offset
_chnNote       = 4;     // SFX Note
_chnFreq       = 5;     // SFX Frequency

_chnMode       = 6;     // SFX Modulator mode

_chnModVal     = 7;     // SFX current Modulator value
_chnCtrl       = 8;     // SFX current distortion & volume

_trackOfs      = 10;    // current Track offest

_tabPtr        = 12;    // pointer to TAB data definition
_tabPtrLo      = 12;    //
_tabPtrHi      = 13;    //
_tabOfs        = 14;    // TAB row offset
_tabRep        = 15;    // TAB current repeat value

; ---------------------------------------------------------------------------
; VARIABLES - PAGE ZERO
; ---------------------------------------------------------------------------

.ifdef MAIN.@DEFINES.SFX_SYNCAUDIOOUT
AUDIOBUF          = $E8      ; 8 bytes audio buffer for sync output
.endif

SONG_TEMPO        = $F0      ; SONG Tempo
SONG_TICK_COUNTER = $F1      ; SONG tick counter
SONG_Ofs				= $F2		  ;
SONG_Rep				= $F3		  ;

dataPtr				= $F4		  ; SFX or TAB data pointer (2 bytes)

TABOrder          = $F7      ; TAB Order
TABParam          = $F8      ; TAB Parameter (Note/Freq/Position)

sfxNoteOfs        = $F6      ; SFX Note Table offset (1 byte)
chnNote           = $F7      ; SFX Note
chnFreq           = $F8      ; SFX Frequency

chnMode           = $F9      ; SFX Modulation Mode
chnModVal         = $FA      ; SFX Modulator
chnCtrl           = $FB      ; SFX Control (distortion & volume)

_regTemp          = $FC

; ---------------------------------------------------------------------------
; CONSTANTS
; ---------------------------------------------------------------------------

SFX_OFF           = $FF
TAB_OFF           = $FF

FN_NOTE_FREQ      = $40

MODFN_SFX_STOP    = $80
MODFN_LFD_FREQ    = $20
MODFN_NLM_NOTE    = $40
MODFN_MFD_FREQ    = $40

MODMODE_DFD       = 3
MODMODE_LFD_NVM   = 2
MODMODE_MFD       = 1
MODMODE_HFD       = 0
MODMODE_RELATIVE  = %1000

trkBlank				= %01000000 // blank (No operation)
trkOff				= %01111111
trkOrd_SetTempo   = %10000000
trkOrd_JumpTo     = %10000010
trkOrd_Repeat     = %10000100
trkOrd_EndSong    = %11111111

; ---------------------------------------------------------------------------
; BEGIN OF INTERRUPT ROUTINE
; ---------------------------------------------------------------------------

         phr
.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
         dec $D301                           ; turn off ROM
.endif
.endif

.ifdef MAIN.@DEFINES.SFX_PLAYBACK

; ---------------------------------------------------------------------------
; SONG CLOCK CONTROL
; ---------------------------------------------------------------------------

			lda SONG_TICK_COUNTER
			bmi dont_reset_tick_counter				; if SONG_TICK>=128 then don't countdown
         dec SONG_TICK_COUNTER
         bpl dont_reset_tick_counter
; reset tick counter
         lda SONG_TEMPO                         ; otherwise, set SONG_TICK as SONG_TEMPO
         sta SONG_TICK_COUNTER
.endif

dont_reset_tick_counter

; ---------------------------------------------------------------------------
; MAIN INTERRUPT LOOP
; ---------------------------------------------------------------------------

tick_start
         ldx #$30                           ; set channel offset to last channel

channel_set

.ifdef MAIN.@DEFINES.SFX_PLAYBACK

         lda SONG_TICK_COUNTER
         beq TAB_set
         jmp end_player_tick

; ---------------------------------------------------------------------------
; TAB PLAY ROUTINE
; ---------------------------------------------------------------------------

TAB_set
         ldy SFX_CHANNELS_ADDR+_tabOfs,x        ; get TAB offset

check_TAB_offset
         cpy #TAB_OFF                           ; check TAB offset
         bne fetch_TAB_pointer
         jmp end_player_tick                    ; $ff = no Tab; end Play routine for this channel

fetch_TAB_pointer
         lda SFX_CHANNELS_ADDR+_tabPtrLo,x      ; get TAB pointer
         sta dataPtr
         lda SFX_CHANNELS_ADDR+_tabPtrHi,x
         sta dataPtr+1

fetch_TAB_row
         lda (dataPtr),y                         ; get current TAB Note
         sta TABParam
         iny                                    ; shift TAB Offset to order value
         lda (dataPtr),y                         ; get current TAB Order
         sta TABOrder

; Current TAB Order check
         bmi check_TAB_Function                       ; check for function (7th bit indicates the function)

; ---------------------------------------------------------------------------
; TAB PLAY SECTION
; ---------------------------------------------------------------------------

; TAB - PLAY NOTE
         sty _regTemp                           ; store current TAB offset

; get SFX settings
         and #%00111111                         ; extract SFX Id from Order
         tay												; transfer SFX Id to Y register
         lda SFX_MODE_SET_ADDR,y                ; get modulator mode for SFX
         sta SFX_CHANNELS_ADDR+_chnMode,x       ; and store it in current channel register

         lda SFX_NOTE_SET_ADDR,y         	      ; get SFX note table preset
         sta SFX_CHANNELS_ADDR+_sfxNoteTabOfs,x ; store in channels registers
			sta self_TABnoteAddr+1						; change the lower byte of the command address

         tya
         asl @                                  ; multiply SFX Id by 2 to get offset in SFXPtr offset table
         tay


         lda SFX_TABLE_ADDR,y							; get SFX pointer from SFXPtr table and
         sta SFX_CHANNELS_ADDR+_SFXPtrLo,x		; set in current channel
         lda SFX_TABLE_ADDR+1,y
         sta SFX_CHANNELS_ADDR+_SFXPtrHi,x

         lda #$00                               ; reset current SFX offset to the beginig of definition
         sta SFX_CHANNELS_ADDR+_chnOfs,x

; get Note frequency divider from NOTE_TABLE

         lda TABOrder                           ; get SFX note type from TAB order
         cmp #FN_NOTE_FREQ                      ; check type of note (Note numer or Frequency Divider Set)
         bpl TAB_FN_Freq                        ; <64 its Note Set

TAB_FN_Note
         ldy TABParam

.ifdef MAIN.@DEFINES.SFX_previewChannels
         tya
         sta SFX_CHANNELS_ADDR+_chnNote,x
.endif

self_TABnoteAddr
			lda NOTE_TABLE_ADDR,y         			; get note frequency value from SFX Note Table
         jmp TAB_FN_setFreq

TAB_FN_Freq
         lda TABParam										; get note frequency from TAB row
TAB_FN_setFreq
         sta SFX_CHANNELS_ADDR+_chnFreq,x			; store frequency in current channels register
         ldy _regTemp                           ; restore current TAB Offset

         jmp next_player_tick

; ---------------------------------------------------------------------------
; TAB FUNCTIONS SECTION
; ---------------------------------------------------------------------------

check_TAB_Function
; current order is in A register
         cmp #$FF											; check TABEND function
         bne check_TABFn_REPEAT

TABFn_TAB_END

			lda SONG_Ofs    							; get current SONG offset
			cmp #$FF										; If the offset points to a value other than $FF
			beq play_TAB_again						;

			jmp SONG_process							; ...it means that SONG is played.

play_TAB_again
			ldy #0										; set TAB offset to begin
			jmp fetch_TAB_row

check_TABFn_REPEAT
         cmp #$C0                               ; check REPEAT function
         beq TAB_FN_Blank_NoteOff

TAB_FN_Loop
         and #%00111111                         ; extract value from order (repeat times)
         beq fetch_next_tab_row                 ; =0 REPEAT zero times? :| Can't possible
																; originally, it was a conditional jump `BNE TAB_FN_JumpTo`,
																; which means, the TAB function JUMP TO
TAB_FN_Repeat
         sta _regTemp                           ; temporary store value from order (repeat value)

; check current loop
         lda SFX_CHANNELS_ADDR+_tabRep,x        ; get current repeat value
         beq TAB_FN_RepeatSet                   ; =0 set loop
TAB_FN_ContinueLoop
         dec SFX_CHANNELS_ADDR+_tabRep,x        ; decrase current repeat value
         bne TAB_FN_JumpTo                      ; if current repeat value <>0 jump to position...
         iny                                    ; increment current TAB offset to next row

; end of loop

         bne fetch_next_tab_row
         jmp SONG_process                      ; if TAB offset is wrap, process TRACK step

fetch_next_tab_row
         jmp fetch_TAB_ROW                      ; fetch next TAB row

TAB_FN_RepeatSet
         lda _regTemp
         sta SFX_CHANNELS_ADDR+_tabRep,x

TAB_FN_JumpTo
         lda TABParam                            ; set jump position
         asl @                                  ; multiply by 2 to get TAB offest
         tay                                    ; store in TAB offset register
         jmp fetch_TAB_row                      ; get new TAB line

TAB_FN_Blank_NoteOff
         lda TABParam                            ; get row value
         bpl TAB_FN_Blank                       ; check BLANK (positive value) or NOTE OFF (negative)

TAB_FN_NoteOff
         lda #SFX_OFF                           ; turn off current SFX
         sta SFX_CHANNELS_ADDR+_chnOfs,x

         lda #$00
         sta chnCtrl
.ifdef MAIN.@DEFINES.SFX_previewChannels
         sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

			jsr turn_off_Audio_channel
         jmp next_player_tick

TAB_FN_Blank
; This function does nothing with registers

;
;
;

next_player_tick
         iny
         bne store_TAB_offset                   ; if TAB offset is wrap?
			jmp TABFn_TAB_END								; process TRACK step

store_TAB_offset
         tya
         sta SFX_CHANNELS_ADDR+_tabOfs,x        ; store current TAB offset in Channels register

end_player_tick
.endif

; ---------------------------------------------------------------------------
; SFX PLAY SECTION
; ---------------------------------------------------------------------------

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


; ---------------------------------------------------------------------------
; this feature is not used in SFXMM
; %1sfxsize - if bit 7th is set, rest of bits of modulate type, indicate size of definition
; %0000type - otherwise, MUST be set STOP SFX function in SFX definition!

.ifdef MAIN.@DEFINES.USE_MODULATORS // .or .def MAIN.@DEFINES.USE_ALL_MODULATORS
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
         sta dataPtr
         lda SFX_CHANNELS_ADDR+_sfxPtrHi,x
         sta dataPtr+1

         lda SFX_CHANNELS_ADDR+_chnNote,x    ; get SFX note
         sta chnNote

         lda SFX_CHANNELS_ADDR+_chnFreq,x    ; get SFX frequency
         sta chnFreq

.ifdef MAIN.@DEFINES.USE_MODULATORS // .or .def MAIN.@DEFINES.USE_ALL_MODULATORS

; ---------------------------------------------------------------------------
; MODULATORS SECTIONS
; ---------------------------------------------------------------------------
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
			bpl check_DFD_Mod							; check 7 bit
			jmp setPokey								; is set means no modulator mode (not supported by SFXMM)
															; The definition data does not contain MOD/VAL information.
															; Only distortion and volume.
															; Definition length is specified in SFX's MOD_MODE (bits 6 to 0)

check_DFD_Mod
.ifdef MAIN.@DEFINES.DFD_MOD // .or .def MAIN.@DEFINES.USE_ALL_MODULATORS
         cmp #MODMODE_DFD                    ; check DFD Modulation mode
         bne check_LFD_NLM_Mod

;
; DFD - Direct Frequency Divider
         lda (dataPtr),y                      ; get MOD/VAL as frequency divider
         jmp setChannelFreq

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

; ---------------------------------------------------------------------------
; Low Frequency Divider Modulator/Note Value Modulator
; ---------------------------------------------------------------------------
; This is the most extensive modulation mode, allowing you to change
; the frequency and the note in range +/-31. Allows to loop SFX.

LFD_NLM_mode
         lda (dataPtr),y          ; get modulate value
         sta chnModVal           ; store in loop register
         bne decode_LFD_NLM      ; check modulation value
         jmp modMode_notDefined  ; if =0, means no modulation
decode_LFD_NLM
         bmi LFD_NLM_JumpTo      ; jump to position in SFX definition, if 7th bit is set

         cmp #MODFN_NLM_NOTE
         bpl LFD_NLM_note_mod

; frequency modulation

         cmp #$20     ; VAL<32 means positive value, otherwise negative
         bmi LFD_NLM_inc_freq

         ora #%11100000          ; set 7th-5th bit to get oposite value
LFD_NLM_inc_freq
         jmp change_freq         ; return frequency in register A

; note modulation
LFD_NLM_note_mod
         and #%00111111
         cmp #32                 ; VAL<32 means positive value, otherwise negative
         bmi LFD_NLM_inc_note

         ora #%11100000          ; set 7th-5th bit to get oposite value
LFD_NLM_inc_note
         clc
         adc chnNote
         sta chnNote

; get frequency representation of note
         sty _regTemp
         tay

         lda SFX_CHANNELS_ADDR+_sfxNoteTabOfs,x  ; get SFX note table preset
         sta self_SFXnoteAddr+1

self_SFXnoteAddr
         lda NOTE_TABLE_ADDR,y
         ldy _regTemp
         jmp setChannelFreq

; Jump to
LFD_NLM_JumpTo
         and #%01111111          ; clear 7th bit
         bne LFD_NLM_setSFXofs
         ldy #SFX_OFF            ; end of SFX definition
         jmp SFX_Set_Offset
LFD_NLM_setSFXofs
         asl @
         tay                     ; set value to SFX offset register
         jmp LFD_NLM_mode        ; one more iteration

.endif

check_MFD
.ifdef MAIN.@DEFINES.MFD_MOD //  .or MAIN.@DEFINES.USE_ALL_MODULATORS
         cmp #MODMODE_MFD                    ; check MFD
         bne check_HFD

; ---------------------------------------------------------------------------
; MFD - Medium Frequency Divider Modulator
; ---------------------------------------------------------------------------
; Mode with frequency modulation capability of +/-63.
; Allows for looping.

MFD_mode
         lda (dataPtr),y          ; get modulate value
         sta chnModVal
         bne decode_MFD          ; check modulation
         jmp modMode_notDefined  ; if 0, means no modulation

decode_MFD
         bmi MFD_JumpTO          ; jump to position in SFX definition, if 7th bit is set

         cmp #MODFN_MFD_FREQ     ; VAL<64 means positive value, otherwise negative
         bmi MFD_incFreq         ; VAL is positive

         ora #%11000000          ; set 7th & 6th bit; VAL is negative
MFD_incFreq
         jmp change_freq         ; return frequency in register A

; Jump To
MFD_JumpTo
         and #%01111111          ; clear 7th bit
         bne MFD_setSFXofs
         ldy #SFX_OFF            ; end of SFX definition
         jmp SFX_Set_Offset
MFD_setSFXofs
         asl @
         tay                     ; set value to SFX offset register
         jmp MFD_mode            ; one more iteration

.endif

check_HFD
.ifdef MAIN.@DEFINES.HFD_MOD // .or MAIN.@DEFINES.USE_ALL_MODULATORS
         cmp #MODMODE_HFD                    ; check HFD mode
         bne modMode_notDefined

; ---------------------------------------------------------------------------
; HFD - High Frequency Divider Modulator
; ---------------------------------------------------------------------------
; only for compatibility with the original SFX engine
; Mode with frequency modulation capability of +/-127.
; Allows for looping.

HFD_MODE
         lda (dataPtr),y          ; get modulate value
         sta chnModVal
         bne decode_HFD          ; check modulation
         jmp modMode_notDefined  ; if 0, means no modulation
decode_HFD
         cmp #MODFN_SFX_STOP
         beq HFD_SFXEnd

         jmp change_freq

HFD_SFXEnd
         ldy #SFX_OFF            ; end of SFX definition
         jmp SFX_Set_Offset
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

; ---------------------------------------------------------------------------
; this part is responsible for the modulator work mode.
; ---------------------------------------------------------------------------
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

;
;
;

.endif

setPokey
         stx _regTemp
         txa                                 ; transfer channel offset (X reg) to A reg
         lsr @                               ; divide channel offset by 8
         lsr @                               ; to calculate AUDIO offset
         lsr @
         tax                                 ; set AUDIO offset in X register

.ifdef MAIN.@DEFINES.SFX_SYNCAUDIOOUT
         lda chnFreq
         sta AUDIOBUF,x
         lda (dataPtr),y
         sta AUDIOBUF+1,x
.else
; get current frequency
         lda chnFreq
         sta audf,x                          ; store direct to POKEY register
; get current distortion & volume
         lda (dataPtr),y                     ; get SFX distortion & volume definition
         sta audc,x                          ; store direct to POKEY register
.endif

         ldx _regTemp                        ; restore current channel offset

.ifdef MAIN.@DEFINES.SFX_previewChannels
         sta SFX_CHANNELS_ADDR+_chnCtrl,x    ; store SFX distortion & volume in channel register
.endif

next_SFX_Set
         iny                                 ; increase SFX offset
			bne SFX_Set_Offset						; check if SFX is wrap
			ldy #SFX_OFF								; set SFX to not play

SFX_Set_Offset
         tya                                 ; tranfer current SFX offset to A register
         sta SFX_CHANNELS_ADDR+_chnOfs,x     ; store SFX offset in channel register

;
;
;

next_channel
         txa                                 ; shift offset to next channel
         sec
         sbc #$10
         bmi end_tick                        ; is it last channel?
         tax                                 ; no, go fetching next channel data
         jmp channel_set
end_tick

.ifdef MAIN.@DEFINES.SFX_SYNCAUDIOOUT
; move audio buffer data, direct to audio registers
         ldx #7
audio_loop
         lda AUDIOBUF,x
         sta audf,x
         dex
         bpl audio_loop
.endif

.ifdef MAIN.@DEFINES.SFX_SWITCH_ROM
.ifdef MAIN.@DEFINES.ROMOFF
         inc $D301                           ; turn on ROM
.endif
.endif
         plr
         jmp xitvbl
         rts

; ---------------------------------------------------------------------------
; end of interrupt routine
; ---------------------------------------------------------------------------


; ---------------------------------------------------------------------------
; subroutine for turn off audio channel
; input: X register - channel offset from main loop (means, multiply by 8, means $00, $10, $20, $30)
; ---------------------------------------------------------------------------

turn_off_Audio_channel
         stx _regTemp
         txa                                 ; transfer channel offset (X reg) to A reg
         lsr @                               ; divide channel offset by 8
         lsr @                               ; to calculate AUDIO offset
         lsr @
         tax                                 ; set AUDIO offset in X register

         lda #$00                            ; silent Audio channel

.ifdef MAIN.@DEFINES.SFX_SYNCAUDIOOUT
         sta AUDIOBUF+1,x
.else
         sta audc,x                          ; store direct to POKEY register
.endif

         ldx _regTemp                        ; restore current channel offset
         rts

; ---------------------------------------------------------------------------
; subroutine to process SONG entry
; ---------------------------------------------------------------------------
; this block processes the next line of the entry in SONG
; each time the TAB finishes playing
;

.ifdef MAIN.@DEFINES.SFX_PLAYBACK

SONG_process
process_next_SONG_row
			clc
			adc #4	; shift SONG offset to next row
			tay

process_SONG_row_data
; first, check function in SONG row (always on 1st track)
			lda SONG_ADDR,y
			bmi check_SONG_order ; If the 7th bit is set, the function will be processed

;otherwise, process SONG row
			sty SONG_Ofs								; store SONG Offset

;
; SONG ROW PROCESSING

			ldx #$00										; set loop counter; step by $10
															; Must be counting, since the data is sequentially
															; arranged for tracks 1 through 4

set_SONG_track
; in A register is SONG track entry
			cmp #$40										; check, if track entry is TAB Id#
			bpl check_track_off						; if no, check other options

set_TABId_in_Track
; TAB index is in the A registry
			sty _regTemp								; store current SONG offset in temporary register

			asl @                               ; multiply TAB index by 2 to get offset in TABPtr table
			tay

; in the X register is an TAB ID offset of the TAB definition array

			lda TAB_TABLE_ADDR,y						; get TAB definition address from TABPtr table
			sta SFX_CHANNELS_ADDR+_tabPtrLo,x	; and set it in channels registers
			lda TAB_TABLE_ADDR+1,y
			sta SFX_CHANNELS_ADDR+_tabPtrHi,x

			lda #$00										; reset TAB offset and repeat counter
			sta SFX_CHANNELS_ADDR+_tabOfs,x
			sta SFX_CHANNELS_ADDR+_tabRep,x

			ldy _regTemp								; restore SONG offset

			jmp process_next_track

; other options in track entry

check_track_off
			cmp #trkOff									; check TRACK_OFF option
			beq SONGFn_TrackOff						;

; play current TAB again

			lda SFX_CHANNELS_ADDR+_tabOfs,x
			cmp #TAB_OFF
			beq dont_reset_TAB
			lda #$00										; reset TAB offset and repeat counter
			sta SFX_CHANNELS_ADDR+_tabOfs,x
			sta SFX_CHANNELS_ADDR+_tabRep,x

dont_reset_TAB
			jmp process_next_track

; TRACK OFF - turns off current track playback

SONGFn_TrackOff
			lda #$FF
			sta SFX_CHANNELS_ADDR+_tabOfs,x
         sta SFX_CHANNELS_ADDR+_chnOfs,x

         lda #$00
         sta chnCtrl
.ifdef MAIN.@DEFINES.SFX_previewChannels
         sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

			jsr turn_off_Audio_channel

process_next_track

			iny											; increase SONG offset to next track

			txa											; change current channel
			clc
			adc #$10
			tax

			cmp #$40
			beq restart_SFX_tick

			lda SONG_ADDR,y							; get track data
			jmp set_SONG_track

restart_SFX_tick
			jmp tick_start



check_SONG_order

; ---------------------------------------------------------------------------
; SONG FUNCTION PROCESSING
; ---------------------------------------------------------------------------

			cmp #trkOrd_SetTempo
			bne check_SONGFn_Repeat

; SET TEMPO

SONGFn_SetTempo
			sty SONG_Ofs

			lda SONG_Addr+1,y							; get function parameter; tempo value
			sta SONG_TEMPO

			jmp process_next_SONG_row

check_SONGFn_Repeat
			cmp #trkOrd_Repeat
			bne check_SONGFn_JumpTo

; REPEAT

SONGFn_Repeat
			lda SONG_Rep
			bne SONG_is_in_loop						; if current SONG_Rep is not zero, means Song Is In Loop

			lda SONG_Addr+1,y							; get function parameter; repeat times value
			sta SONG_Rep
			jmp SONG_Rep_JumpTo

SONG_is_in_loop
			dec SONG_Rep								; decrease current SONG_Rep
			bne SONG_Rep_JumpTo						; if SONG_Rep is not zero, Jump to position

SONG_is_out_of_loop
			sty SONG_Ofs
			tya
			jmp process_next_SONG_row				; if SONG_Rep is zero, process next SONG row

SONG_Rep_JumpTo
			lda SONG_Addr+2,y							; get secound function parameter; jump position value
			jmp SONGFn_JumpTo

; JUMP TO

check_SONGFn_JumpTo
			cmp #trkOrd_JumpTo
			bne check_SONGFn_SONG_END

			lda SONG_Addr+1,y							; get function parameter; jump position value
SONGFn_JumpTo
			asl @											; multiply this vale by 4 to get SONG offset
			asl @
			tay											; move calculated value to SONG offset register

; before making the jump, it is necessary to turn off all tracks
; as if they were playing from the beginning.

			jsr reset_all_tracks

			jmp process_SONG_row_data     	   ;

check_SONGFn_SONG_END
			cmp #trkOrd_EndSong
			bne SONGFn_notRecognized

			lda #$FF
			sta SONG_TICK_COUNTER

			jsr reset_all_tracks
			jmp end_tick

SONGFn_notRecognized
			sty SONG_Ofs
			jmp process_next_SONG_row

; end TRACK process

.endif

; ---------------------------------------------------------------------------
; subroutine for reset all tracks
; ---------------------------------------------------------------------------

reset_all_tracks
			ldx #$30

reset_TRACKS
			lda #$FF										; disable the playback...
			sta SFX_CHANNELS_ADDR+_tabOfs,x		; ... of TABs
         sta SFX_CHANNELS_ADDR+_chnOfs,x		; ... and SFXs

.ifdef MAIN.@DEFINES.SFX_previewChannels
			lda #00
         sta SFX_CHANNELS_ADDR+_chnCtrl,x
.endif

			jsr turn_off_Audio_channel

			txa											; change current channel
			sec
			sbc #$10
			tax
			bpl reset_TRACKS

			rts
