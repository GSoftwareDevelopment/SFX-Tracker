procedure getTheme(nTheme:shortint; var colors:byteArray);
var
	themeOfs:byte;

begin
	currentTheme:=nTheme;
	themeOfs:=nTheme*5;
	move(@DLIColTab[themeOfs],@colors,5);
end;
