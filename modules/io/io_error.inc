procedure IOError(err:byte);
var
	flashTM:longint;
	flashState:boolean;

begin
	fillchar(@screen[220],20,0);
	putMultiText(resptr[msg_IO_Error],3);
	putValue(14,11,err,3,0);
	screen2video();
	kbcode:=255; flashState:=false; flashTM:=0;
	repeat
		if (getTime-flashTM>=25) then
		begin
			flashState:=not flashState;
			colorHLine(0,11,20,3*byte(flashState));
			screen2video();
			flashTM:=getTime;
		end;
	until kbcode<>255; kbcode:=255;
	fillchar(@screen[220],20,0);
	screen2video();
end;
