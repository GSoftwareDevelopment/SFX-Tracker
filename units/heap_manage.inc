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
		move(pointer(addr),@tmpbuf,size);

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

	inc(HEAP_TOP,nameLength); 					// incrase address by nameLength
	HEAP_PTR[hid]:=HEAP_TOP;
	_heap_sizes[hid]:=size;

	move(@tmpbuf,pointer(HEAP_TOP),size);	// copy prepared data to the heap

	inc(HEAP_TOP,size);							// set HEAP_TOP to the end of stored data
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

//
//
//

function dataList(var listPtr:wordArray; currentItem,maxItems,itemSize:byte):shortint;
var
	i,ch:byte;
	adr,ofs:word;
	tptr:pointer;

begin
	ofs:=0; itemSize:=itemSize+1;
	clearListBuf();
	for i:=0 to maxItems-1 do
	begin
		adr:=listPtr[i];
		if (adr<>$ffff) then
		begin
			tptr:=pointer(adr-itemSize+1);
			ch:=$0a;
		end
		else
		begin
			tptr:=resptr[str_notDefined];
			ch:=$00;
		end;
		conv2internalP2P(tptr,@listBuf[ofs],itemSize);
		ofs:=ofs+itemSize;
		listBuf[ofs-1]:=ch;
	end;
	move(@moduleBar,@tmpbuf,200);
	box(0,2,20,9,$40);
	result:=listChoice(1,2,itemSize,9,currentItem,listBuf,maxItems,true);
	move(@tmpbuf,@moduleBar,200);
end;
