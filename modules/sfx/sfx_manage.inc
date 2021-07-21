procedure getSFXData(nSFX:byte);
var
   size:word;
   ofs:byte;

begin
   // clear local structure of SFX
	ClearEditBufs(true);

   size:=heap_getData(nSFX,SFXNameLength,SFXName);
   if size>0 then
   begin
      sfxLen:=0;
      ofs:=0;
      repeat
         SFX_modulate[sfxLen]:=heapStoreBuf[ofs]; ofs:=ofs+1;
         SFX_vol_dist[sfxLen]:=heapStoreBuf[ofs]; ofs:=ofs+1;
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
         if ((_modMode=0) and (v=$80)) then break;
         if ((_modMode>0) and (v and $80=$80)) then break;
      until (len=SFX_maxLength);
      sfxLen:=len;
   end
   else
      sfxLen:=SFX_maxLength
end;

procedure storeSFXData(nSFX:byte);
var
   size:word;
   ofs,i:byte;

begin
   heap_release(nSFX,SFXNameLength);

   // if length of stored SFX is equal zero...
   if (sfxLen=0) then
   begin
      // set current name to "Not defined"
      SFXName[0]:=char(SFXNameLength);
      move(resptr[str_notDefined],@SFXName[1],SFXNameLength);
      exit;
   end;

   // ...otherwise, prepare data to store in Heap
   ofs:=0;
   for i:=0 to sfxLen-1 do
   begin
      heapStoreBuf[ofs]:=SFX_modulate[i]; ofs:=ofs+1;
      heapStoreBuf[ofs]:=SFX_vol_dist[i]; ofs:=ofs+1;
   end;
   size:=sfxLen shl 1; // multiply SFX length by two, becouse, for one entry is 2 bytes info

   heap_putData(nSFX,size,SFXNameLength,SFXName);
end;
