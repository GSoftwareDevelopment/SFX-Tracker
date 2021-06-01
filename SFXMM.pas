{$DEFINE ROMOFF}

{$librarypath './units/'}
uses SFX_Engine, sysutils, strings, gr2, ui, pmgraph;

{$i types.inc}

const
{$i memory.inc}
{$i const.inc}
{$r resources.rc}

var
	CHBAS:byte absolute 756;
	KRPDEL:byte absolute $2d9;
	KEYREP:byte absolute $2da;

	listBuf:array[0..0] of byte absolute LIST_BUFFER_ADDR; // universal list buffer array
	tmpbuf:array[0..255] of byte absolute TEMP_BUFFER_ADDR; // store previous screen, for better UI experience
	IOBuf:array[0..IO_BUFFER_SIZE-1] of byte absolute IO_BUFFER_ADDR;

	resptr:array[0..0] of pointer absolute RESOURCES_ADDR; // pointers list to resources

	themesNames:array[0..0] of byte absolute DLI_COLOR_TABLE_ADDR+45; // list of themes names; located just after the color definition for the DLI
	currentTheme:byte;
//

	HEAP_TOP:word; // memory occupied by heap
	_mem:array[0..0] of byte absolute HEAP_MEMORY_ADDR;
	HEAP_PTR:array[0..0] of word absolute HEAP_PTRLIST_ADDR;
	_heap_sizes:array[0..0] of word absolute HEAP_SIZES_ADDR;

//
	SONGTitle:string[SONGNameLength];

	currentFile:string[FILEPATHMaxLength]; // indicate a current opened SFXMM file with full path and device
	searchPath:string[FILEPATHMaxLength]; // used only in IO->DIR

	cursorPos:byte;
	cursorShift:byte;

	currentMenu:shortint;
	section:byte;

	currentSFX:byte;
	currentOct:byte;
	currentTAB:byte;

	modified:boolean = false;
	key:TKeys;

// global access function and procedures
{$i units/heap_manage.inc}
{$i modules/io/io_clear_all_data.inc}
{$i modules/io/io_error.inc}
{$i modules/io/io_prompt.inc}
{$i modules/io/io_tag_compare.inc}

{$i modules/vis_piano.inc}

// modules
{$i modules/gsd/gsd.pas}
{$i modules/io/io.pas}
{$i modules/sfx/sfx.pas}
{$i modules/tab/tab.pas}

procedure init();
begin
	INIT_SFXEngine(HEAP_MEMORY_ADDR,SFX_MODE_SET_ADDR,SFX_POINTERS_ADDR,TAB_POINTERS_ADDR,SONG_ADDR);
	SetNoteTable(NOTE_TABLE_ADDR);

	PMGInit(PMG_BASE);
	initGraph(DLIST_ADDR,VIDEO_ADDR,SCREEN_BUFFER_ADDR);
	getTheme(0,PFCOLS); // set default theme color
	IOLoadTheme(defaultThemeFile);
	fillchar(@screen[0],20,$40);
	fillchar(@screen[20],20,$00);
	fillchar(@screen[40],20,$80);
	KRPDEL:=20;
	KEYREP:=3;
	CHBAS:=$BC;

	fillchar(@listBuf,LIST_BUFFER_SIZE,0);

	currentMenu:=0;

// set defaults
	move(@defaultSongTitle,@SONGTitle,length(defaultSongTitle)+1);

	fillchar(@currentFile,FILEPATHMaxLength,0);
	move(@defaultFileName,@currentFile,length(defaultFileName)+1);

	fillchar(@searchPath,FILEPATHMaxLength,0);
	move(@defaultSearchPath,@searchPath,length(defaultSearchPath)+1);

	IO_clearAllData();
end;

begin
	init();
	repeat
		if optionsList(resptr[menu_top],width_menuTop,5,currentMenu,key_Left,key_Right) then
			case currentMenu of
				0: GSDModule();
				1: IOModule();
				2: SFXModule();
				3: TABModule();
			end;
	until false;
end.
