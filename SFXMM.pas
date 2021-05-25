{$DEFINE ROMOFF}

{$librarypath './units/'}
uses sysutils, strings, heap, gr2, ui, pmgraph;

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

	themesNames:array[0..0] of byte absolute DLI_COLOR_TABLE_ADDR+45; // list of subject names; located just after the color definition for the DLI
	currentTheme:byte;

//
	SFXModMode:array[0..maxSFXs-1] of byte absolute SFX_MODE_SET_ADDR; // indicates the type of modulation used in the SFX.
(*
	0 - HFD - High Freq. Div.     - relative modulation of the frequency divider in the range of +/- 127
											- without the possibility of looping the SFX
											- Full backwards compatibility with the original SFX engine
	1 - MFD - Middle Freq. Div.   - relative modulation of the frequency divider in the range of +/- 63
											- SFX looping possible
	2 - LFD/NLM - Low Freq Div.	- note level modulation in relative range of +/- 32 half tones;
											- relative modulation of freq. divider in the range of +/- 32
											- SFX looping possible
	3 - DSD - Direct Set Div.		- direct set of the frequency divider - without looping possible
*)

	SFXPtr:array[0..maxSFXs-1] of word absolute SFX_POINTERS_ADDR; // heap pointers to SFX definitions
	TABPtr:array[0..maxTABs-1] of word absolute TAB_POINTERS_ADDR; // heap pointera to TAB definitions
	SONGData:array[0..255] of byte absolute SONG_ADDR; // table for SONG data
	SONGTitle:string[SONGNameLength];
	song_tact,song_beat,song_lpb:byte;

//
	currentFile:string; // indicate a current opened SFXMM file with full path and device
	searchPath:string; // used only in IO->DIR

	cursorPos:smallInt;
	cursorShift:smallInt;

	currentMenu:shortint;
	section:byte;

	currentSFX:byte;
	currentOct:byte;
	currentTAB:byte;

	modified:boolean = false;
	key:TKeys;

// global access function and procedures (must have :D)
{$i modules/io/io_error.inc}
{$i modules/io/io_prompt.inc}

// modules
{$i modules/gsd/gsd.pas}
{$i modules/io/io.pas}
{$i modules/modified.inc}
{$i modules/sfx/sfx.pas}
{$i modules/tab/tab.pas}

procedure init();
begin
	HEAP_Init();
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
	fillchar(@SFXPtr,maxSFXs*2,$ff);
	fillchar(@TABPtr,maxTABs*2,$ff);
	fillchar(@songData,256,$ff);

	currentMenu:=0;

// set defaults
	currentSFX:=0;
	currentTAB:=0;
	currentOct:=$10;
	song_tact:=4;
	song_beat:=4;
	song_lpb:=4;

	fillchar(@SONGTitle,SongNameLength,0);
	move(@defaultSongTitle,@SONGTitle,length(defaultSongTitle)+1);

	fillchar(@currentFile,FILEPATHMaxLength,0);
	move(@defaultFileName,@currentFile,length(defaultFileName)+1);

	fillchar(@searchPath,FILEPATHMaxLength,0);
	move(@defaultSearchPath,@searchPath,length(defaultSearchPath)+1);

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
