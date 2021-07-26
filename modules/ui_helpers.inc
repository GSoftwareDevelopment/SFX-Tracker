var
   ScreenTm,PLYtm,PLYstate:byte;

procedure PlayFlash();
begin
   if (timer-PLYtm>=$10) then
   begin
      PLYtm:=timer;
      PLYstate:=2-PLYstate;
      colorHLine(60,0,4,1+PLYstate);
   end;
end;

function controlSFXKeys():byte;
begin
	if (key and $40=$40) then
		result:=keyScan(key and $3f,keys_alphaNum,keysRange_decNum)
	else
		result:=255;
end;

function controlEditKeys(key:byte; winSize:shortint; max:byte; var _pos,_shift:byte; prevKey,nextKey:byte):boolean;
var step:shortint;

begin
   if (key and $80=$80) then
   begin
      key:=key and $7f;
      step:=winSize;
   end
   else
      step:=1;
   result:=(key=prevKey) or (key=nextKey);
   if result then
   begin
      if key=prevKey then step:=-step;
      moveCursor(step,winSize,max,_pos,_shift);
   end;
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

procedure setWorkarea();
begin
   box(0,2,20,9,$40);
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

function msgYesNo(msgID:byte):shortint;
begin
   fillchar(@moduleBar,20,$40);
   result:=messageBox(msgID,color_choice,but_YesNo,16,2,0,key_Up,key_Down);
end;

procedure set2SFX_Charset();
begin
	moveRes(chr_NoteShUp,CHARSET_ADDR+96,8);
	moveRes(chr_NoteShDn,CHARSET_ADDR+216,8);
	moveRes(chr_FreqShUp,CHARSET_ADDR+232,8);
	moveRes(chr_FreqShDn,CHARSET_ADDR+256,8);
end;

procedure set2General_Charset();
begin
	moveRes(app_charset,CHARSET_ADDR,512);
end;
