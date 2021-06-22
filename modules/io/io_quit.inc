procedure IOQuit();
var
	answer:shortint;

begin
	answer:=messageBox(msg_IO_Quit,color_choice,but_IO_Quit,16,2,1,key_Up,key_Down);
	if answer=0 then halt(1);
	move(@tmpbuf[40],@screen[40],60)
end;
