procedure getTheme(nTheme:shortint; var colors:byteArray);
begin
	currentTheme:=nTheme;
	nTheme:=nTheme*5;
	move(@DLIColTab[nTheme],@colors,5);
end;
