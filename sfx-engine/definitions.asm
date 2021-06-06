SFX_TABLE_ADDR			= $CE00;	// list for SFX definitions
TAB_TABLE_ADDR			= $CE80;	// list for TAB definitions
DATA_ADDR				= $D800;	// data address
NOTE_TABLE_ADDR		= $CD80;
SFX_NameLength			= 14


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
_tabRep		= 11

; VARIABLES - PAGE ZERO
SONG_LPB		= $F0		  ; SONG Line Per Beat
SONG_TICK	= $F1		  ; SONG tick counter

.ifdef MAIN.@DEFINES.SFX_SYNCAUDIOOUT
AUDIOBUF		= $E8		  ; 8 bytes audio buffer for sync output
.endif

TABPtr		= $F5		  ; TAB Pointer (2 bytes)
TABNote		= $F7		  ; TAB Note
TABOrder		= $F8		  ; TAB Order

sfxPtr      = $F5      ; SFX Pointer (2 bytes)
chnNote     = $F7      ; SFX Note
chnFreq     = $F8      ; SFX Frequency

chnMode     = $F9      ; SFX Modulation Mode
chnModVal   = $FA      ; SFX Modulator
chnCtrl     = $FB      ; SFX Control (distortion & volume)

_regTemp    = $FC


; CONSTANTS

SFX_OFF           = $FF
TAB_OFF				= $FF

FN_NOTE_FREQ		= $40

MODFN_SFX_STOP    = $80
MODFN_LFD_FREQ    = $20
MODFN_NLM_NOTE    = $40
MODFN_MFD_FREQ    = $40

MODMODE_DFD       = 3
MODMODE_LFD_NVM   = 2
MODMODE_MFD       = 1
MODMODE_HFD       = 0
MODMODE_RELATIVE  = %1000

