procedure updateFreeMem();
var
	s,o:string[3];
	free:word;
	submenu:array[0..0] of byte;

begin
	submenu:=resptr[menu_GSD];
	free:=trunc((HEAP_FreeMem/HEAP_MEMORY_SIZE)*100);
	str(free,s); o:=concat(StringOfChar('0',3-length(s)),s);
	conv2Internal(o);
	move(@o[1],@submenu[17],3);
end;

procedure GSDScreen();
begin
	move(@screen,@tmpbuf,240);
	fillchar(@screen,20,$00);
	menuBar(resptr[menu_GSD],menu_Top,0);
end;
