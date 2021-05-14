procedure getSFXData(nSFX:byte);
var
	hptr,i,sfxAdr,size:word;

begin
	hptr:=SFXPtr[nSFX];
	// clear local structure of SFX
	fillchar(@SFX_vol_dist,SFX_maxLength,0);
	fillchar(@SFX_modulate,SFX_maxLength,0);

	// if SFX address is specified...
	if (hptr<>$FFFF) then
	begin
		sfxAdr:=HEAP_GetAddr(hptr);
		// ...get SFX name and determine its length
		move(@_mem[sfxAdr],@SFXName[1],SFXNameLength);
		i:=SFXNameLength; while (i>0) and (SFXName[i]=#0) do i:=i-1;
		SFXName[0]:=char(i); // store detected name length

		sfxLen:=0; // set current SFX length to zero (0)
		sfxAdr:=sfxAdr+SFXNameLength; // move SFX addr to begining of data set
		size:=HEAP_GetSize(SFXPtr[nSFX])-SFXNameLength; // calculate data set size
		// get SFX data to local structure and calculate length
		repeat
			i:=_mem[sfxAdr]; SFX_modulate[sfxLen]:=i; sfxAdr:=sfxAdr+1;
			SFX_vol_dist[sfxLen]:=_mem[sfxAdr]; sfxAdr:=sfxAdr+1;
			sfxLen:=sfxLen+1; size:=size-2;
		until  (sfxLen=SFX_maxLength) or (size=0);
	end
	else
	begin // ...if not, put empty SFX definition
		// set notDefined string to current SFX name
		SFXName[0]:=char(SFXNameLength);
		move(resptr[str_SFX_notDefined],@SFXName[1],SFXNameLength);
		sfxLen:=SFX_maxLength; // set maximum length od SFX definition
//		SFX_modulate[sfxLen-1]:=$80; // store MOD80 at end of SFX definition
	end;

	cursorPos:=0; cursorShift:=0; currentSFX:=nSFX;
end;

procedure SFXDetermineLength();
var v,len:byte;

begin
	len:=0;
	// auto seek end of SFX definition
	repeat
		v:=SFX_modulate[len] and $80;
		len:=len+1;
	until (len=SFX_maxLength) or (v=$80);
(*	if (len=SFX_maxLength) then
	begin
		len:=SFX_maxLength;
		if (SFX_modulate[len-1] and $80=0) then // if last modulator type is not "($8x) stop"...
			SFX_modulate[len-1]:=$80;// ...append "($8x) stop" at the end of SFX definition
	end;*)
	sfxLen:=len;
end;

procedure storeSFXData(nSFX:byte);
var
	sfxAdr:word;
	size:word;
	ofs:word;
	i:byte;
	len:word;

begin
	size:=HEAP_GetSize(SFXPtr[nSFX]);
	HEAP_Release(SFXPtr[nSFX]);

	// if length of stored SFX is equal zero...
	if (sfxLen=0) then
	begin
		// ...
		SFXName[0]:=#16; move(resptr[str_SFX_notDefined],@SFXName[1],SFXNameLength);
		fillchar(@screen[SFXNameX+vadr[SFXNumberY]],SFXNameLength,$40);
		putText(SFXNameX,SFXNumberY,SFXName,1);
		exit;
	end;

	// ...otherwise, prepare data to store in Heap

	ofs:=0;
	for i:=0 to sfxLen-1 do
	begin
		tmpbuf[ofs]:=SFX_modulate[i]; ofs:=ofs+1;
		tmpbuf[ofs]:=SFX_vol_dist[i]; ofs:=ofs+1;
	end;
	len:=sfxLen shl 1; // multiply SFX length by two, becouse, for one entry is 2 bytes info

	// allocate new pointer in heap
	SFXPtr[nSFX]:=HEAP_Allocate(SFXNameLength+len);
	// and get allocated heap address
	sfxAdr:=HEAP_GetAddr(SFXPtr[nSFX]);

	//store name of SFX
	fillchar(@_mem[sfxAdr],SFXNameLength,0);
	move(@SFXName[1],@_mem[sfxAdr],length(SFXName));

	sfxAdr:=sfxAdr+SFXNameLength; // incrase current heap address

	// copy prepared data SFX to the heap
	move(@tmpbuf,@_mem[sfxAdr],len);
end;

procedure prepareSFXsList();
var i:byte;
	hptr,adr,ofs:word;

begin
	ofs:=0;
	for i:=0 to maxSFXs-1 do
	begin
		hptr:=SFXPtr[i];
		if (hptr<>$ffff) then
		begin
			adr:=HEAP_GetAddr(hptr);
			move(@_mem[adr],@listBuf[ofs],SFXNameLength);
		end
		else
			move(resptr[str_SFX_notDefined],@listBuf[ofs],SFXNameLength);
		ofs:=ofs+SFXNameLength;
	end;
end;
