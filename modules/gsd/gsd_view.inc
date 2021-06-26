procedure updateFreeMem();
var
	s:string[6];

begin
//	free:=trunc((HEAP_FreeMem/HEAP_MEMORY_SIZE)*100);
	str((HEAP_MEMORY_SIZE-HEAP_TOP),s);
	putASCIIText(20-length(s),0,s,1);
end;

procedure GSDScreen();
begin
	clearTopMenu();
	showAppSplash();
	updateFreeMem();
end;
