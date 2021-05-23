function compareTag(var srcTag,dstTag:TTag):boolean;
var
	i:byte;

begin
	i:=0;
	while (i<5) and (srcTag[i]=dstTag[i]) do i:=i+1;
	result:=(i=5);
end;

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
		while (themesDef[themeOfs]<>255) do	themeOfs:=themeOfs+1; // seek end of name
		themeOfs:=themeOfs+1; // skip name termination byte
		nTheme:=nTheme-1; // decrase seeking theme number
	end;
	move(@themesDef[themeOfs],@colors,5);
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
		while (themesDef[themeOfs]<>255) do	themeOfs:=themeOfs+1; // seek end of name
		themeOfs:=themeOfs+1; // skip name termination byte
		nTheme:=nTheme-1; // decrase seeking theme number
	end;
	move(@themeColors,@themesDef[themeOfs],5);
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
