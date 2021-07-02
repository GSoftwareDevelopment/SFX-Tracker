procedure IOQuit();
begin
	if msgYesNo(msg_IO_Quit)=1 then exit2DOS();
end;
