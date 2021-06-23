program SFXMM;
{$librarypath './units/'}
{$librarypath './sfx_engine/'}

{$DEFINE ROMOFF}

uses SFX_Engine, sysutils, strings, gr2, ui, pmgraph;
{$I-}
{$i types.inc}

const
{$i memory.inc}
{$i const.inc}
{$r resources.rc}

var
	CHBAS:byte absolute 756;
	KRPDEL:byte absolute $2d9;
	KEYREP:byte absolute $2da;

//	buffers

	listBuf:array[0..0] of byte absolute LIST_BUFFER_ADDR; // universal list buffer array
	tmpbuf:array[0..255] of byte absolute IO_BUFFER_ADDR;
	IOBuf:array[0..IO_BUFFER_SIZE-1] of byte absolute IO_BUFFER_ADDR;

//	UI color themes

	themesNames:array[0..0] of byte; // list of themes names
	currentTheme:byte;

// heap

	HEAP_TOP:word; // memory occupied by heap
	_mem:array[0..0] of byte absolute HEAP_MEMORY_ADDR;
	HEAP_PTR:array[0..0] of word absolute HEAP_PTRLIST_ADDR;
	_heap_sizes:array[0..0] of word absolute HEAP_SIZES_ADDR;

//

	SONGTitle:string[SONGNameLength];

	otherFile:string[FILEPATHMaxLength] absolute OTHER_FILE_ADDR;
	searchPath:string[FILEPATHMaxLength] absolute SEARCH_PATH_ADDR; // used only in IO->DIR
	currentFile:string[FILEPATHMaxLength] absolute CURRENT_FILE_ADDR; // indicate a current opened SFXMM file with full path and device
	FName:string[16];

	cursorPos,cursorShift:byte;			// general cursor position and view offset

	SONGChn,SONGPos,SONGShift:byte;		// SONG current channel,position and view offset

	currentMenu:byte;
	section:byte;

	currentSFX:byte;
	currentOct:byte;
	currentTAB:byte;

	song_beat:byte;

	modified:boolean = false;

//
	statusBar:array[0..0] of byte absolute STATUSBAR_ADDR;
	moduleBar:array[0..0] of byte absolute MODULE_ADDR;
	note_names:array[0..0] of byte;
	octShift:array[0..0] of byte;

	f:file;

// global access function and procedures
{$i units/heap_manage.inc}
{$i modules/ui_helpers.inc}
{$i modules/io/io_clear_all_data.inc}
{$i modules/io/io_error.inc}
{$i modules/io/io_tag_compare.inc}
{$i modules/io/io_manage.inc}
{$i modules/io/io_options.inc}
{$i modules/io/io_dir.inc}
{$i modules/vis_piano.inc}

// modules
{$i modules/gsd/gsd.pas}
{$i modules/io/io.pas}
{$i modules/notetune/notetune.pas}
{$i modules/sfx/sfx.pas}
{$i modules/tab/tab.pas}
{$i modules/song/song.pas}

procedure init();
begin
	initGraph(DLIST_ADDR,VIDEO_ADDR,SCREEN_BUFFER_ADDR); CHBAS:=CHARSET_PAGE;
	PMGInit(PMG_BASE);
	Init_UI(RESOURCES_ADDR);

	getTheme(0,PFCOLS); // set default theme color

// keyboard set
	KRPDEL:=20;	KEYREP:=3;

// keyboard resources set
	chars_alphaNum:=resptr[scan_to_scr];
	keys_alphaNum:=resptr[scan_key_codes];
	keys_notes:=resptr[scan_piano_codes];

// other resources set
	note_names:=resptr[str_NoteNames];
	themesNames:=resptr[themes_names_list];
	octShift:=resptr[octaveShifts];

	IO_clearAllData();

	reset_pianoVis();
	updatePiano();
	SFX_Start();

// load defaults
	setFilename(defaultThemeFile,otherFile);
	IOLoadTheme();
	IOLoadDefaultNoteTable();

// set defaults files
	clearFilename(otherFile);
	setFilename(defaultFileName,currentFile);
	setFilename(defaultSearchPath,searchPath);

	currentMenu:=0;
end;

begin
	init();
	repeat
		clearTopMenu();
		if optionsList(menu_top,width_menuTop,5,currentMenu,key_Left,key_Right) then
			case currentMenu of
				0: GSDModule();
				1: IOModule();
				2: SFXModule();
				3: TABModule();
				4: SONGModule();
			end;
		uncolorWorkarea();
	until false;
end.
