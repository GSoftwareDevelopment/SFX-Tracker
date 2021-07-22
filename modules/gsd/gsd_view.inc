procedure updateFreeMem();
var
   s:string[6];

begin
   str((HEAP_MEMORY_ADDR+HEAP_MEMORY_SIZE-HEAP_TOP),s);
   putASCIIText(20-length(s),0,s,1);
end;

procedure GSDScreen();
begin
   clearTopMenu();
   showAppSplash();
   updateFreeMem();
end;
