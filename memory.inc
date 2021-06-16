
IO_BUFFER_ADDR			= $400;							   // 768 bytes available
IO_BUFFER_SIZE			= $13F;								// 64 bytes for header; 256 bytes for raw data

// TOP_DATA					= $B400;							// only indicte top of all data used by program

PMG_BASE					= $B0;								// Player & Missile Graphics base page

TEMP_BUFFER_ADDR		= $B800; 							// temporary buffer for store previous screen (for better UI experience)
VIDEO_ADDR				= $B900;								// direct screen address
KEYBOARD_ADDR			= VIDEO_ADDR+240;					// view of piano roll
SCREEN_BUFFER_ADDR	= VIDEO_ADDR+280;					// screen buffer (for better UI experience
MODULE_ADDR				= SCREEN_BUFFER_ADDR+20;
STATUSBAR_ADDR			= SCREEN_BUFFER_ADDR+220;
LOGO_ADDR				= SCREEN_BUFFER_ADDR+60;		// SFXMM Logo Screen
DLI_COLOR_TABLE_ADDR = SCREEN_BUFFER_ADDR+240;		// DLI Color table (45 bytes) and...
THEMES_NAMES_ADDR		= DLI_COLOR_TABLE_ADDR+30;		// Theme names (72 bytes=6 schemas*12 char/name)
DLIST_ADDR				= $BBE0;								// screen organization map (Display List)
CHARSET_PAGE			= $BC;
CHARSET_ADDR			= CHARSET_PAGE*$100;				// character set definition
EDIT_BUF1_ADDR			= $BE00;
EDIT_BUF2_ADDR			= $BE80;
VIS_TABLE_ADDR			= $BF00;								// Table for piano visualisation :)

SEARCH_PATH_ADDR		= $C000;
RESOURCES_ADDR			= $C080;								// programm resources from file resource/app_ressoures.asm

LIST_BUFFER_SIZE		= 1024;								// universal list buffer size
LIST_BUFFER_ADDR		= $C700; 							// universal list buffer for UI.listChoice()

HEAP_ENTRIES			= 128;
HEAP_MEMORY_SIZE		= $27F9;

HEAP_PTRLIST_ADDR		= $CE00; // general table of pointers on the heap
HEAP_SIZES_ADDR		= $CF00; // table of data sizes on the heap
HEAP_MEMORY_ADDR		= $D800;	// = DATA_ADDR										; heap - main data memory
