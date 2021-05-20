procedure IOQuit();
begin
	if messageBox(2,2,resptr[str_IO_Quit],color_choice,resptr[menu_YesNo],16,2,1)=1 then
		move(@tmpbuf[40],@screen[40],60)
	else
		halt(1);
end;
