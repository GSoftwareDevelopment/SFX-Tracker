function storeHDataInFile(var f:file; var section:TTag; nameLength,id:byte):boolean;
var
	ofs,
	dataSize,
	dataAddr:word;

begin
	ofs:=0;
	move(@section,@IOBuf,5);	// store section tag
	inc(ofs,5);
	IOBuf[ofs]:=id; inc(ofs);			// store id for SFX/TAG

	if (id<maxSFXs) then // only for SFX!
	begin
		IOBuf[ofs]:=SFXModMode[id]; inc(ofs); // store SFX Mode
	end;

	dataAddr:=HEAP_PTR[id];	// get data address
	dataSize:=_heap_sizes[id];
	IOBuf[ofs]:=lo(dataSize); inc(ofs); // store data size
	IOBuf[ofs]:=hi(dataSize); inc(ofs);
	move(@_mem[dataAddr],@IOBuf[ofs],dataSize); inc(ofs,dataSize); // store data from heap

// write to disk
	BlockWrite(f,IOBuf,ofs);
	result:=IOResult=1;
end;

function storeSONGDataInFile(var f:file):boolean;
var
	ofs:word;
	SONGOfs:byte;

begin
	ofs:=0;
	move(@section_SONG,@IOBuf[ofs],5); inc(ofs,5);
	IOBuf[ofs]:=0; inc(ofs); // store data size
	IOBuf[ofs]:=1; inc(ofs);
	move(@SONGData,@IOBuf[ofs],256); inc(ofs,256);

// write to disk
	BlockWrite(f,IOBuf,ofs);
	result:=IOResult=1;
end;

function makeCompletteFile(var f:file):boolean;
var
	i:byte;

begin
	// prepare main section data
	move(@section_main,@IOBuf,5);
	IOBuf[5]:=SFXMM_VER1_0;
	IOBuf[6]:=SONGNameLength;
	move(@SONGTitle[1],@IOBuf[7],SONGNameLength);
	BlockWrite(f,IOBuf,5+1+1+SONGNameLength);

	if IOResult<>1 then exit(false);
	// store SONG data
	if not storeSONGDataInFile(f) then exit(false);

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

procedure IOSave();
var
	f:file;

begin
	box(0,2,20,10,$40);
	if filenamePrompt(resptr[msg_IO_SavePrompt],1,currentFile)<>-1 then
	begin
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
	end;
end;
