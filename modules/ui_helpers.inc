var
	ScreenTm,PLYtm,PLYstate:byte;

procedure PlayFlash();
begin
	if (timer-PLYtm>=$10) then
	begin
		PLYtm:=timer;
		PLYstate:=2-PLYstate;
		colorHLine(60,0,4,1+PLYstate);
//		move(@screen[60],@video[60],4);
	end;
end;

procedure controlEditKeys(key:byte; winSize:shortint; max:byte; var _pos,_shift:byte; pcKey,ncKey:byte);
var step:shortint;

begin
	if (key and $80=$80) then
	begin
		key:=key and $7f;
		step:=winSize;
	end
	else
		step:=1;
	if key=pcKey then
		moveCursor(-step,winSize,max,_pos,_shift)
	else
		if key=ncKey then
			moveCursor(step,winSize,max,_pos,_shift);
end;

procedure clearListBuf();
begin
	fillchar(@listBuf,LIST_BUFFER_SIZE,0);
end;

procedure uncolorWorkarea();
var i:byte;

begin
	for i:=1 to 11 do
		colorHLine(0,i,20,0);
end;

procedure clearTopMenu();
begin
	fillchar(@screen,20,$40);
end;

procedure clearWorkarea();
begin
	box(4,2,16,9,0);
end;

procedure clearStatusBar();
begin
	fillchar(@statusBar,20,0);
end;

procedure clearModuleBar();
begin
	fillchar(@moduleBar,20,0);
end;

procedure clearModule();
begin
	fillchar(@moduleBar,220,0);
end;
