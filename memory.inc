RESOURCES_ADDR			= $AB00;	// programm resources from file resource/app_ressoures.asm

SONG_ADDR				= $B000;	// SONG table definition
SFX_MODE_SET_ADDR		= $B100; // table of SFX modes
SFX_POINTERS_ADDR		= $B200;	// heap pointers list for SFX definitions
TAB_POINTERS_ADDR		= $B300;	// heap pointers list for TAB definitions
IO_BUFFER_ADDR			= $400;	// 768 bytes available
IO_BUFFER_SIZE			= $13F;	// 64 bytes for header; 256 bytes for raw data
DLI_COLOR_TABLE_ADDR = $600;

PMG_BASE					= $B0;	// Player & Missile Graphics base page

TEMP_BUFFER_ADDR		= $B800; // temporary buffer for store previous screen (for better UI experience)
VIDEO_ADDR				= $B900;							// direct screen address
KEYBOARD_ADDR			= VIDEO_ADDR+240;				// view of piano roll
SCREEN_BUFFER_ADDR	= VIDEO_ADDR+280;				// screen buffer (for better UI experience
LOGO_ADDR				= SCREEN_BUFFER_ADDR+60;	// SFXMM Logo Screen
DLIST_ADDR				= $BBE0;							// screen organization map (Display List)
CHARSET_ADDR			= $BC00;							// character set definition
EDIT_BUF1_ADDR			= $BE00;
EDIT_BUF2_ADDR			= $BF00;

LIST_BUFFER_SIZE		= 2048;	// universal list buffer size
LIST_BUFFER_ADDR		= $C000; // universal list buffer for UI.listChoice()


