procedure heap_getData(addr,size:word; nameLength:byte; var name:string);
begin
	move(@_mem[addr],@name[1],nameLength);	// get name...
	addr:=addr+nameLength; // move addr to begining of data set
	// ... and determine its length
	while (nameLength>0) and (name[nameLength]=#0) do nameLength:=nameLength-1;
	name[0]:=char(nameLength); // store detected name length

	move(@_mem[addr],@tmpbuf,size);
end;

function heap_putData(size:word; nameLength:byte; var name:string):word;
var
	addr:word;

begin
	addr:=HEAP_TOP; result:=addr;
	//store name of TAB
	fillchar(@_mem[addr],nameLength,0);
	move(@name[1],@_mem[addr],length(name));

	addr:=addr+nameLength; // incrase current heap address

	// copy prepared data TAB to the heap
	move(@tmpbuf,@_mem[addr],size);
	HEAP_TOP:=HEAP_TOP+size+nameLength;
end;

procedure heap_release(addr,size:word);
var i:byte;

begin
	if addr<>$FFFF then
	begin
		if addr+size<HEAP_TOP then
		begin
			for i:=0 to HEAP_ENTRIES-1 do
				if (HEAP_PTR[I]<>$FFFF) and (HEAP_PTR[i]>=addr) then HEAP_PTR[i]:=HEAP_PTR[i]-size;
			move(@_mem[addr+size],@_mem[addr],HEAP_TOP-(addr+size));
		end;
		HEAP_TOP:=HEAP_TOP-size;
	end;
end;

//

function getData(ptrId,nameLength:byte; var name:string):word;
var
	addr,size:word;

begin
	addr:=HEAP_PTR[ptrId];
	if addr<>$FFFF then
	begin
		size:=_heap_sizes[ptrId]-nameLength;
		heap_getData(addr,size,nameLength,name);
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

//
//
//

function dataList(var listPtr:wordArray; currentItem,maxItems,itemSize:byte):shortint;
var
	i,ch:byte;
	adr,ofs:word;

begin
	ofs:=0; itemSize:=itemSize+1;
	fillchar(@listBuf,maxItems*itemSize,0);
	for i:=0 to maxItems-1 do
	begin
		adr:=listPtr[i];
		if (adr<>$ffff) then
		begin
			conv2internalP2P(@_mem[adr],@listBuf[ofs],itemSize);
			ch:=$0a;
		end
		else
		begin
			conv2internalP2P(resptr[str_notDefined],@listBuf[ofs],itemSize);
			ch:=$00;
		end;
		ofs:=ofs+itemSize-1;
		listBuf[ofs]:=ch; ofs:=ofs+1;
	end;
	move(@screen[20],@tmpbuf,200);
	box(0,2,20,9,$40);
	result:=listChoice(1,2,itemSize,9,currentItem,listBuf,maxItems,true);
	move(@tmpbuf,@screen[20],200);
end;
