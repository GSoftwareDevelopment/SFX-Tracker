{$DEFINE SFX_SWITCH_ROM}         // Enables RAM under ROM for SFX engine (on interrupt)
{$DEFINE SFX_previewChannels}    // Enables live viewing of data in channels
{$DEFINE SFX_SYNCAUDIOOUT}       // Enables audio output buffering for better audio synchronization

{$DEFINE USE_MODULATORS}         // Enables the use of modulators
{$DEFINE DFD_MOD}                // Direct Frequency Divider modulator
{$DEFINE LFD_NLM_MOD}            // Low Frequency Divider & Note Level Modulation
{$DEFINE MFD_MOD}                // Middle Frequency Divider modulator
{$DEFINE HFD_MOD}                // High Frequency Divider modulator
// {$DEFINE USE_ALL_MODULATORS}

{$DEFINE SFX_PLAYBACK}           // Enable SONG/TAB playback support

AUDIO_BUFFER_ADDR = $E8;
SFX_REGISTERS     = $F0;

SFX_CHANNELS_ADDR = $6C0;
SONG_ADDR         = $CB00;                // table of SONG definition
SFX_MODE_SET_ADDR = $CC00;                // table of SFX modes
SFX_NOTE_SET_ADDR = $CC80;                // table of SFX note table presets

NOTE_TABLE_PAGE   = $CD;
NOTE_TABLE_ADDR   = NOTE_TABLE_PAGE*$100; // = NOTE_TABLE_ADDR                      ; predefined table of note frequency

SFX_TABLE_ADDR    = $CE00;                // list for SFX definitions
TAB_TABLE_ADDR    = SFX_TABLE_ADDR+$0080; // list for TAB definitions

DATA_ADDR         = $D800;                // data address


