function getBlock(var f:file; var dest; size:word):boolean;
begin
	blockRead(f,dest,size);
	result:=IOResult<>IO_OK;
end;

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
		if getBlock(f,version,1) then begin err:=IOResult; exit; end;
		// sum:=sum+readed;
		if getBlock(f,nameLen,1) then begin err:=IOResult; exit; end;
		// sum:=sum+readed;
		if (nameLen=SONGNameLength) then
		begin
			SONGTitle[0]:=char(nameLen);
			blockRead(f,SONGTitle[1],nameLen,readed);
			// sum:=sum+readed;
		end;
		if (IOResult<>3) then
			if (IOResult<>IO_OK) then begin err:=IOResult; exit; end;
//		if (sum<>2+SONGNameLength) then err:=error_badFileFormat;
	end;

	procedure loadDefinition(isSFX:boolean; nameLength:byte);
	var
		id,_sfxMode:byte;
		dataSize,dataAddr:word;

	begin
		sum:=0;
		if getBlock(f,id,1) then begin err:=IOResult; exit; end;	// read ID
		putValue(17,1,id,3,0); // debug

		if (isSFX) then
		begin
			if id>maxSFXs-1 then begin err:=error_badFileFormat; exit; end;
			if getBlock(f,_sfxMode,1) then begin err:=IOResult; exit; end;	// read SFX Modulation Type
			if _sfxMode>3 then begin err:=error_badFileFormat; exit; end;
			SFXModMode[id]:=_sfxMode;
		end
		else
			if id-64>maxTABs-1 then begin err:=error_badFileFormat; exit; end;

		if getBlock(f,dataSize,2) then begin err:=IOResult; exit; end; // read data size
		if dataSize>256+nameLength then begin err:=error_badFileFormat; exit; end;

		blockRead(f,IOBuf,dataSize);				// read data
		if IOResult<>3 then
			if IOResult<>IO_OK then begin err:=IOResult; exit; end;

		dataAddr:=HEAP_TOP;
		HEAP_PTR[id]:=dataAddr;
		move(@IOBuf,@_mem[dataAddr],dataSize);			// store name and data from IO buffer to HEAP
		HEAP_TOP:=HEAP_TOP+dataSize;

		// store data size in heap_sizes table
		_heap_sizes[id]:=dataSize;
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
						loadDefinition(true,SFXNameLength)
					else
						if (compareTag(IOBuf,section_TAB)) then
							loadDefinition(false,TABNameLength)
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
