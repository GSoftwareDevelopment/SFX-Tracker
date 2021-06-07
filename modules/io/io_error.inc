procedure flashError();
var
	flashTM:byte;
	flashState:boolean;

begin
	screen2video();
	kbcode:=255; flashState:=false; flashTM:=timer;
	repeat
		if (timer-flashTM>=$10) then
		begin
			flashTM:=timer;
			flashState:=not flashState;
			colorHLine(0,11,20,3*byte(flashState));
			screen2video();
		end;
	until kbcode<>255; kbcode:=255;
	fillchar(@statusBar,20,0);
	screen2video();
end;

procedure showError(msg:pointer);
begin
	fillchar(@statusBar,20,0);
	putMultiText(msg,0);
	flashError();
end;

procedure IOError(err:byte);
begin
	fillchar(@statusBar,20,0);
	putMultiText(resptr[msg_IO_Error],0);
	putValue(14,11,err,3,0);
	flashError();
end;
