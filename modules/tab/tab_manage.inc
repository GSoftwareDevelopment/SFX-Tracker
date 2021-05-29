procedure getTABData(nTAB:byte);
var
	size:word;
	ofs:byte;

begin
	// clear local structure of TAB
	fillchar(@TAB_notes,TAB_maxLength,$ff);
	fillchar(@TAB_fnSFX,TAB_maxLength,$fe);

	size:=getData(64+nTAB,TABNameLength,TABName);
	if size>0 then
	begin
		TABLen:=0; // set current TAB length to zero (0)
		ofs:=0;
		repeat
			TAB_fnSFX[TABLen]:=tmpbuf[ofs]; ofs:=ofs+1;
			TAB_notes[TABLen]:=tmpbuf[ofs]; ofs:=ofs+1;
			TABLen:=TABLen+1; size:=size-2;
		until (TABLen=TAB_maxLength) or (size=0);
	end
	else
		TABLen:=TAB_maxLength; // set maximum length od pattfrn definition

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
	tabAdr,ofs,len:word;
	i:byte;

begin
	heap_release(TABPtr[nTAB],_heap_sizes[64+nTAB]);

	// if length of stored TAB is equal zero...
	if (TABLen=0) then
	begin
		TABPtr[nTAB]:=$ffff;	_heap_sizes[64+nTAB]:=0;
		// ... set TAB name as free
		TABName[0]:=char(TABNameLength);
		move(resptr[str_notDefined],@TABName[1],TABNameLength);
//		fillchar(@screen[SFXNameX+vadr[SFXNumberY]],TABNameLength,$40);
//		putText(SFXNameX,SFXNumberY,TABName,1);
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

	tabAdr:=heap_putData(len,TABNameLength,TABName);
	TABPtr[nTAB]:=tabAdr;

	_heap_sizes[64+nTAB]:=TABNameLength+len;
end;
