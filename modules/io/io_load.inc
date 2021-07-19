procedure LoadData(var filename:string; mode:byte; destId:byte);
(*
 * mode possible values:
 * 0 %000 - read all data (destId is ignored)
 * 1 %001 - read NOTE definition to destId
 * 2 %010 - read SFX definition to destId
 *)

var
	readed,sum:word;

	version:byte;
	nameLen:byte;
	_readNoteTabData:boolean;

	procedure loadMain();
	begin
		IO_clearAllData();

		sum:=0;
		if getByte(version) then exit;
		_readNoteTabData:=version>SFXMM_VER1_0;

		if getByte(nameLen) then exit;
		if (nameLen=SONGNameLength) then
		begin
			if getBlock(SONGTitle[1],nameLen) then exit;
			SONGTitle[0]:=char(nameLen);
		end;
	end;

	procedure loadNoteTab();
	var
		noteTabId:byte;
		dataAddr:word;

	begin
		if getByte(noteTabId) then exit;	// read ID
		if (mode=LOAD_NOTE_DEF) and (destId<255) then noteTabId:=destId;
		if noteTabId>3 then begin IOErr:=error_badFileFormat; exit; end;
//		putValue(17,11,noteTabId,3,0); // debug

		getBlock(IOBuf,NOTETABnameLength+64);				// read data
		if IOErr>3 then exit;

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
			if getByte(id) then exit;	// read ID

		if (isSFX) then
		begin
//			putValue(17,11,id,3,0); // debug
			if id>maxSFXs-1 then begin IOErr:=error_badFileFormat; exit; end;
			if getByte(_sfxMode) then exit;	// read SFX Modulation Type
			if _sfxMode>3 then begin IOErr:=error_badFileFormat; exit; end;
			SFXModMode[id]:=_sfxMode;
			if _readNoteTabData then
			begin
				if getByte(_sfxNoteTabOfs) then exit;	// read SFX Modulation Type
				if _sfxNoteTabOfs>3 then begin IOErr:=error_badFileFormat; exit; end;
				SFXNoteSetOfs[id]:=_sfxNoteTabOfs shl 6;
			end;
		end
		else
		begin
//			putValue(17,11,id-64,3,0); // debug
			if id-64>maxTABs-1 then begin IOErr:=error_badFileFormat; exit; end;
		end;

		if getBlock(dataSize,2) then exit; // read data size
		if dataSize>256+nameLength then begin IOErr:=error_badFileFormat; exit; end;

		getBlock(IOBuf,dataSize);				// read data
		if IOErr>3 then exit;

		dataAddr:=HEAP_TOP;
		HEAP_PTR[id]:=dataAddr+nameLength;	// pointer must indicate direct to definition (without name)
		move(@IOBuf,pointer(dataAddr),dataSize);			// store name and data from IO buffer to HEAP
		HEAP_TOP:=HEAP_TOP+dataSize;

		// store data size in heap_sizes table
		_heap_sizes[id]:=dataSize-nameLength;
	end;

	procedure loadSONG();
	var
		dataSize:word;

	begin
		if getBlock(dataSize,2) then exit; // read data size
		if dataSize>256 then begin IOErr:=error_badFileFormat; exit; end;

		getBlock(IOBuf,dataSize);				// read data
		if IOErr>3 then exit;

		move(@IOBuf,@SONGData,dataSize);
	end;

begin
	if IO_messages then clearStatusBar();

	openFile(filename,file_read);
	while IOErr=IO_OK do
	begin
		getBlock(IOBuf,5); // get tag
		if (IOErr=IO_OK) then
		begin
//			conv2internalP2P(@IOBuf,@statusBar[11],5); // debug
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
								IOErr:=error_badFileFormat;
		end
		else
			IOErr:=error_badFileFormat;
//		move(@screen,@video,240); // put screen buffer to video without wait4screen
	end;
	closeFile();
end;

procedure IOLoad();
begin
	clearModule();
	putMultiText(msg_IO_LoadPrompt,0);

	if IOPrompt(currentFile) then
	begin
		clearModule();
		LoadData(currentFile,LOAD_ALL_DATA,0);
		if (IOErr<>3) then IOError();
	end
end;
