Unit HEAP;

interface
const
	HEAP_ENTRIES			= 256;
(*
{$IFNDEF ROMOFF}
	HEAP_BOTTOM_ADDRESS  = $B400;
	HEAP_MEMORY_SIZE		= $2800;

	HEAP_DATA_ADDR			= HEAP_BOTTOM_ADDRESS - HEAP_MEMORY_SIZE - HEAP_ENTRIES * 4;
	HEAP_OFFSETS_ADDR		= HEAP_DATA_ADDR;
	HEAP_SIZES_ADDR		= HEAP_DATA_ADDR + HEAP_ENTRIES * 2;
	HEAP_MEMORY_ADDR		= HEAP_DATA_ADDR + HEAP_ENTRIES * 4;
{$ELSE}
*)
	HEAP_MEMORY_SIZE		= $27F9; //2800;
	HEAP_OFFSETS_ADDR		= $C400;
	HEAP_SIZES_ADDR		= HEAP_OFFSETS_ADDR + HEAP_ENTRIES*2;
	HEAP_MEMORY_ADDR		= $D800;
(*
{$ENDIF}
*)
var
	HEAP_TOP:word; // memory occupied by heap
	_mem:array[0..0] of byte absolute HEAP_MEMORY_ADDR;

procedure HEAP_Init();
function HEAP_Allocate(size:word):word;
function HEAP_GetAddr(hptr:word):word;
function HEAP_GetSize(hptr:word):word;
procedure HEAP_Release(hptr:word);
function HEAP_FreeMem():word;

implementation
var
	_heap_offsets:array[0..0] of word absolute HEAP_OFFSETS_ADDR;
	_heap_sizes:array[0..0] of word absolute HEAP_SIZES_ADDR;

function HEAP_Allocate(size:word):word;
var
	i,_heap_entry:word;

begin
	if (size=0) or (size>HEAP_MEMORY_SIZE) then
	begin
		result:=$ffff; exit;
	end;
	// find first free entry in heap_pointers list
	for i:=0 to HEAP_ENTRIES-1 do
		if (_heap_offsets[i]=$ffff) then
			_heap_entry:=i;

	// store HEAP_TOP in entry
	_heap_offsets[_heap_entry]:=HEAP_TOP;
	// store reserved size
	_heap_sizes[_heap_entry]:=size;

	// return heap entry
	result:=_heap_entry;

	// incrace HEAP_TOP
	HEAP_TOP:=HEAP_TOP+size;
end;

function HEAP_GetAddr(hptr:word):word;
begin
	if (hptr<>$ffff) then
		result:=_heap_offsets[hptr]
	else
		result:=$ffff;
end;

function HEAP_GetSize(hptr:word):word;
begin
	if (hptr<>$ffff) then
		result:=_heap_sizes[hptr]
	else
		result:=0;
end;

procedure HEAP_Release(hptr:word);
var
	c,adr,size:word;

begin
	// get entry address
	adr:=_heap_offsets[hptr];
	size:=_heap_sizes[hptr];
	// keep from releasing the free pointer
	if (adr=$ffff) then exit;

	if (adr+size<HEAP_TOP) then
	begin
		// search the heap for addresses greater than or equal to the pointer to be released (its address)
		for c:=0 to HEAP_ENTRIES-1 do
			if (_heap_offsets[c]>=adr) then
				_heap_offsets[c]:=_heap_offsets[c]-size;
	// shift heap move data up the heap, freeing up heap memory
		move(@_mem[adr+size],@_mem[adr],size);
	end;

	_heap_offsets[hptr]:=$ffff;
	HEAP_TOP:=HEAP_TOP-size;
end;

function HEAP_FreeMem:word;
begin
	result:=HEAP_MEMORY_SIZE-HEAP_TOP;
end;

procedure HEAP_Init;
begin
	fillchar(@_mem,HEAP_MEMORY_SIZE,$ff);
	fillchar(@_heap_offsets,HEAP_ENTRIES*2,$FF);
	fillchar(@_heap_sizes,HEAP_ENTRIES*2,$FF);
end;

end.
