
IO_BUFFER_ADDR			= $400;	// 768 bytes available
IO_BUFFER_SIZE			= $13F;	// 64 bytes for header; 256 bytes for raw data
DLI_COLOR_TABLE_ADDR = $600;

TOP_DATA					= $B400;	// only indicte top of all data used by program

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

RESOURCES_ADDR			= $C000;	// programm resources from file resource/app_ressoures.asm

LIST_BUFFER_SIZE		= 1024;	// universal list buffer size
LIST_BUFFER_ADDR		= $C800; // universal list buffer for UI.listChoice()

SONG_ADDR				= $CC00;	// SONG table definition
SFX_MODE_SET_ADDR		= $CD00; // table of SFX modes
NOTE_TABLE_ADDR		= $CD80; // note frequency divider table

HEAP_ENTRIES			= 128;
HEAP_MEMORY_SIZE		= $27F9;
HEAP_PTRLIST_ADDR		= $CE00;

SFX_POINTERS_ADDR		= HEAP_PTRLIST_ADDR;	// heap pointers list for SFX definitions
TAB_POINTERS_ADDR		= HEAP_PTRLIST_ADDR + $0080;	// heap pointers list for TAB definitions

HEAP_SIZES_ADDR		= $CF00;

HEAP_MEMORY_ADDR		= $D800;	// heap - main data memory
