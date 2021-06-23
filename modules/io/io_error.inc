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
	clearStatusBar();
end;

procedure drawError(msg:byte);
begin
	clearStatusBar();
	putMultiText(msg,0);
end;

procedure showError(msg:byte);
begin
	drawError(msg);
	flashError();
end;

procedure IOError(err:byte);
begin
	drawError(msg_IO_Error);
	putValue(14,11,err,3,0);
	flashError();
end;
