procedure IOSaveTheme(var fn:string);
var
	tmp:byte;

begin
	openFile(fn,file_write);
	if IOErr=IO_OK then
	begin
		IOOfs:=0;
		push2IOBuf(@section_theme,5);
		tmp:=sfxmm_ver1_0; push2IOBuf(@tmp,1);
		push2IOBuf(@currentTheme,1);
		push2IOBuf(@DLIColtab,30);
		push2IOBuf(@themesNames,72);
		putIOBuf();
		closeFile();
	end;
	if IOErr<>IO_OK then IOError();
end;

procedure IOLoadTheme(var fn:string);
var
	fileVer:byte;

begin
	openFile(fn,file_read);
	if IOErr=IO_OK then
	begin
		getBlock(IOBuf,5);
		if (IOErr=IO_OK) and (compareTag(IOBuf,section_theme)) then
		begin
			getByte(fileVer);
			if (fileVer=SFXMM_VER1_0) then
			begin
				getByte(currentTheme);
				getBlock(IOBuf,102);
				if (IOErr=IO_EOF) then
				begin
					move(@IOBuf,@DLIColtab,30);
					move(@IOBuf[30],@themesNames,72);
				end;
			end;
		end
		else
			IOErr:=error_badSectionTag;
		closeFile();
	end
end;

function IOLoadDefaultTheme():boolean;
begin
	IOLoadTheme(defaultThemeFile);
	result:=(IOErr=IO_OK);
end;

procedure Theme_IO_Load();
begin
	if IOPrompt(otherFile) then
	begin
		IOLoadTheme(otherFile);
		if IOErr<>IO_OK then IOError();
	end;
end;
