procedure getTABData(nTAB:byte);
var
	hptr,tabAdr,size:word;
	i:byte;

begin
	hptr:=TABPtr[nTAB];
	// clear local structure of TAB
	fillchar(@TAB_notes,TAB_maxLength,$ff);
	fillchar(@TAB_fnSFX,TAB_maxLength,$fe);
	// clear current TAB name
	//fillchar(@TABName,TABNameLength+1,0);

	// if TAB address is specified...
	if (hptr<>$FFFF) then
	begin
		tabAdr:=HEAP_GetAddr(hptr);
		// ...get TAB name and determine its length
		move(@_mem[tabAdr],@TABName[1],TABNameLength);
		i:=TABNameLength; while (i>0) and (TABName[i]=#0) do i:=i-1;
		TABName[0]:=char(i); // store detected name length

		TABLen:=0; // set current TAB length to zero (0)
		tabAdr:=tabAdr+TABNameLength; // move TAB addr to begining of data set
		size:=HEAP_GetSize(TABPtr[nTAB])-TABNameLength; // calculate data set size
		// get TAB data to local structure and calculate length
		repeat
			i:=_mem[tabAdr]; TAB_fnSFX[TABLen]:=i; tabAdr:=tabAdr+1;
			TAB_notes[TABLen]:=_mem[tabAdr]; tabAdr:=tabAdr+1;
			TABLen:=TABLen+1; size:=size-2;
		until  (TABLen=TAB_maxLength) or (size=0);
	end
	else
	begin // ...if not
		// set notDefined string to current TAB name
		TABName[0]:=char(TABNameLength);
		move(resptr[str_TAB_types],@TABName[1],TABNameLength);
		TABLen:=TAB_maxLength; // set maximum length od pattfrn definition
	end;

	cursorPos:=0; cursorShift:=0; currentTAB:=nTAB;
end;

procedure TABDetermineLength();
var v,len:byte;

begin
	len:=0;
	// auto seek end of TAB definition
	repeat
		v:=TAB_fnSFX[len];
		len:=len+1;
	until (len=TAB_maxLength) or (v=$FF);
	TABLen:=len;
end;

procedure storeTABData(nTAB:byte);
var
	tabAdr,size,ofs,len:word;
	i:byte;

begin
	size:=HEAP_GetSize(TABPtr[nTAB]);
	HEAP_Release(TABPtr[nTAB]);

	// if length of stored TAB is equal zero...
	if (TABLen=0) then
	begin
		// ... set TAB name as free
		TABName[0]:=#16;
		move(resptr[str_TAB_types],@TABName[1],TABNameLength);
		fillchar(@screen[SFXNameX+vadr[SFXNumberY]],TABNameLength,$40);
		putText(SFXNameX,SFXNumberY,TABName,1);
		exit;
	end;

	// ...otherwise, prepare data to store in heap

	ofs:=0;
	for i:=0 to TABLen-1 do
	begin
		tmpbuf[ofs]:=TAB_fnSFX[i]; ofs:=ofs+1;
		tmpbuf[ofs]:=TAB_notes[i]; ofs:=ofs+1;
	end;
	len:=TABLen shl 1; // multiply TAB length by two, becouse, for one entry is 2 bytes info

	// allocate new pointer in heap
	TABPtr[nTAB]:=HEAP_Allocate(TABNameLength+len);
	// and get allocated heap address
	tabAdr:=HEAP_GetAddr(TABPtr[nTAB]);

	//store name of TAB
	fillchar(@_mem[tabAdr],TABNameLength,0);
	move(@TABName[1],@_mem[tabAdr],length(TABName));

	tabAdr:=tabAdr+TABNameLength; // incrase current heap address

	// copy prepared data TAB to the heap
	move(@tmpbuf,@_mem[tabAdr],len);
end;

procedure prepareTABsList();
var i:byte;
	hptr,adr,ofs:word;

begin
	ofs:=0;
	fillchar(@listBuf,maxSFXs*SFXNameLength,0);
	for i:=0 to maxTABs-1 do
	begin
		hptr:=TABPtr[i];
		if (hptr<>$ffff) then
		begin
			adr:=HEAP_GetAddr(hptr);
			conv2internalP2P(@_mem[adr],@listBuf[ofs],TABNameLength);
		end
		else
			conv2internalP2P(resptr[str_TAB_types],@listBuf[ofs],TABNameLength);

		ofs:=ofs+TABNameLength;
	end;
end;
