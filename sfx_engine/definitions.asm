xitvbl      = $e462
sysvbv      = $e45c
audf        = $d200
audc        = $d201

; CONSTANTS - offsets in channels registers table

            icl 'sfx_engine_const.inc'

; VARIABLES - PAGE ZERO
.ifdef MAIN.@DEFINES.SFX_SYNCAUDIOOUT
AUDIOBUF          = $E8      ; 8 bytes audio buffer for sync output
.endif

SONG_LPB          = $F0      ; SONG Line Per Beat
SONG_TICK_COUNTER = $F1      ; SONG tick counter
_regTemp2			= $F3
TABPtr            = $F4      ; TAB Pointer (2 bytes)
TABNote           = $F7      ; TAB Note
TABOrder          = $F8      ; TAB Order

sfxPtr            = $F4      ; SFX Pointer (2 bytes)
sfxNoteOfs        = $F6      ; SFX Note Table offset (1 byte)
chnNote           = $F7      ; SFX Note
chnFreq           = $F8      ; SFX Frequency

chnMode           = $F9      ; SFX Modulation Mode
chnModVal         = $FA      ; SFX Modulator
chnCtrl           = $FB      ; SFX Control (distortion & volume)

_regTemp          = $FC


; CONSTANTS

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
