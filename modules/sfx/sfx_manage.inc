procedure getSFXData(nSFX:byte);
var
	size:word;
	ofs:byte;

begin
	// clear local structure of SFX
	fillchar(@SFX_vol_dist,SFX_maxLength,0);
	fillchar(@SFX_modulate,SFX_maxLength,0);

	size:=getData(nSFX,SFXNameLength,SFXName);
	if size>0 then
	begin
		sfxLen:=0;
		ofs:=0;
		repeat
			SFX_modulate[sfxLen]:=tmpbuf[ofs]; ofs:=ofs+1;
			SFX_vol_dist[sfxLen]:=tmpbuf[ofs]; ofs:=ofs+1;
			sfxLen:=sfxLen+1; size:=size-2;
		until  (sfxLen=SFX_maxLength) or (size=0);
	end
	else
		sfxLen:=SFX_maxLength; // set maximum length od SFX definition

	cursorPos:=0; cursorShift:=0; currentSFX:=nSFX;
end;

procedure SFXDetermineLength();
var v,len,_modMode:byte;

begin
	len:=0; _modMode:=SFXModMode[currentSFX];
	// auto seek end of SFX definition
	if _modMode<3 then
	begin
		repeat
			v:=SFX_modulate[len];
			len:=len+1;
			if ((_modMode=0) and (v=$80)) or
				((_modMode<3) and (v and $80=$80)) then break;
		until (len=SFX_maxLength);
		sfxLen:=len;
	end
	else
		sfxLen:=SFX_maxLength
end;

procedure storeSFXData(nSFX:byte);
var
	sfxAdr,ofs,len:word;
	i:byte;

begin
	heap_release(SFXPtr[nSFX],_heap_sizes[nSFX]);

	// if length of stored SFX is equal zero...
	if (sfxLen=0) then
	begin
		SFXPtr[nSFX]:=$ffff;	_heap_sizes[nSFX]:=0;

		// set current name to "Not defined"
		SFXName[0]:=char(SFXNameLength);
		move(resptr[str_notDefined],@SFXName[1],SFXNameLength);
//		fillchar(@screen[SFXNameX+vadr[SFXNumberY]],SFXNameLength,$40);
//		putText(SFXNameX,SFXNumberY,SFXName,1);
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

	sfxAdr:=heap_putData(len,SFXNameLength,SFXName);
	SFXPtr[nSFX]:=sfxAdr;

	_heap_sizes[nSFX]:=SFXNameLength+len;
end;
