RESOURCES_ADDR			= $AC00;	// programm resources from file resource/app_ressoures.asm

LIST_BUFFER_SIZE		= 2048;	// universal list buffer size
LIST_BUFFER_ADDR		= $C000; // universal list buffer for UI.listChoice()

SONG_ADDR				= $B000;	// SONG table definition
SFX_POINTERS_ADDR		= $B100;	// heap pointers list for SFX definitions
TAB_POINTERS_ADDR		= $B200;	// heap pointers list for TAB definitions
TEMP_BUFFER_ADDR		= $B300; // temporary buffer for store previous screen (for better UI experience)
IO_BUFFER_ADDR			= $400;	// 768 bytes available
IO_BUFFER_SIZE			= $13F;	// 64 bytes for header; 256 bytes for raw data

PMG_BASE					= $B0;	// Player & Missile Graphics base page

VIDEO_ADDR				= $B900;							// direct screen address
KEYBOARD_ADDR			= VIDEO_ADDR+240;				// view of piano roll
SCREEN_BUFFER_ADDR	= VIDEO_ADDR+280;				// screen buffer (for better UI experience
LOGO_ADDR				= SCREEN_BUFFER_ADDR+60;	// SFXMM Logo Screen
DLIST_ADDR				= $BBE0;							// screen organization map (Display List)
CHARSET_ADDR			= $BC00;							// character set definition

