{$i modules/gsd/dli_interrupt.inc}

procedure prepareThemeList();
var
	nTheme,themeOfs:byte;
	scrOfs,y:byte;

begin
	themeOfs:=0; scrOfs:=41; y:=2; nTheme:=0;

	while (nTheme<6) do
	begin
		fillchar(@screen[scrOfs],5,$89); // "boxes" for scheme colors
		scrOfs:=scrOfs+20; // shift screen pointer to next line (12 bytes of name+2 bytes of blank)
		themeOfs:=themeOfs+12; // next color scheme
		nTheme:=nTheme+1;
	end;
end;

procedure exitThemeSelector();
begin
	wait4screen();
	fillchar(@playersHPos,4,0);
//	DLIDone();
end;

procedure ThemesScreen();
var
	y,nP:byte;
	POfs:word;

begin
	box(0,1,20,11,$40);
	prepareThemeList();

	fillchar(@playersData,$400,0);
	for y:=0 to 5 do
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

//	DLIInit();
end;
