var
	PFCOLS:array[0..4] of byte absolute 708;
	colorThemes:array[0..0] of byte;
	currentTheme:byte;

	NMIEN:word absolute $d40e;
	oldDLI:pointer;

{$i modules/theme/theme_manage.inc}
{$i modules/theme/theme_edit.inc}

procedure initThemeSelector();
var
	y,nP:byte;
	POfs:word;

begin
	box(0,2,20,9,$40);
	prepareThemeList();

	fillchar(@playersData,$400,0);
	for y:=0 to 8 do
	begin
		POfs:=50+y*16;
		for nP:=0 to 3 do
			fillchar(@playersData[POfs+np*$100],12,%01111110);
	end;

	wait4screen();
	for nP:=0 to 3 do
	begin
		playersSize[nP]:=PSizeByOne;
		playersHPos[nP]:=56+nP*8;
		playersColor[nP]:=$00;
	end;

	NMIEN:=%01000000; // turn off DLI
	DLIThemeCnt:=0; // reset DLI counter
	getIntVec(iDLI, oldDLI);
	SetIntVec(iDLI, @theme_dli);
	NMIEN:=%11000000; // turn on DLI
end;

procedure exitThemeSelector();
begin
	wait4screen();
	playersHPos[0]:=0;
	playersHPos[1]:=0;
	playersHPos[2]:=0;
	playersHPos[3]:=0;

	NMIEN:=%01000000; // turn off DLI
	setIntVec(iDLI, oldDLI);
end;

procedure theme_selector();
var
	nTheme:byte;

begin
	initThemeSelector();
	nTheme:=currentTheme;
	colorHLine(6,2+nTheme,14,3);
	screen2video();

	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			colorHLine(6,2+nTheme,14,1);
			controlSelectionKeys(key,key_Up,key_Down,nTheme,0,8);
			case key of
				key_ESC: break;
				key_RETURN: setTheme(nTheme);
				key_SPACE: begin
					editTheme(nTheme);
					exitThemeSelector();
					initThemeSelector();
				end;
			end;
			colorHLine(6,2+nTheme,14,3);
			screen2video();
		end;
	until false;

	exitThemeSelector();
	move(@tmpbuf[20],@screen[20],200);
end;

procedure initThemes(themesPtr:pointer);
begin
	colorThemes:=themesPtr;
	setTheme(0);
end;
