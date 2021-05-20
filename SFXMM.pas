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

	resptr:array[0..0] of pointer absolute RESOURCES_ADDR;

	SFXPtr:array[0..maxSFXs-1] of word absolute SFX_POINTERS_ADDR;
	TABPtr:array[0..maxTABs-1] of word absolute TAB_POINTERS_ADDR;
	SONGData:array[0..255] of byte absolute SONG_ADDR;
	SONGTitle:string[SONGNameLength];
	currentFile:string[FILEPATHMaxLength]; // indicate a current opened SFXMM file with full path and device

	cursorPos:smallInt;
	cursorShift:smallInt;

	song_tact,song_beat,song_lpb:byte;

	currentMenu:byte;
	section:byte;

	currentSFX:byte;
	currentOct:byte;
	currentTAB:byte;

	modified:boolean = false;
	key:TKeys;

//

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
	initThemes(resptr[color_themes]);
	fillchar(@screen[0],40,$00);
	fillchar(@screen[40],20,$80);
	KRPDEL:=20;
	KEYREP:=3;
	CHBAS:=$BC;

	fillchar(@listBuf,1600,0);
	fillchar(@SFXPtr,maxSFXs*2,$ff);
	fillchar(@TABPtr,maxTABs*2,$ff);
	fillchar(@songData,256,$ff);

	menuBar(resptr[menu_top],width_menuTop,0);
	currentMenu:=0;
	updateBar(resptr[menu_top],width_menuTop,currentMenu,0,color_selected);
	screen2video();

// set defaults
	currentSFX:=0;
	currentTAB:=0;
	currentOct:=$10;
	song_tact:=4;
	song_beat:=4;
	song_lpb:=4;

	SONGTitle:=DefaultSongTitle;
	currentFile:=DefaultFileName;
end;

begin
	init();
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			controlSelectionKeys(key,key_Left,key_Right,currentMenu,0,4);
			if key=key_RETURN then
				case currentMenu of
					0: GSDModule();
					1: IOModule();
					2: SFXModule();
					3: TABModule();
				end;
			updateBar(resptr[menu_top],width_menuTop,currentMenu,0,color_selected);
		end;
		screen2video();
	until false;
end.
