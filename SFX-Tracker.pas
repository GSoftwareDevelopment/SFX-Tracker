{$DEFINE ROMOFF}

{$librarypath './units/'}
uses heap,gr2,ui;

const
{$i memory.inc}
{$i const.inc}

	palette:array[0..4] of byte = (
		$0a,	// color 0
		$e6,	// color 1
		$68,	// color 2
		$34,	// color 3
		$00	// color 4
(*
		$00,
		$02,
		$04,
		$30,
		$08
*)
	);

{$r resources.rc}
{$i types.inc}
{ $i interrupts.inc}

var
	CHBAS:byte absolute 756;
	KRPDEL:byte absolute $2d9;
	KEYREP:byte absolute $2da;
	PFCOLS:array[0..4] of byte absolute 708;

	resptr:array[0..0] of pointer absolute DATA_DEFS_ADDR;
	listBuf:array[0..0] of byte absolute LIST_BUFFER_ADDR;

	SFXPtr:array[0..maxSFXs-1] of word absolute INSTR_POINTERS_ADDR;
	TABPtr:array[0..maxTABs-1] of word absolute PTRN_POINTERS_ADDR;
	songData:array[0..255] of byte absolute SONG_ADDR;
	tmpbuf:array[0..255] of byte;

	cursorPos:smallInt;
	cursorShift:smallInt;

	song_tact,song_beat,song_lpb:byte;

	currentMenu:byte;
	section:shortint;

	currentSFX:shortint;
	currentOct:shortint;
	currentTAB:shortint;

	key:TKeys;

//

{$i modules/gsd_options/gsd_options.pas}
{$i modules/io/io.pas}
{$i modules/sfx/sfx.pas}
{$i modules/tab/tabs.pas}

procedure init();
begin
	HEAP_Init();
	initGraph(DLIST_ADDR,VIDEO_ADDR,BUFFER_ADDR);

	fillchar(@screen[40],20,$80);
//	putNText(5,11,resptr[str_GSD_footer],0);
	KRPDEL:=20;
	KEYREP:=3;
	CHBAS:=$BC;
	move(@palette,@PFCOLS,5);
//	PFCOLS[0]:=$0f; pfcols[2]:=$84;

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

end;

begin
	init();
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			case key of
				key_RETURN:
					case currentMenu of
						0: GSDOptions();
						1: IOScreen();
						2: SFXScreen();
						3: TABScreen();
					end;
				key_Left: if (currentMenu>0) then currentMenu:=currentMenu-1;
				key_Right: if (currentMenu<4) then currentMenu:=currentMenu+1;
			end;
			updateBar(resptr[menu_top],width_menuTop,currentMenu,0,color_selected);
		end;
		screen2video();
	until false;
end.
