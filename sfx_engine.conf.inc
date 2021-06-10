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

const
	SFX_CHANNELS_ADDR	= $6C0;
	SFX_TABLE_ADDR		= $CE00;	// list for SFX definitions
	SFX_MODE_SET_ADDR	= $CC00; // table of SFX modes
	SFX_NOTE_SET_ADDR	= $CC80; // The array of pointers to arrays of notes
	TAB_TABLE_ADDR		= $CE80;	// list for TAB definitions
	DATA_ADDR			= $D800;	// data address
	NOTE_TABLE_ADDR	= $CD80;

	SONG_ADDR			= $CB00;	// table of SONG definition

