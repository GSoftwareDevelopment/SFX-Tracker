{$DEFINE SFX_SWITCH_ROM}         // Enables RAM under ROM for SFX engine (on interrupt)
{$DEFINE SFX_previewChannels}    // Enables live viewing of data in channels
{$DEFINE SFX_SYNCAUDIOOUT}       // Enables audio output buffering for better audio synchronization

{$DEFINE USE_MODULATORS}         // Enables the use of modulators
{$DEFINE DFD_MOD}                // Direct Frequency Divider modulator
{$DEFINE LFD_NLM_MOD}            // Low Frequency Divider & Note Level Modulation
{$DEFINE MFD_MOD}                // Middle Frequency Divider modulator
{$DEFINE HFD_MOD}                // High Frequency Divider modulator
// {$DEFINE USE_ALL_MODULATORS}

{$DEFINE TAB_PLAYBACK}           // Enable TAB support

SFX_CHANNELS_ADDR = $6C0;
SONG_ADDR         = $CC00;                // table of SONG definition
SFX_MODE_SET_ADDR = $CD00;                // table of SFX modes
SFX_NOTE_SET_ADDR = $CD80;                // table of SFX note table presets

NOTE_TABLE_PAGE		= $CD;
NOTE_TABLE_ADDR		= NOTE_TABLE_PAGE*$100; // = NOTE_TABLE_ADDR								; predefined table of note frequency
PREDEF_PURE_ADDR     = NOTE_TABLE_ADDR;      //
PREDEF_BASS1_ADDR    = NOTE_TABLE_ADDR+$40;  //
PREDEF_BASS2_ADDR    = NOTE_TABLE_ADDR+$80;  //


SFX_TABLE_ADDR    = $CE00;                // list for SFX definitions
TAB_TABLE_ADDR    = SFX_TABLE_ADDR+$0080; // list for TAB definitions

DATA_ADDR         = $D800;                // data address


