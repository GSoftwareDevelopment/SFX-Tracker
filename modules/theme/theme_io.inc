function IOSaveTheme(var fn:string):byte;
var
	writen:word;

begin
	if openFile(fn,file_write) then
	begin
		move(@section_theme,@IOBuf,5);
		IOBuf[5]:=SFXMM_VER1_0;
		IOBuf[6]:=currentTheme;
		move(@DLIColTab,@IOBuf[7],102);
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
	if fileExists(fn) then
	begin
		if openFile(fn,file_read) then
		begin
			blockRead(f,IOBuf,5,readed);
			if (readed=5) and (compareTag(IOBuf,section_theme)) then
			begin
				blockRead(f,fileVer,1);
				if (fileVer=SFXMM_VER1_0) then
				begin
					blockRead(f,currentTheme,1);
					blockRead(f,ThemesNames,102,readed);
					if (readed=72) then
					begin
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
		end;
	end
	else
		result:=IOError_fileNotFound;
end;
