procedure memory_stats();
var
	memSFX,mem:word;
	i:byte;
	s:string[6];

	procedure calcMem(s,f:byte);
	begin
		mem:=0;
		while s<f do
		begin
			if _heap_sizes[s]<>$FFFF then inc(mem,_heap_sizes[s]);
			inc(s);
		end;
	end;

	procedure putVal();
	begin
		str(mem,s); dec(__scrOfs,length(s)); putASCIIText(s,0);
	end;

begin
	clearModule();
	putMultiText(msg_mem_stats,0);

	calcMem(0,64); memSFX:=mem;
	__scrOfs:=98; putVal();
	calcMem(64,128);
	__scrOfs:=118; putVal();
	mem:=mem+memSfx; __scrOfs:=158; putVal();
	mem:=HEAP_MEMORY_ADDR+HEAP_MEMORY_SIZE-HEAP_TOP;
	__scrOfs:=198; putVal();
end;
