procedure TABputIn(_pos:byte; _note,_fn:byte);
begin
   TAB_notes[_pos]:=_note;
   TAB_fnSFX[_pos]:=_fn;
   modified:=true;
end;

procedure TABdblMove(_pos,ofsSrc,ofsDest:byte);
var len:byte;

begin
   len:=127-_pos;
   ofsSrc:=_pos+ofsSrc; ofsDest:=_pos+ofsdest;
   move(@TAB_notes[ofsSrc],@TAB_notes[ofsDest],len);
   move(@TAB_fnSFX[ofsSrc],@TAB_fnsfx[ofsDest],len);
end;

procedure getTABData(nTAB:byte);
var
   size:word;
   ofs:byte;

begin
   // clear local structure of TAB
   fillchar(@TAB_notes,TAB_maxLength,$00);
   fillchar(@TAB_fnSFX,TAB_maxLength,$c0);

   size:=heap_getData(64+nTAB,TABNameLength,TABName);
   if size>0 then
   begin
      TABLen:=0; // set current TAB length to zero (0)
      ofs:=0;
      repeat
         TAB_notes[TABLen]:=tmpbuf[ofs]; ofs:=ofs+1;
         TAB_fnSFX[TABLen]:=tmpbuf[ofs]; ofs:=ofs+1;
         TABLen:=TABLen+1; size:=size-2;
      until (TABLen=TAB_maxLength) or (size=0);
   end
   else
      TABLen:=TAB_maxLength; // set maximum length od pattfrn definition

   currentTAB:=nTAB;
end;

function TABEmptyCheck():boolean;
var i:byte;

begin
   i:=0;
   while (i<TAB_maxLength) and (TAB_fnSFX[i]=TABFn_blank) and (TAB_notes[i]=$00) do i:=i+1;
   result:=(i=TAB_maxLength);
end;

procedure TABDetermineLength();
var
   order,
   _ord,
   value,
   len:byte;

begin
   len:=0; TABRealLen:=0;
   fillchar(@TAB_RealPos,256,$ff);
   if not TABEmptyCheck then
      repeat
         order:=TAB_fnSFX[len];
         _ord:=order and $c0;
         if (order<>$FF) then
            if (order<TABFn_jump) or (_ord=$c0) then
            begin
               inc(TABRealLen);
               TAB_RealPos[len]:=TABRealLen;
            end
            else
               if (_ord=$80) then
               begin
                  value:=TAB_notes[len];
                  inc(TABRealLen,(order-TABFn_jump)*(len-value));
               end;
         len:=len+1;
      until (len=TAB_maxLength) or (order=TABFn_end); // or (order=TABFn_jump);
   TABLen:=len;
end;

procedure storeTABData(nTAB:byte);
var
   ofs,size:word;
   i,hid:byte;

begin
   hid:=64+nTab;
   heap_release(hid,TABNameLength);

   // if length of stored TAB is equal zero...
   if (TABLen=0) then
   begin
      // ... set TAB name as free
      TABName[0]:=char(TABNameLength);
      move(resptr[str_notDefined],@TABName[1],TABNameLength);
      exit;
   end;

   // ...otherwise, prepare data to store in heap
   ofs:=0;
   for i:=0 to TABLen-1 do
   begin
      tmpbuf[ofs]:=TAB_notes[i]; ofs:=ofs+1;
      tmpbuf[ofs]:=TAB_fnSFX[i]; ofs:=ofs+1;
   end;
   size:=TABLen shl 1; // multiply TAB length by two, becouse, for one entry is 2 bytes info

   heap_putData(hid,size,TABNameLength,TABName);
end;
