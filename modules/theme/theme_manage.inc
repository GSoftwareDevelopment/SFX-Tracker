procedure getTheme(nTheme:shortint; var colors:array[0..4] of byte);
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

procedure storeTheme(nTheme:shortint; var themeColors:array[0..4] of byte);
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
