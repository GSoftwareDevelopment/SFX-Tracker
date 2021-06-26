procedure IOQuit();
var
	answer:shortint;

begin
	answer:=messageBox(msg_IO_Quit,color_choice,but_IO_Quit,16,2,1,key_Up,key_Down);
	if answer=1 then exit;
	exit2DOS();
end;
