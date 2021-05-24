function storeHDataInFile(var f:file; var sectionTAG:TTag; id:byte; dataHPtr:word; nameLength:byte):boolean;
var
	ofs,
	dataSize,
	dataAddr:smallint;

begin
	ofs:=0;
	// prepare SFX section data
	move(@sectionTag,@IOBuf,5); inc(ofs,5);
	IOBuf[ofs]:=id; inc(ofs);
	dataAddr:=HEAP_GetAddr(dataHPtr);
	if (nameLength>0) then
	begin
		IOBuf[ofs]:=nameLength; inc(ofs);
		move(@_mem[dataAddr],@IOBuf[ofs],nameLength); inc(ofs,nameLength);
	end;
	dataSize:=HEAP_GetSize(dataHPtr)-nameLength;
	if dataSize>0 then
	begin
		IOBuf[ofs]:=lo(dataSize); inc(ofs);
		IOBuf[ofs]:=hi(dataSize); inc(ofs);
		dataAddr:=HEAP_GetAddr(dataHPtr)+nameLength;
		move(@_mem[dataAddr],@IOBuf[ofs],dataSize); inc(ofs,dataSize);
		BlockWrite(f,IOBuf,ofs);
	end;
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

	// store SFXs data
	if IOResult<>1 then exit(false);
	for i:=0 to maxSFXs-1 do
		if SFXPtr[i]<>$FFFF then
			if not storeHDataInFile(f,section_SFX,i,SFXPtr[i],SFXNameLength) then exit(false);

	// store TABs data
	for i:=0 to maxTABs-1 do
		if TABPtr[i]<>$FFFF then
			if not storeHDataInFile(f,section_TAB,i,TABPtr[i],TABNameLength) then exit(false);

	result:=true;
end;

procedure IOSave();
var
	f:file;

begin
	box(0,2,20,10,$40);
	if filenamePrompt(resptr[msg_IO_SavePrompt],1,currentFile)<>-1 then
	begin
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
	end;
end;
