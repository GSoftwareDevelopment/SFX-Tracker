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
	tmpbuf:array[0..255] of byte absolute TEMP_BUFFER_ADDR; // store previous screen, for better UI experience
	IOBuf:array[0..IO_BUFFER_SIZE-1] of byte absolute IO_BUFFER_ADDR;

//	resources

	resptr:array[0..0] of pointer absolute RESOURCES_ADDR; // pointers list to resources

//	UI color themes

	themesNames:array[0..0] of byte absolute THEMES_NAMES_ADDR; // list of themes names
	currentTheme:byte;

// heap

	HEAP_TOP:word; // memory occupied by heap
	_mem:array[0..0] of byte absolute HEAP_MEMORY_ADDR;
	HEAP_PTR:array[0..0] of word absolute HEAP_PTRLIST_ADDR;
	_heap_sizes:array[0..0] of word absolute HEAP_SIZES_ADDR;

//

	SONGTitle:string[SONGNameLength];

	searchPath:string[FILEPATHMaxLength] absolute SEARCH_PATH_ADDR; // used only in IO->DIR
	currentFile:string[FILEPATHMaxLength]; // absolute $b300; // indicate a current opened SFXMM file with full path and device
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

	f:file;

// global access function and procedures
{$i units/heap_manage.inc}
{$i modules/io/io_clear_all_data.inc}
{$i modules/io/io_error.inc}
{$i modules/io/io_tag_compare.inc}
{$i modules/io/io_manage.inc}
{$i modules/io/io_options.inc}
{$i modules/edit_ctrl.inc}
{$i modules/vis_piano.inc}

// modules
{$i modules/gsd/gsd.pas}
{$i modules/io/io.pas}
{$i modules/sfx/sfx.pas}
{$i modules/tab/tab.pas}
{$i modules/song/song.pas}

procedure init();
begin
//	INIT_SFXEngine();

	PMGInit(PMG_BASE);
	initGraph(DLIST_ADDR,VIDEO_ADDR,SCREEN_BUFFER_ADDR);
	KRPDEL:=20;	KEYREP:=3; CHBAS:=CHARSET_PAGE;

	getTheme(0,PFCOLS); // set default theme color

	fillchar(@screen[0],20,$40);

	Init_UI(resptr[scan_to_scr],resptr[scan_key_codes]);
	keys_notes:=resptr[scan_piano_codes];

	fillchar(@listBuf,LIST_BUFFER_SIZE,0);

	currentMenu:=0;

	IO_clearAllData();

	reset_pianoVis();
	updatePiano();
	SFX_Start();

	IOLoadTheme(defaultThemeFile);
	IOLoadDefaultNoteTable();

// set defaults
	setFilename(defaultFileName,currentFile);
//	fillchar(@currentFile,FILEPATHMaxLength,0);
//	move(@defaultFileName,@currentFile,length(defaultFileName)+1);

	setFilename(defaultSearchPath,searchPath);
//	fillchar(@searchPath,FILEPATHMaxLength,0);
//	move(@defaultSearchPath,@searchPath,length(defaultSearchPath)+1);

end;

procedure uncolorWorkarea();
var i:byte;

begin
	for i:=1 to 11 do
		colorHLine(0,i,20,0);
end;

begin
	init();
	repeat
		fillchar(@screen,20,$40);
		if optionsList(resptr[menu_top],width_menuTop,5,currentMenu,key_Left,key_Right) then
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
