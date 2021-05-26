function storeHDataInFile(var f:file; isSFX:boolean; id:byte):boolean;
var
	ofs,
	dataSize,
	dataAddr,
	dataHPtr:word;
	nameLength:byte;

begin
	ofs:=0;
	if (isSFX) then
	begin
		move(@section_SFX,@IOBuf,5);	// store section tag
		dataHPtr:=SFXPtr[id];			// get data about SFX
		nameLength:=SFXNameLength;
	end
	else
	begin
		move(@section_TAB,@IOBuf,5);	// store section tag
		dataHPtr:=TABPtr[id];			// get data about TAG
		nameLength:=TABNameLength;
	end;

	inc(ofs,5);

	IOBuf[ofs]:=id; inc(ofs);			// store id for SFX/TAG

	if (isSFX) then // only for SFX!
	begin
		IOBuf[ofs]:=SFXModMode[id]; inc(ofs); // store SFX Mode
	end;

	dataAddr:=HEAP_GetAddr(dataHPtr);

	if (nameLength>0) then	// store name
	begin
		IOBuf[ofs]:=nameLength; inc(ofs);
		move(@_mem[dataAddr],@IOBuf[ofs],nameLength); inc(ofs,nameLength);
	end;
	dataSize:=HEAP_GetSize(dataHPtr)-nameLength;
	if dataSize>0 then
	begin
		IOBuf[ofs]:=lo(dataSize); inc(ofs); // store data size
		IOBuf[ofs]:=hi(dataSize); inc(ofs);
		dataAddr:=HEAP_GetAddr(dataHPtr)+nameLength;
		move(@_mem[dataAddr],@IOBuf[ofs],dataSize); inc(ofs,dataSize); // store data from heap
		BlockWrite(f,IOBuf,ofs); // write to disk
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
			if not storeHDataInFile(f,true,i) then exit(false);

	// store TABs data
	for i:=0 to maxTABs-1 do
		if TABPtr[i]<>$FFFF then
			if not storeHDataInFile(f,false,i) then exit(false);

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
