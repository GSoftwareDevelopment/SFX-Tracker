var
	IOOfs:word;

procedure push2IOBuf(data:pointer; size:word);
begin
	move(data,@IOBuf[IOOfs],size); inc(IOOfs,size);
end;

function storeHDataInFile(var f:file; var section:TTag; nameLength,id:byte):boolean;
var
	dataSize,
	dataAddr:word;
	noteTabId:byte;

begin
	IOOfs:=0;
	push2IOBuf(section,5);
	push2IOBuf(@id,1);

	if (id<maxSFXs) then // only for SFX!
	begin
		push2IOBuf(@SFXModMode[id],1);
		noteTabId:=SFXNoteSetOfs[id] shr 6;
		push2IOBuf(@noteTabId,1);
	end;

	dataAddr:=HEAP_PTR[id];	// get data address
	dataSize:=_heap_sizes[id];
	push2IOBuf(@dataSize,2);
	push2IOBuf(@_mem[dataAddr],dataSize);

// write to disk
	BlockWrite(f,IOBuf,IOOfs);
	result:=IOResult=1;
end;

function storeNOTEDataInFile(var f:file; noteTabId:byte):boolean;
var
	addr:word;

begin
	IOOfs:=0;
	push2IOBuf(@section_NOTE,5);
	push2IOBuf(@noteTabId,1);

	addr:=word(resptr[menu_NoteDef])+4+noteTabId*16;
	push2IOBuf(pointer(addr),NOTETABnameLength);
	addr:=NOTE_TABLE_ADDR+noteTabId*64;
	push2IOBuf(pointer(addr),64);

// write to disk
	BlockWrite(f,IOBuf,IOOfs);
	result:=IOResult=1;
end;

function storeSONGDataInFile(var f:file):boolean;
var
	dataSize:word;

begin
	IOofs:=0;
	push2IOBuf(@section_SONG,5);
	dataSize:=$100;
	push2IOBuf(@dataSize,2);
	push2IOBuf(@SONGData,dataSize);

// write to disk
	BlockWrite(f,IOBuf,IOOfs);
	result:=IOResult=1;
end;

function makeCompletteFile(var f:file):boolean;
var
	i:byte;

begin
	// prepare main section data
	move(@section_main,@IOBuf,5);
	IOBuf[5]:=SFXMM_VER1_1;
	IOBuf[6]:=SONGNameLength;
	move(@SONGTitle[1],@IOBuf[7],SONGNameLength);
	BlockWrite(f,IOBuf,5+1+1+SONGNameLength);

	if IOResult<>1 then exit(false);

	// store SONG data
	if not storeSONGDataInFile(f) then exit(false);

	// store NOTE TABLE data
	for i:=0 to 3 do
		if not storeNOTEDataInFile(f,i) then exit(false);

	// store SFXs data
	for i:=0 to maxSFXs-1 do
		if SFXPtr[i]<>$FFFF then
			if not storeHDataInFile(f,section_SFX,SFXNameLength,i) then exit(false);

	// store TABs data
	for i:=0 to maxTABs-1 do
		if TABPtr[i]<>$FFFF then
			if not storeHDataInFile(f,section_TAB,TABNameLength,64+i) then exit(false);

	result:=true;
end;

procedure saveSong();
begin
	SFX_End();
	putMultiText(resptr[msg_IO_writing],0);
	{$I-}
	assign(f,currentFile);
	rewrite(f,1);
	if (IOResult=1) then
	begin
		if (not makeCompletteFile(f)) then
			IOError(IOResult);
	end
	else
		IOError(IOResult);
	close(f);
	{$I+}
	fillchar(screen[20],20,0);
	SFX_Start();
end;

procedure IOSave();
begin
	move(@screen,@tmpbuf,240);
	fillchar(@moduleBar,20,0);
	fillchar(@moduleBar[20],200,$40);
	putMultiText(resptr[msg_IO_SavePrompt],1);
	if IOPrompt(currentFile) then
	begin
		fillchar(@moduleBar,220,0);
		saveSong();
	end;
	move(@tmpbuf,@screen,240);
end;
