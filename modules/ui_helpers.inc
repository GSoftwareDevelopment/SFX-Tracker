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

procedure showAppSplash();
begin
	clearModule();
	move(resptr[app_logo],pointer(LOGO_ADDR),140);
end;

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

procedure moveRes(resId:byte; addr:word; size:word);
begin
	move(resptr[resId],pointer(addr),size);
end;

procedure exit2DOS();
begin
	SFX_End();
	halt;
end;

{$ifndef INCLUDE_RESOURCES}
procedure GotoXY(x,y: byte); assembler;
asm
rowcrs	=	$54	; wiersz  kursora
colcrs	=	$55	; (2) Kolumna kursora

	ldy x
	sty colcrs
	mvy #$00 colcrs+1
	ldy y
	sty rowcrs
end;

procedure Resources_Load();
var
	ofs,readed:word;

begin
	assign(f,resourcesFile);
	reset(f,1);
	if IOResult=1 then
	begin
		ofs:=RESOURCES_ADDR;
		repeat
			blockRead(f,listBuf,1024,readed);
			move(@listBuf,pointer(ofs),readed);
			inc(ofs,readed);
		until readed<>IO_BUFFER_ADDR;
	end
	else
	begin
		close(f);
		writeLn('RESOURCES FILE NOT FOUND :(');
		write('Press the key to return to DOS...');
		repeat until kbcode<>255; kbcode:=255;
		exit2DOS();
	end;
	close(f);
end;
{$endif}
