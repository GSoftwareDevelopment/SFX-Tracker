function getBlock(var f:file; var dest; size:word):boolean;
begin
	blockRead(f,dest,size);
	result:=IOResult<>IO_OK;
end;


procedure LoadData(mode:byte; destId:byte);
(*
 * mode possible values:
 * 0 %000 - read all data (destId is ignored)
 * 1 %001 - read NOTE definition to destId
 * 2 %010 - read SFX definition to destId
 *)

var
	readed,sum:word;
	err:byte;

	version:byte;
	nameLen:byte;
	_readNoteTabData:boolean;

	procedure loadMain();
	begin
		IO_clearAllData();

		sum:=0;
		if getBlock(f,version,1) then begin err:=IOResult; exit; end;
		_readNoteTabData:=version>SFXMM_VER1_0;

		if getBlock(f,nameLen,1) then begin err:=IOResult; exit; end;
		if (nameLen=SONGNameLength) then
		begin
			SONGTitle[0]:=char(nameLen);
			blockRead(f,SONGTitle[1],nameLen,readed);
		end;
		if (IOResult<>3) then
			if (IOResult<>IO_OK) then begin err:=IOResult; exit; end;
	end;

	procedure loadNoteTab();
	var
		noteTabId:byte;
		dataAddr:word;

	begin
		if getBlock(f,noteTabId,1) then begin err:=IOResult; exit; end;	// read ID
		if (mode=LOAD_NOTE_DEF) and (destId<255) then noteTabId:=destId;
		if noteTabId>3 then begin err:=error_badFileFormat; exit end;
		putValue(17,11,noteTabId,3,0); // debug

		blockRead(f,IOBuf,NOTETABnameLength+64);				// read data
		if IOResult<>3 then
			if IOResult<>IO_OK then begin err:=IOResult; exit; end;

		dataAddr:=word(resptr[menu_NoteDef])+4+noteTabId*16;
		move(@IOBuf,pointer(dataAddr),NOTETABnameLength);
		dataAddr:=NOTE_TABLE_ADDR+noteTabId*64;
		move(@IOBuf[NOTETABnameLength],pointer(dataAddr),64);
	end;

	procedure loadDefinition(isSFX:boolean; nameLength:byte);
	var
		id,_sfxMode,_sfxNoteTabOfs:byte;
		dataSize,dataAddr:word;

	begin
		sum:=0;
		if (mode=LOAD_SFX_DEF) then
			id:=destId
		else
			if getBlock(f,id,1) then begin err:=IOResult; exit; end;	// read ID

		if (isSFX) then
		begin
			putValue(17,11,id,3,0); // debug
			if id>maxSFXs-1 then begin err:=error_badFileFormat; exit; end;
			if getBlock(f,_sfxMode,1) then begin err:=IOResult; exit; end;	// read SFX Modulation Type
			if _sfxMode>3 then begin err:=error_badFileFormat; exit; end;
			SFXModMode[id]:=_sfxMode;
			if _readNoteTabData then
			begin
				if getBlock(f,_sfxNoteTabOfs,1) then begin err:=IOResult; exit; end;	// read SFX Modulation Type
				if _sfxNoteTabOfs>3 then begin err:=error_badFileFormat; exit; end;
				SFXNoteSetOfs[id]:=_sfxNoteTabOfs shl 6;
			end;
		end
		else
		begin
			putValue(17,11,id-64,3,0); // debug
			if id-64>maxTABs-1 then begin err:=error_badFileFormat; exit; end;
		end;

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
	var
		dataSize:word;

	begin
		if getBlock(f,dataSize,2) then begin err:=IOResult; exit; end; // read data size
		if dataSize>256 then begin err:=error_badFileFormat; exit; end;

		blockRead(f,IOBuf,dataSize);				// read data
		if IOResult<>3 then
			if IOResult<>IO_OK then begin err:=IOResult; exit; end;

		move(@IOBuf,@SONGData,dataSize);
	end;

begin
	fillchar(@statusBar,20,0);

	if openFile(currentFile,file_read) then
	begin
		err:=IO_OK;
		repeat
			blockRead(f,IOBuf,5,readed); // get tag
			if (readed=5) then
			begin
				conv2internalP2P(@IOBuf,@statusBar[11],5);
				if (mode=LOAD_ALL_DATA) and (compareTag(IOBuf,section_main)) then
					loadMain()
				else
					if ((mode=LOAD_ALL_DATA) or (mode=LOAD_NOTE_DEF)) and (compareTag(IOBuf,section_NOTE)) then
						loadNoteTab()
					else
						if ((mode=LOAD_ALL_DATA) or (mode=LOAD_NOTE_DEF)) and (compareTag(IOBuf,section_SFX)) then
							loadDefinition(true,SFXNameLength)
						else
							if (mode=LOAD_ALL_DATA) and (compareTag(IOBuf,section_TAB)) then
								loadDefinition(false,TABNameLength)
							else
								if (mode=LOAD_ALL_DATA) and (compareTag(IOBuf,section_SONG)) then
									loadSONG()
								else
									err:=error_badSectionTag;
			end
			else
				err:=error_badFileFormat;
			move(@screen,@video,240); // put screen buffer to video without wait4screen
		until (err<>IO_OK) or (IOResult=3);
		fillchar(@statusBar,20,0);
		if (err<>IO_OK) then	IOError(err);
		closeFile();
	end
	else
		IOError(IOResult);
end;

procedure IOLoad();
begin
	move(@screen,@tmpbuf,240);
	fillchar(@moduleBar,20,0);
	fillchar(@moduleBar[20],200,$40);
	putMultiText(resptr[msg_IO_LoadPrompt],1);

	if IOPrompt(currentFile) then
	begin
		fillchar(@moduleBar,200,0);
		LoadData(LOAD_ALL_DATA,0);
	end;
	move(@tmpbuf,@screen,240);
end;
