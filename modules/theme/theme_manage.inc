procedure getTheme(nTheme:shortint; var colors:byteArray);
var
	themeOfs:byte;

begin
	currentTheme:=nTheme;
	themeOfs:=0;
	// seek offset of theme number
	while (nTheme>0) do
	begin
		themeOfs:=themeOfs+5; // skip first 5 bytes of color definition
		while (colorThemes[themeOfs]<>255) do	themeOfs:=themeOfs+1; // seek end of name
		themeOfs:=themeOfs+1; // skip name termination byte
		nTheme:=nTheme-1; // decrase seeking theme number
	end;
	move(@colorThemes[themeOfs],@colors,5);
end;

procedure storeTheme(nTheme:shortint; var themeColors:byteArray);
var
	themeOfs:byte;

begin
	currentTheme:=nTheme;
	themeOfs:=0;
	// seek offset of theme number
	while (nTheme>0) do
	begin
		themeOfs:=themeOfs+5; // skip first 5 bytes of color definition
		while (colorThemes[themeOfs]<>255) do	themeOfs:=themeOfs+1; // seek end of name
		themeOfs:=themeOfs+1; // skip name termination byte
		nTheme:=nTheme-1; // decrase seeking theme number
	end;
	move(@themeColors,@colorThemes[themeOfs],5);
end;

procedure setTheme(nTheme:shortint);
var i:byte;
	themeColors:array[0..4] of byte;
	sum:smallint;

begin
	getTheme(nTheme,themeColors);
	sum:=0; for i:=0 to 4 do sum:=sum+themeColors[i];
	if (sum>0) then move(@themeColors,PFCOLS,5);
end;

procedure prepareThemeList();
var
	nTheme,colTabOfs,themeOfs,themeNameLen:byte;
	scrOfs,y:byte;

begin
	fillchar(@DLIColTab,9*4,$0);
	colTabOfs:=0; themeOfs:=0; scrOfs:=41; y:=2; nTheme:=9;
	while (nTheme>0) do
	begin
		fillchar(@screen[scrOfs],5,$89);
		scrOfs:=scrOfs+6; // shift screen pointer to theme name position

		move(@colorThemes[themeOfs],@DLIColTab[colTabOfs],5); // copy theme colors to DLI color table

		themeOfs:=themeOfs+5; // skip first 5 bytes of color definition
		themeNameLen:=0;
		while (colorThemes[themeOfs+themeNameLen]<>255) do // determine theme name length
			themeNameLen:=themeNameLen+1;

		if themeNameLen<=12 then
			move(@colorThemes[themeOfs],@screen[scrOfs],themeNameLen) // put all name to screen
		else
			move(@colorThemes[themeOfs],@screen[scrOfs],12); // put only 12 fisrt character to screen
		colorHLine(6,y,14,1); // set background color for name

		y:=y+1;
		scrOfs:=scrOfs+14; // shift screen pointer o next line (12 bytes of name+2 bytes of blank
		themeOfs:=themeOfs+themeNameLen+1; // next color schema
		colTabOfs:=colTabOfs+5; // next DLI color entry
		nTheme:=nTheme-1;
	end;
end;
