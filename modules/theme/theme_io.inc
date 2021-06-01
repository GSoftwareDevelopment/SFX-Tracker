function IOSaveTheme(var fn:string):byte;
var
	f:file;
	i,ofs,cOfs,nOfs:byte;
	writen:word;

begin
	putMultiText(resptr[msg_IO_Writing],0);
	screen2video();
	{$I-}
	assign(f,fn);
	rewrite(f,1);
	if IOResult=1 then
	begin
		ofs:=0; cOfs:=0; nOfs:=0;
		move(@section_theme,@IOBuf[ofs],5); ofs:=ofs+5;
		IOBuf[ofs]:=SFXMM_VER1_0; ofs:=ofs+1;
		IOBuf[ofs]:=currentTheme; ofs:=ofs+1;
		for i:=0 to 5 do
		begin
			move(@DLIColTab[cOfs],@IOBuf[ofs],5); ofs:=ofs+5;
			move(@ThemesNames[nOfs],@IOBuf[ofs],12); ofs:=ofs+5;
		end;
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
	i,fileVer,cOfs,nOfs,Ofs:byte;

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
				cOfs:=0; nOfs:=0; Ofs:=0;
				for i:=0 to 5 do
				begin
					blockRead(f,DLIColTab[cOfs],5,readed); ofs:=ofs+readed; cOfs:=cOfs+5;
					blockRead(f,ThemesNames[nOfs],12,readed); ofs:=ofs+readed; nOfs:=nOfs+5;
				end;
				if (readed=102) then
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
		{$I+}
	end
	else
		result:=IOError_fileNotFound;
end;
