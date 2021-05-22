procedure prepareThemeList();
var
	nTheme,colTabOfs,themeOfs,themeNameLen:byte;
	scrOfs,y:byte;

begin
	fillchar(@DLIColTab,9*4,$0);
	colTabOfs:=0; themeOfs:=0; scrOfs:=41; y:=2; nTheme:=0;

	// first, show theme sets
	while (nTheme<6) do
	begin
		fillchar(@screen[scrOfs],5,$89);

		move(@themesDef[themeOfs],@DLIColTab[colTabOfs],5); // copy theme colors to DLI color table
		colTabOfs:=colTabOfs+5; // next DLI color entry
		scrOfs:=scrOfs+6; // shift screen pointer to theme name position
		themeOfs:=themeOfs+5; // skip first 5 bytes of color definition

		themeNameLen:=0;
		while (themesDef[themeOfs+themeNameLen]<>255) do // determine theme name length
			themeNameLen:=themeNameLen+1;

		if themeNameLen<=12 then
			move(@themesDef[themeOfs],@screen[scrOfs],themeNameLen) // put all name to screen
		else
			move(@themesDef[themeOfs],@screen[scrOfs],12); // put only 12 fisrt character to screen
		colorHLine(6,y,14,1); // set background color for name

		y:=y+1;
		scrOfs:=scrOfs+14; // shift screen pointer o next line (12 bytes of name+2 bytes of blank
		themeOfs:=themeOfs+themeNameLen+1; // next color schema

		nTheme:=nTheme+1;
	end;
	menuBar(resptr[menu_themes],0,1);
end;

procedure initThemeSelector();
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

	DLIInit();
end;


procedure exitThemeSelector();
begin
	wait4screen();
	fillchar(@playersHPos,4,0);
	DLIDone();
end;

procedure ThemesScreen();
begin
	initThemeSelector();
end;

procedure ThemeEditScreen();
var i:byte;

begin
	PMGClear();
	for i:=0 to 4 do fillchar(@playersData[50+i*16],12,255);

	playersHPos[0]:=144;
	playersSize[0]:=PSizeByFour;

	box(0,2,20,9,0);
	VBar(0,2,width_themeEdit,0);
	menuBar(resptr[menu_themeEdit],width_themeEdit,1);
end;
