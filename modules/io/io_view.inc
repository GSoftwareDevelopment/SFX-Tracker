procedure IOScreen();
begin
	move(@screen,@tmpbuf,240);
	fillchar(@screen[0],20,$00);
	menuBar(resptr[menu_IO],menu_Top,0);
end;
