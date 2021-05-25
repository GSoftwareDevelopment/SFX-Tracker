procedure IOScreen();
begin
	move(@screen,@tmpbuf,240);
	fillchar(@screen[0],20,$40);
end;
