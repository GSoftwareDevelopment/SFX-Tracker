{$i modules/gsd/dli_interrupt.inc}

procedure prepareThemeList();
var
	nTheme:byte;
	scrOfs,y:byte;

begin
	scrOfs:=41; y:=2; nTheme:=0;
	while (nTheme<6) do
	begin
		fillchar(@screen[scrOfs],5,$89); // "boxes" for scheme colors
		scrOfs:=scrOfs+20; // shift screen pointer to next line (12 bytes of name+2 bytes of blank)
		nTheme:=nTheme+1;
	end;
end;

procedure exitThemeSelector();
begin
	wait4screen();
	fillchar(@playersHPos,4,0);
end;

procedure ThemesScreen();
var
	y,nP:byte;
	POfs:word;

begin
	box(0,1,20,11,$40);
	prepareThemeList();

	PMGClear();
	POfs:=50;
	for y:=0 to 5 do
	begin
		for nP:=0 to 3 do
			fillchar(@playersData[POfs+np*$100],12,%01111110);
		inc(POfs,16);
	end;

	wait4screen(); y:=56;
	for nP:=0 to 3 do
	begin
		playersSize[nP]:=PSizeByOne;
		playersHPos[nP]:=y;
		playersColor[nP]:=$00;
		inc(y,8);
	end;
end;
