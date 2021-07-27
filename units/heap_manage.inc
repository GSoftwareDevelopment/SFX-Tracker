function heap_getData(hid,nameLength:byte; var name:string):word;
var
   addr,size:word;

begin
   addr:=HEAP_PTR[hid];
   if addr<>$FFFF then
   begin
      size:=_heap_sizes[hid];

      // get data name
      move(pointer(addr-nameLength),@name[1],nameLength);
      // and determine its length
      while (nameLength>0) and (name[nameLength]=#0) do nameLength:=nameLength-1;
      name[0]:=char(nameLength); // store detected name length
      // get data definition
      move(pointer(addr),@heapStoreBuf,size);

      result:=size;
   end
   else
   begin
      // set notDefined string to current SFX name
      name[0]:=char(nameLength);
      move(resptr[str_notDefined],@name[1],nameLength);
      result:=0;
   end;
end;

procedure heap_putData(hid:byte; size:word; nameLength:byte; var name:string);
begin
   // addr:=HEAP_TOP;
   // store data name
   fillchar(pointer(HEAP_TOP),nameLength,0);
   move(@name[1],pointer(HEAP_TOP),length(name));

   inc(HEAP_TOP,nameLength);              // incrase address by nameLength
   HEAP_PTR[hid]:=HEAP_TOP;
   _heap_sizes[hid]:=size;

   move(@heapStoreBuf,pointer(HEAP_TOP),size);  // copy prepared data to the heap

   inc(HEAP_TOP,size);                    // set HEAP_TOP to the end of stored data
end;

procedure heap_release(hid,nameLength:byte);
var i:byte;
   hadr:word;
   addr,size,sizewname:word;

begin
   addr:=HEAP_PTR[hid];
   if addr<>$FFFF then
   begin
      size:=_heap_sizes[hid];
      sizewname:=size+nameLength;
      if addr+size<HEAP_TOP then
      begin
         // inc(size,nameLength);
         for i:=0 to HEAP_ENTRIES-1 do
         begin
            hadr:=HEAP_PTR[i];
            if (hadr<$FFFF) then
              if (hadr>=addr) then HEAP_PTR[i]:=hadr-sizewname;
         end;
         hadr:=addr+size;
         move(pointer(hadr),pointer(addr-nameLength),HEAP_TOP-hadr+nameLength);
      end;
      HEAP_TOP:=HEAP_TOP-sizewname;
      HEAP_PTR[hid]:=$FFFF;
      _heap_sizes[hid]:=$FFFF;
   end;
end;
