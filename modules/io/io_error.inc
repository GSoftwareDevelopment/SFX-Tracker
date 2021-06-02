procedure IOError(err:byte);
var
	flashTM:byte;
	flashState:boolean;

begin
	fillchar(@screen[220],20,0);
	putMultiText(resptr[msg_IO_Error],3);
	putValue(14,11,err,3,0);
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
	fillchar(@screen[220],20,0);
	screen2video();
end;
