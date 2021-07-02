procedure IONew();
begin
	if msgYesNo(msg_newPrompt)=1 then IO_clearAllData();
end;
