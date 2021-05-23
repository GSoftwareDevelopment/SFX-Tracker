function IOSaveTheme(var fn:string):byte;
var
	f:file;
	ofs,writen:word;

begin
	putMultiText(resptr[msg_IO_Writing],0);
	screen2video();
	{$I-}
	assign(f,fn);
	rewrite(f,1);
	if IOResult=1 then
	begin
		move(@section_theme,@IOBuf[ofs],5); ofs:=ofs+5;
		IOBuf[ofs]:=SFXMM_VER1_0; ofs:=ofs+1;
		IOBuf[ofs]:=currentTheme; ofs:=ofs+1;
		move(@themesDef,@IOBuf[ofs],108); ofs:=ofs+108; // 108=18 bytes per schema * 6 schemas
		blockWrite(f,IOBuf,ofs,writen);
		if (writen=ofs) then	result:=IO_OK;
	end
	else
		result:=IOResult;
	close(f);
	{$I+}
end;

function IOLoadTheme(var fn:string):byte;
var
	f:file;
	readed:word;
	fileVer:byte;

begin
	putMultiText(resptr[msg_IO_Reading],0);
	screen2video();
	if fileExists(fn) then
	begin
		{$I-}
		assign(f,fn);
		reset(f,1);
		blockRead(f,IOBuf,5,readed);
		if (readed=5) and (compareTag(IOBuf,section_theme)) then
		begin
			blockRead(f,fileVer,1);
			if (fileVer=SFXMM_VER1_0) then
			begin
				blockRead(f,currentTheme,1);
				blockRead(f,themesDef,108,readed); // 18 bytes per schema * 6 schemas
				if (readed=108) then
				begin
					setTheme(currentTheme);
					result:=IO_OK;
				end
				else
					result:=IOResult;
			end;
		end
		else
			result:=error_notThemeFormat;
		close(f);
		{$I+}
	end
	else
		result:=IOError_fileNotFound;
end;
