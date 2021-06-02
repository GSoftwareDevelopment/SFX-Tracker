xitvbl      = $e462
sysvbv      = $e45c
audf        = $d200
audc        = $d201

; below variables is only for current channel
_sfxPtr     = 0
_sfxPtrLo   = 0
_sfxPtrHi   = 1
_chnOfs     = 2
_chnNote    = 3
_chnFreq    = 4

_chnMode    = 5
_chnModVal  = 6
_chnCtrl    = 7

sfxPtr      = $F5      ; SFX Pointer (2 bytes)
; chnOfs      = $F7      ; SFX Offset in SFX definition
chnNote     = $F7      ; SFX Note
chnFreq     = $F8      ; SFX Frequency

chnMode     = $F9      ; SFX Modulation Mode
chnModVal   = $FA      ; SFX Modulator
chnCtrl     = $FB      ; SFX Control (distortion & volume)

_regA       = $FD
_regX       = $FE
_regY       = $FF

;

SFX_OFF				= $FF

MODFN_SFX_STOP		= $80
MODFN_LFD_FREQ		= $20
MODFN_NLM_NOTE		= $40
MODFN_MFD_FREQ		= $40

MODMODE_DFD			= 3
MODMODE_LFD_NVM	= 2
MODMODE_MFD			= 1
MODMODE_HFD			= 0
MODMODE_RELATIVE	= %1000
