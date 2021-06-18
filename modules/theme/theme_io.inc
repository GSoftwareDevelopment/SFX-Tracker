function IOSaveTheme(var fn:string):byte;
var
	writen:word;

begin
	result:=openFile(fn,file_write);
	if result=IO_OK then
	begin
		move(@section_theme,@IOBuf,5);
		IOBuf[5]:=SFXMM_VER1_0;
		IOBuf[6]:=currentTheme;
		move(@DLIColTab,@IOBuf[7],30);
		move(@themesNames,@IOBuf[37],72);
		blockWrite(f,IOBuf,109,writen);
		if (writen=109) then	result:=IO_OK;
		closeFile();
	end
	else
		result:=IOResult;
end;

function IOLoadTheme(var fn:string):byte;
var
	readed:word;
	fileVer:byte;

begin
	result:=openFile(fn,file_read);
	if result=IO_OK then
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
					getTheme(currentTheme,PFCOLS);
					result:=IO_OK;
				end
				else
					result:=IOResult;
			end;
		end
		else
			result:=error_badSectionTag;
		closeFile();
	end
end;

procedure Theme_IO_Load();
var err:byte;

begin
	if IOPrompt(currentFile) then
	begin
		err:=IOLoadTheme(currentFile);
		if err<>IO_OK then IOError(err);
	end;
end;
