function storeHDataInFile(var f:file; var sectionTAG:TTag; id:byte; dataHPtr:word; nameLength:byte):boolean;
var
	ofs,
	dataSize,
	dataAddr:smallint;

begin
	ofs:=0;
	// prepare SFX section data
	move(@sectionTag,@listBuf,5); inc(ofs,5);
	listBuf[ofs]:=id; inc(ofs);
	dataAddr:=HEAP_GetAddr(dataHPtr);
	if (nameLength>0) then
	begin
		listBuf[ofs]:=nameLength; inc(ofs);
		move(@_mem[dataAddr],@listBuf[ofs],nameLength); inc(ofs,nameLength);
	end;
	dataSize:=HEAP_GetSize(dataHPtr)-nameLength;
	if dataSize>0 then
	begin
		listBuf[ofs]:=lo(dataSize); inc(ofs);
		listBuf[ofs]:=hi(dataSize); inc(ofs);
		dataAddr:=HEAP_GetAddr(dataHPtr)+nameLength;
		move(@_mem[dataAddr],@listBuf[ofs],dataSize); inc(ofs,dataSize);
		BlockWrite(f,listBuf,ofs);
	end;
	result:=IOResult=1;
end;

function makeCompletteFile(var f:file):boolean;
var
	i:byte;

begin
	// prepare main section data
	move(@section_main,@listBuf,5);
	listBuf[5]:=SFXMM_VERSION;
	listBuf[6]:=SONGNameLength;
	move(@SONGName[1],@listBuf[7],SONGNameLength);
	BlockWrite(f,listBuf,5+1+1+SONGNameLength);

	// store SFXs data
	if IOResult<>1 then begin result:=false; exit; end;
	for i:=0 to maxSFXs-1 do
		if SFXPtr[i]<>$FFFF then
			if not storeHDataInFile(f,section_SFX,i,SFXPtr[i],SFXNameLength) then begin result:=false; exit; end;

	// store TABs data
	for i:=0 to maxTABs-1 do
		if TABPtr[i]<>$FFFF then
			if not storeHDataInFile(f,section_TAB,i,TABPtr[i],TABNameLength) then begin result:=false; exit; end;
	result:=true;
end;

procedure IOSave();
var
	f:file;

begin
	if inputLongText(0,11,20,FILEPATHMaxLength,currentFile,0,0) then
	begin
		conv2ASCII(currentFile);
		{$I-}
		Assign(f,currentFile);
		Rewrite(f,1);
		if IOResult=1 then
			makeCompletteFile(f)
		else
			IOError();
		close(f);
		{$I+}
		conv2Internal(currentFile);
	end
end;
