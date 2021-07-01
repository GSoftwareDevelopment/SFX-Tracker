
IO_BUFFER_ADDR			= $400;							   // 768 bytes available
IO_BUFFER_SIZE			= $140;								// 64 bytes for header; 256 bytes for raw data

// TOP_DATA					= $B400;							// only indicte top of all data used by program

PMG_BASE					= $B0;								// (1024) Player & Missile Graphics base page

VIDEO_ADDR				= $B800;								//  (240) direct screen address
VIDEO_PIANO_ADDR		= VIDEO_ADDR+240;					//   (80) view of virtual piano roll
SCREEN_BUFFER_ADDR	= VIDEO_ADDR+280;					//  (240) screen buffer (for better UI experience
MODULE_ADDR				= SCREEN_BUFFER_ADDR+20;		//   (20)
STATUSBAR_ADDR			= SCREEN_BUFFER_ADDR+220;		//   (20)
LOGO_ADDR				= SCREEN_BUFFER_ADDR+60;		//  (140) SFXMM Logo Screen

OTHER_FILE_ADDR		= $BA80;								//  (128)
CURRENT_FILE_ADDR		= $BB00;								//  (128)
SEARCH_PATH_ADDR		= $BB80;								//  (128)

CHARSET_PAGE			= $BC;
CHARSET_ADDR			= CHARSET_PAGE*$100;				//  (512) character set definition

EDIT_BUF1_ADDR			= $BE00;								//  (128)
EDIT_BUF2_ADDR			= $BE80;								//  (128)
EDIT_BUF3_ADDR			= $BF00;								//  (256)

VIS_TABLE_ADDR			= $600;								//   (58) Table for piano visualisation :)
DLI_COLOR_TABLE_ADDR = $63a;								//   (30) DLI Color table (30 bytes)
DLIST_ADDR				= $658;								//   (24) screen organization map (Display List)

RESOURCES_ADDR			= $C000;								// (...$CAFF) programm resources

LIST_BUFFER_SIZE		= 1024;								// universal list buffer size
LIST_BUFFER_ADDR		= $B400; 							// (1024) universal list buffer for UI.listChoice()

HEAP_ENTRIES			= 128;
HEAP_MEMORY_SIZE		= $27F9;

HEAP_PTRLIST_ADDR		= $CE00; // general table of pointers on the heap
HEAP_SIZES_ADDR		= $CF00; // table of data sizes on the heap
HEAP_MEMORY_ADDR		= $D800;	// = DATA_ADDR										; heap - main data memory
