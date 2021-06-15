function IOSaveTheme(var fn:string):byte;
var
	writen:word;

begin
	putMultiText(resptr[msg_IO_Writing],0);
	screen2video();
	{$I-}
	assign(f,fn);
	rewrite(f,1);
	if IOResult=1 then
	begin
		move(@section_theme,@IOBuf,5);
		IOBuf[5]:=SFXMM_VER1_0;
		IOBuf[6]:=currentTheme;
		move(@DLIColTab,@IOBuf[7],30);
		move(@ThemesNames,@IOBuf[37],72);
		blockWrite(f,IOBuf,109,writen);
		if (writen=109) then	result:=IO_OK;
	end
	else
		result:=IOResult;
	close(f);
	{$I+}
end;

function IOLoadTheme(var fn:string):byte;
var
	readed,count:word;
	fileVer:byte;

begin
	putMultiText(resptr[msg_IO_Reading],0);
	screen2video();
	(* $54a6 *)
	{$I-}
	if fileExists(fn) then
	begin
		assign(f,fn);
		reset(f,1);
		blockRead(f,IOBuf,5,readed);
		if (readed=5) and (compareTag(IOBuf,section_theme)) then
		begin
			blockRead(f,fileVer,1);
			if (fileVer=SFXMM_VER1_0) then
			begin
				blockRead(f,currentTheme,1);
				blockRead(f,DLIColTab,30);
				blockRead(f,ThemesNames,72);
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
		close(f);
	end
	else
		result:=IOError_fileNotFound;
	{$I+}
end;
