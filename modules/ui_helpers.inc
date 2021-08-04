var
   ScreenTm,PLYtm,PLYstate:byte;

procedure PlayFlash();
begin
   if (timer-PLYtm>=$10) then
   begin
      PLYtm:=timer;
      PLYstate:=2-PLYstate;
      __scrOfs:=60; setColor(1+PLYstate); colorHLine(4);
   end;
end;

function controlEditKeys(winSize:shortint; max:byte; var _pos,_shift:byte; prevKey,nextKey:byte):boolean;
var step:shortint;
	okey:TKeys;

begin
	okey:=key;
   if (* keyCTRL *) (key and $80=$80) then
   begin
		key:=key and $7f;
      step:=winSize;
   end
   else
      step:=1;
//   result:=(ckey=prevKey) or (ckey=nextKey);
   result:=(key=prevKey) or (key=nextKey);
   if result then
   begin
      if key=prevKey (*ckey=prevKey*) then step:=-step;
      moveCursor(step,winSize,max,_pos,_shift);
   end;
   key:=okey;
end;

procedure clearListBuf();
begin
   fillchar(@listBuf,LIST_BUFFER_SIZE,0);
end;

procedure uncolorWorkarea();
var i:byte;

begin
	__scrOfs:=20; color:=0;
   i:=10; while i>0 do begin colorHLine(20); dec(i); end;
end;

procedure clearTopMenu();
begin
   fillchar(@screen,20,$40);
end;

procedure clearWorkarea();
begin
   __scrOfs:=44; color:=0; box(16,9);
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
   __scrOfs:=40; color:=$40; box(20,9);
end;

procedure showAppSplash();
begin
   clearModule();
   move(resptr[app_logo],pointer(LOGO_ADDR),140);
end;

procedure flashError();
var
   flashTM:byte;

begin
   screen2video();
   kbcode:=255; flashTM:=timer;
   color:=0;
   repeat
      if (timer-flashTM>=$10) then
      begin
         flashTM:=timer;
         color:=$c0-color;
         __scrOfs:=220; colorHLine(20);
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

	procedure copyChar(resId:byte; addr:word);
	begin
		moveRes(resId,CHARSET_ADDR+addr,8);
	end;

begin
	copyChar(chr_NoteShUp,96);
	copyChar(chr_NoteShDn,216);
	copyChar(chr_FreqShUp,232);
	copyChar(chr_FreqShDn,256);
end;

procedure set2General_Charset();
begin
	moveRes(app_charset,CHARSET_ADDR,512);
end;

function controlSFXShortcutKeys():byte;
begin
	if (key and $40=$40) then
	begin
		key:=key and $3f;
		result:=keyScan(keys_alphaNum,keysRange_decNum);
		key:=key or $40;
	end
	else
		result:=255;
end;

//

function SFXshortcutExist(excludeSC,nSFX:byte):byte;
var i:byte;

begin
	result:=255;
	for i:=0 to 9 do if (i<>excludeSC) and (SFX_shortcut[i]=nSFX) then result:=i;
end;

function setShortcut2currentSFX(SFXKey:byte):byte;
var i:byte;

begin
	i:=SFXshortcutExist(SFXKey,currentSFX); if i<>255 then SFX_shortcut[i]:=255;
	result:=SFX_shortcut[SFXKey];
	if (result<>currentSFX) then result:=currentSFX else result:=255;
	SFX_shortcut[SFXKey]:=result;
end;

procedure SFXshortcutMessage(SFXKey:byte);
var
	nSFX:byte;

begin
	nSFX:=SFX_shortcut[SFXKey];
	color:=0;
	if nSFX<>255 then
	begin
		drawError(msg_SFX_FastKey_Assigned);
		__scrOfs:=238; putValue(nSFX,2);
	end
	else
	begin
		drawError(msg_SFX_FastKey_Cleared);
	end;
	__scrOfs:=229; putValue(SFXKey,1);
	screen2video();
end;

function dataList(var listPtr:wordArray; currentItem,maxItems,itemSize:byte):shortint;
var
   i,ch,lis:byte;
   adr,ofs:word;
   tptr:pointer;
	isSFX:boolean;

begin
	isSFX:=lo(word(@listPtr))=$00;

   ofs:=0; lis:=itemSize+1;
   if isSFX then inc(lis);
   clearListBuf();
   for i:=0 to maxItems-1 do
   begin
      adr:=listPtr[i];
      if (adr<>$ffff) then
      begin
         tptr:=pointer(adr-itemSize);
         ch:=$0a;
      end
      else
      begin
         tptr:=resptr[str_notDefined];
         ch:=$00;
      end;
      listBuf[ofs]:=ch;
      conv2internalP2P(tptr,@listBuf[ofs+1],itemSize);
      ofs:=ofs+lis;

		if isSFX then
		begin
			ch:=SFXshortcutExist(255,i);
			if ch<>255 then
				ch:=$50+ch
			else
				ch:=$00;
			listBuf[ofs-1]:=ch;
		end;
   end;
   setWorkarea();
   result:=listChoice(1,2,lis,9,currentItem,listBuf,maxItems,true);
   PMGClear(); // prevent to draw mess in PMG
   key:=TKeys(255); // clear keyboard state
end;
