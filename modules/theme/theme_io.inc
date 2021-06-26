procedure IOSaveTheme(var fn:string);
var
	writen:word;

begin
	openFile(fn,file_write);
	if IOErr=IO_OK then
	begin
		move(@section_theme,@IOBuf,5);
		IOBuf[5]:=SFXMM_VER1_0;
		IOBuf[6]:=currentTheme;
		move(@DLIColTab,@IOBuf[7],30);
		move(@themesNames,@IOBuf[37],72);
		blockWrite(f,IOBuf,109,writen);
		if (writen=109) then	IOErr:=IO_OK;
		closeFile();
	end;
	if IOErr<>IO_OK then IOError();
end;

procedure IOLoadTheme(var fn:string);
var
	readed:word;
	fileVer:byte;

begin
	openFile(fn,file_read);
	if IOErr=IO_OK then
	begin
		blockRead(f,IOBuf,5,readed);
		if (readed=5) and (compareTag(IOBuf,section_theme)) then
		begin
			blockRead(f,fileVer,1);
			if (fileVer=SFXMM_VER1_0) then
			begin
				blockRead(f,currentTheme,1);
				blockRead(f,IOBuf,102,readed);
				if (readed=102) then
				begin
					move(@IOBuf,@DLIColtab,30);
					move(@IOBuf[30],@themesNames,72);
					IOErr:=IO_OK;
				end
				else
					IOErr:=IOResult;
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
