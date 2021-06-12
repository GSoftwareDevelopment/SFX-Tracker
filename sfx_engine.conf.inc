{$DEFINE SFX_SWITCH_ROM}
{$DEFINE SFX_previewChannels}
{$DEFINE SFX_SYNCAUDIOOUT}

{$DEFINE USE_MODULATORS}
{$DEFINE DFD_MOD}
{$DEFINE LFD_NLM_MOD}
{$DEFINE MFD}
{$DEFINE HFD}
// {$DEFINE USE_ALL_MODULATORS}

{$DEFINE TAB_PLAYBACK}

SFX_CHANNELS_ADDR	= $6C0;
SONG_ADDR			= $CB00;	// table of SONG definition
SFX_MODE_SET_ADDR	= $CC00; // table of SFX modes
SFX_NOTE_SET_ADDR	= $CC80; // The array of pointers to arrays of notes
NOTE_TABLE_ADDR	= $CD00;

SFX_TABLE_ADDR		= $CE00;	// list for SFX definitions
TAB_TABLE_ADDR		= SFX_TABLE_ADDR+$0080;	// list for TAB definitions

DATA_ADDR			= $D800;	// data address


