xitvbl      = $e462
sysvbv      = $e45c
audf        = $d200
audc        = $d201

; CONSTANTS - offsets in channels registers table
_sfxPtr     = 0
_sfxPtrLo   = 0
_sfxPtrHi   = 1
_chnOfs     = 2
_chnNote    = 3
_chnFreq    = 4

_chnMode    = 5
_chnModVal  = 6
_chnCtrl    = 7

_tabPtr		= 8
_tabPtrLo	= 8
_tabPtrHi	= 9
_tabOfs		= 10

; VARIABLES - PAGE ZERO
SONG_LPB		= $F0		  ; SONG Line Per Beat
TABPtr		= $F1		  ; TAB Pointer (2 bytes)
TABOfs		= $F2		  ; TAB Offset

sfxPtr      = $F5      ; SFX Pointer (2 bytes)
chnNote     = $F7      ; SFX Note
chnFreq     = $F8      ; SFX Frequency

chnMode     = $F9      ; SFX Modulation Mode
chnModVal   = $FA      ; SFX Modulator
chnCtrl     = $FB      ; SFX Control (distortion & volume)

_regTemp    = $FC


; CONSTANTS

SFX_OFF           = $FF

MODFN_SFX_STOP    = $80
MODFN_LFD_FREQ    = $20
MODFN_NLM_NOTE    = $40
MODFN_MFD_FREQ    = $40

MODMODE_DFD       = 3
MODMODE_LFD_NVM   = 2
MODMODE_MFD       = 1
MODMODE_HFD       = 0
MODMODE_RELATIVE  = %1000

