procedure updateFreeMem();
var
	s:string[6];

begin
//	free:=trunc((HEAP_FreeMem/HEAP_MEMORY_SIZE)*100);
	str(HEAP_FreeMem,s);
	putASCIIText(20-length(s),0,s,1);
end;

procedure GSDScreen();
begin
	move(@screen,@tmpbuf,240);
	fillchar(@screen,20,$40);
	updateFreeMem();
end;
