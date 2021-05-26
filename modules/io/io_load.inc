procedure IOLoad();
var
	f:file;
	readed,sum:word;
	err:byte;

	version:byte;
	nameLen:byte;

	procedure loadMain();
	begin
		IO_clearAllData();

		sum:=0;
		blockRead(f,version,1,readed); sum:=sum+readed;
		if (IOResult<>IO_OK) then begin err:=IOResult; exit; end;
		blockRead(f,nameLen,1,readed); sum:=sum+readed;
		if (IOResult<>IO_OK) then begin err:=IOResult; exit; end;
		if (nameLen=SONGNameLength) then
		begin
			SONGTitle[0]:=char(nameLen);
			blockRead(f,SONGTitle[1],nameLen,readed); sum:=sum+readed;
		end;
		if (IOResult<>3) then
			if (IOResult<>IO_OK) then begin err:=IOResult; exit; end;
		if (sum<>2+SONGNameLength) then err:=error_badFileFormat;
	end;

	procedure loadDefinition(isSFX:boolean);
	var
		id,_sfxMode:byte;
		dataSize,HPtr,dataAddr:word;

	begin
		sum:=0;
		blockRead(f,id,1,readed);										// read ID
		if IOResult<>IO_OK then begin err:=IOResult; exit; end;
		putValue(17,1,id,3,0); // debug

		if (isSFX) then
		begin
			if id>maxSFXs-1 then begin err:=error_badFileFormat; exit; end;
			blockRead(f,_sfxMode,1,readed);							// read SFX Modulation Type
			if IOResult<>IO_OK then begin err:=IOResult; exit; end;
			if _sfxMode>3 then begin err:=error_badFileFormat; exit; end;
		end
		else
			if id>maxTABs-1 then begin err:=error_badFileFormat; exit; end;

		blockRead(f,nameLen,1,readed);								// read name length
		if IOResult<>IO_OK then begin err:=IOResult; exit; end;
		if isSFX then
		begin
			if nameLen<>SFXNameLength then begin err:=error_badFileFormat; exit; end;
		end
		else
		begin
			if nameLen<>TABNameLength then begin err:=error_badFileFormat; exit; end;
		end;

		blockRead(f,IOBuf,nameLen,readed);						 	// read name into IO buffer
		if IOResult<>IO_OK then begin err:=IOResult; exit; end;

		blockRead(f,dataSize,2,readed);								// read data size
		if IOResult<>IO_OK then begin err:=IOResult; exit; end;
		if dataSize>256 then begin err:=error_badFileFormat; exit; end;

		blockRead(f,IOBuf[nameLen],dataSize,readed);				// read data
		if IOResult<>3 then
			if IOResult<>IO_OK then begin err:=IOResult; exit; end;

		HPtr:=HEAP_Allocate(dataSize+nameLen);						// allocate mem in heap
		if isSFX then
			SFXPtr[id]:=HPtr												// set SFX entry
		else
			TABPtr[id]:=HPtr;												// set TAB entry
		dataAddr:=HEAP_GetAddr(HPtr);
		move(@IOBuf,@_mem[dataAddr],dataSize+nameLen);			// store name and data from IO buffer to HEAP
	end;

	procedure loadSONG();
	begin
	end;

begin
	box(0,2,20,10,$40);
	if filenamePrompt(resptr[msg_IO_LoadPrompt],1,currentFile)<>-1 then
	begin
		putMultiText(resptr[msg_IO_Reading],0);
		{$I-}
		assign(f,currentFile);
		reset(f,1);
		err:=IOResult;
		while (err=IO_OK) do
		begin
			blockRead(f,IOBuf,5,readed); // get tag
			conv2internalP2P(@IOBuf,@screen[31],5);
			if (readed=5) then
				if (compareTag(IOBuf,section_main)) then
					loadMain()
				else
					if (compareTag(IOBuf,section_SFX)) then
						loadDefinition(true)
					else
						if (compareTag(IOBuf,section_TAB)) then
							loadDefinition(false)
						else
							if (compareTag(IOBuf,section_SONG)) then
								loadSONG()
							else
								err:=error_badSectionTag;
			screen2video(); // debug
			if (IOResult=3) then break;
		end;

		fillchar(screen[20],20,0);
		if (err<>IO_OK) then	IOError(err);

		close(f);
		{$I+}
	end;
end;
