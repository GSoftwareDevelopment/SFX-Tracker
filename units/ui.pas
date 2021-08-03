unit UI;

interface

type
   byteArray = array[0..0] of byte;

   TKeys = (
      key_Left = 6,
      key_Right = 7,
      key_RETURN = 12,
      key_Up = 14,
      key_Down = 15,
      key_ESC = 28,
      key_SPACE = 33,
      key_INVERS = 39,
      key_TAB = 44,
      key_BackSpc = 52,
      key_CAPS = 60,
      key_SHIFT_Left = 70,
      key_SHIFT_Right = 71,
      key_SHIFT_RETURN = 76,
      key_SHIFT_Up = 78,
      key_SHIFt_Down = 79,
      key_SHIFT_TAB = 108,
		key_SHIFT_DELETE = 116,
		key_SHIFT_CLEAR = 118,
      key_SHIFT_INSERT = 119,

      key_CTRL_Left = 134,
      key_CTRL_Right = 135,
      key_CTRL_RETURN = 140,
      key_CTRL_Up = 142,
      key_CTRL_Down = 143,
      key_CTRL_V = 144,
      key_CTRL_C = 146,
      key_CTRL_X = 150,
      key_CTRL_BackSpc = 180,
      key_CTRL_DELETE = 180,
		key_CTRL_CLEAR = 182,
      key_CTRL_INSERT = 183,

      key_CTRL_SHIFT_Up = 206,
      key_CTRL_SHIFT_Down = 207
);

const
	key_SHIFT			= 64;
	key_CONTROL			= 128;

   key_0	=50;
   key_1	=31;
   key_2	=30;
   key_3	=26;
   key_4	=24;
   key_5	=29;
   key_6	=27;
   key_7	=51;
   key_8	=53;
   key_9	=48;

   key_A	=63;
   key_B =21;
   key_C	=18;
   key_D	=58;
   key_E	=42;
   key_F	=56;
   key_G	=61;
   key_H	=57;
   key_I	=13;
   key_J	=1;
   key_K	=5;
   key_L	=0;
   key_M	=37;
   key_N	=35;
   key_O	=8;
   key_P	=10;
   key_Q	=47;
   key_R	=40;
   key_S	=62;
   key_T	=45;
   key_U	=11;
   key_V	=16;
   key_W	=46;
   key_X	=22;
   key_Y	=43;
   key_Z	=23;

const
   color_white       = 0;
   color_choice      = 1;
   color_background  = 2;
   color_selected    = 3;

   keysRange_all        = 49;
   keysRange_hexNum     = 16;
   keysRange_decNum     = 10;

var
   KBCODE:byte absolute 764;
   SSFLAG:byte absolute $2FF;
   KRPDEL:byte absolute $2d9;
   KEYREP:byte absolute $2da;
   CONSOL:byte absolute 53279;
   key:TKeys;
//   ckey:TKeys;
//   keySHIFT:boolean;
//   keyCTRL:boolean;
   timer:byte absolute $14;
   keyClick:boolean = true;

   resptr:array[0..0] of pointer; // pointers list to resources
   chars_alphaNum,
   keys_alphaNum:byteArray;

procedure Init_UI(resAddr:word);
function keyPressed():boolean;
function checkEscape:boolean;
function keyScan(var keyDefs:byteArray; keysRange:byte):byte;
function controlSelectionKeys(decKey,incKey:byte; var value:byte; min,max:byte):boolean;
procedure moveCursor(ofs:shortint; winSize,overSize:byte; var curPos,curShift:byte);
function inputText(x,y,width:byte; var s:string; colEdit,colOut:byte):boolean;
function inputLongText(x,y,width:byte; maxLen:byte; var s:string; colEdit,colOut:byte):boolean;
function inputValue(x,y,width:byte; var v:smallint; min,max:smallint; colEdit,colOut:byte):boolean;
procedure putMultiText(multitext:byte; bgColor:byte);
procedure VBar(x,y,width,height,col:byte);
// procedure updateBar(bar:byte; width:byte; currentSel:shortint; canChoiceColor,choicedColor:byte);
procedure menuBar(bar:byte; width:byte; currentSel:shortint; bgColor,selColor:byte);
function optionsList(optTabs:byte; optWidth:byte; opts:byte; var currentOpt:byte; pcKey,ncKey:byte):boolean;
function listChoice(x,y,width,height,defaultPos:byte; listPtr:pointer; listSize:byte; showCount:boolean):shortint;
function messageBox(msgPtr:byte; msgColor:byte; menuPtr:byte; menuWidth,menuOpts:byte; defaultOpt:byte; pcKey,ncKey:byte):shortint;

implementation
uses gr2;

procedure Init_UI;
begin
   resptr:=pointer(resAddr);
   SSFLAG:=0;
end;

function keyPressed:boolean;
var i:byte;

begin
	if SSFLAG<>0 then
	begin
		SSFLAG:=0; kbcode:=159; // CONTROL+1
	end;
   if kbcode<>255 then
   begin
      key:=TKeys(kbcode);
//      keyCTRL:=kbcode and $80=$80;
//      keySHIFT:=kbcode and $40=$40;
//      ckey:=TKeys(kbcode and $3f);
      kbcode:=255;
      result:=true;
      if keyClick then
      begin
         asm
            ldx #$2E
            pha
loop_beep
            stx $D01F   ;CONSOL
            lda $D40B   ;VCOUNT
loop_wait
            cmp $D40B   ;VCOUNT
            beq loop_wait
            dex
            bpl loop_beep
            pla
         end;
      end;
   end
   else
      result:=false;
end;

function checkEscape:boolean;
begin
	result:=(key=key_ESC) or (key=key_BackSpc); key:=TKeys($FF);
end;

function keyScan:byte;
var
   keyOfs:byte;

begin
   result:=255;
   for keyOfs:=0 to keysRange-1 do
      if (keyDefs[keyOfs]=key) then
         exit(keyOfs);
end;

function controlSelectionKeys:boolean;
begin
   if key=decKey then
   begin
      if value>min then value:=value-1 else value:=max;
      exit(true);
   end;
   if key=incKey then
   begin
      if value<max then value:=value+1 else value:=min;
      exit(true);
   end;
   result:=false;
end;

procedure moveCursor;
var _pos:smallint;

begin
   _pos:=curPos+ofs;
   if (oversize<=winsize) then
   begin
      if (_pos>-1) and (_pos<oversize) then
         curPos:=_pos
      else
         if (_pos<0) then
            _pos:=0
         else
            curPos:=oversize-1;
   end
   else
      if (_pos<0) then
      begin
//         if (curPos=0) then
//         begin
            _pos:=curShift+ofs;
            if (_pos>=0) then
               curShift:=_pos
            else
               curShift:=0
//         end;
//         curPos:=0;
      end
      else
      if (_pos>winSize-1) then
      begin
//         if (curPos=winSize-1) then
//         begin
            _pos:=curShift+ofs;
            if (_pos<=overSize-winSize) then
               curShift:=_pos
            else
               curShift:=overSize-winSize;
//         end;
//         curPos:=winSize-1;
      end
      else
         curPos:=_pos
end;

function inputLongText:boolean;
var
   buf:byteArray absolute $400; // use IO buffer for temporary input strage
   i,len:byte;
   curX,shiftX:byte;
   ch,ofs,scrOfs:byte;
   ctm,stm:byte;
   curState:boolean;

   procedure updateTextLine();
   begin
		__scrOfs:=scrOfs;
      move(@buf[shiftX],@screen[scrOfs],width);
      if width<20 then
         colorHLine(width+1,colEdit)
      else
         colorHLine(width,colEdit)
   end;

	procedure drawCursor();
	begin
		__scrOfs:=scrOfs+curX;
		screen[__scrOfs]:=screen[__scrOfs] xor $C0;
	end;

begin
   len:=length(s);
   fillchar(@buf,256,0);
   conv2internalP2P(@s[1],@buf,len);
   if (len>width) then
   begin
      curX:=width-1; shiftX:=len-width;
   end
   else
   begin
      curX:=len; shiftX:=0;
   end;

   scrOfs:=vadr[y]+x;
   updateTextLine();
   screen2video();
   ctm:=0; curState:=false;
   repeat
      if keyPressed then
      begin
         ofs:=shiftX+curX;
         if curState then drawCursor();
         case key of
            key_ESC: begin result:=false; break; end;
            key_RETURN: begin result:=true; break; end;
            key_CTRL_Left:
               moveCursor(-1,width,len,curX,shiftX);
            key_CTRL_Right:
               moveCursor(+1,width,len,curX,shiftX);
            key_BackSpc:
               if (ofs>0) then
               begin
                  move(@buf[ofs],@buf[ofs-1],len-ofs);
                  if (curX=0) and (shiftX>0) then
                  begin
                     i:=shiftX;
                     if (i>width div 2) then i:=width div 2;
                     moveCursor(-i,width,maxLen,curX,shiftX);
                     moveCursor(+i,width,maxLen,curX,shiftX);
                  end;
                  moveCursor(-1,width,maxLen,curX,shiftX);
                  len:=len-1; buf[len]:=0;
                  ofs:=ofs-1;
               end
         end;

         if (ofs<maxLen) then
         begin
            i:=keyScan(keys_alphaNum,keysRange_all);
            if (i<>255) then
            begin
               move(buf[ofs],buf[ofs+1],len-ofs);
               ch:=chars_alphaNum[i];
               buf[ofs]:=ch;
               if len<maxLen then len:=len+1;
               moveCursor(+1,width,maxLen,curX,shiftX);
            end;
         end;

         updateTextLine();
         kbcode:=255; ctm:=0; curState:=false;
      end;
      if (timer-ctm>=$10) then
      begin
         ctm:=timer;
         curState:=not curState;
         drawCursor();
      end;
      if (timer<>stm) then
      begin
         stm:=timer;
         screen2video();
      end;
   until false;
   if result then
   begin
      s[0]:=char(len);
      if len>0 then move(@buf,@s[1],width);
      move(@buf,@screen[scrOfs],width);
      conv2ASCII(s);
   end
   else
      conv2internalP2P(@s[1],@screen[scrOfs],width);
   __scrOfs:=vadr[y]+x; colorHLine(width,colOut);
   key:=TKeys($FF);
end;

function inputText:boolean;
begin
   result:=inputLongText(x,y,width,width,s,colEdit,colOut);
end;

function inputValue:boolean;
var
   s:string[4];
   err:byte;
   ok:boolean;

begin
   result:=false;
   repeat
      str(v,s);
      ok:=inputText(x,y,width,s,colEdit,colOut);
      if not ok then exit;
      val(s,v,err);
      if (v<min) or (v>max) then err:=255;
   until err=0;
   result:=true;
end;

procedure putMultiText;
begin
   menuBar(multitext,0,-1,bgColor,bgColor);
end;

procedure VBar;
begin
   col:=colMask[col]; width:=width-1;
   setPos(x,y);
   while height>0 do
   begin
      fillchar(@screen[__scrOfs],width,col);
      screen[__scrOfs+width]:=$06+col;
      inc(__scrOfs,20); dec(height);
   end;
end;

procedure menuBar;
var
   opt,j,v,col:byte;
   dataOfs:byte;
   data:byteArray;

begin
   data:=resptr[bar];
   bgColor:=colMask[bgColor]; selColor:=colMask[selColor];
   dataOfs:=0; width:=width-1; opt:=0;
   while (data[dataOfs]<>255) do
   begin
		if (opt=currentSel) then
         col:=selColor
      else
         col:=bgColor;

      __scrOfs:=data[dataOfs]; inc(dataOfs);
      for j:=0 to width-1 do
      begin
         v:=data[dataOfs];
         if (v=255) then // entry end
         begin
            if (width=255) then break;
            v:=0;
         end
         else
            inc(dataOfs);

         screen[__scrOfs]:=col+v;
         inc(__scrOfs);
      end;
      if (width<>255) then
         screen[__scrOfs]:=col+$06;

      inc(dataOfs); inc(opt);
   end;
end;

function optionsList:boolean;
begin
   opts:=opts-1;
   menuBar(optTabs,optWidth,currentOpt,color_choice,color_selected);
//   updateBar(optTabs,optWidth,currentOpt,color_choice,color_selected);
   screen2video();
   repeat
      if keyPressed then
      begin
         controlSelectionKeys(pckey,ncKey,currentOpt,0,opts);
         case key of
            key_ESC, key_BackSpc: begin result:=false; break; end;
            key_RETURN: begin result:=true; break; end;
         end;
			menuBar(optTabs,optWidth,currentOpt,color_choice,color_selected);
//         updateBar(optTabs,optWidth,currentOpt,color_choice,color_selected);
         screen2video();
      end;
   until false;
   key:=TKeys($FF);
end;

function listChoice:shortint;
var
   listShift,listPos:byte;
   listData:array[0..0] of byte;
	ofs:word;

   procedure drawCursor();
   begin
		setPos(x,y+listPos); colorHLine(width+2*byte(showCount),3);
   end;

   procedure updateList();
   var i,n:byte;

   begin
		n:=listShift;
      ofs:=n*width;
      setPos(x,y);
      for i:=0 to height-1 do
      begin
			if showCount then
			begin
				putValue(n,2,1);
				screen[__scrOfs]:=$40;
			end;

			move(@listData[ofs],@screen[__scrOfs],width);
			inc(ofs,width); inc(n);
			colorHLine(width,1);
			if showCount then
				inc(__scrOfs,18-width)
			else
				inc(__scrOfs,20-width);
		end;
		drawCursor();
		screen2video();
   end;

begin
   listData:=listPtr;
   listShift:=(defaultPos div height)*height;
   listPos:=defaultPos mod height;
   if (defaultPos>listSize-height) then
   begin
      listShift:=listSize-height;
      listPos:=defaultPos-listShift;
   end;
   updateList();
   kbcode:=255;
   repeat
      if keyPressed then
      begin
         case key of
            key_CTRL_Up:begin
               moveCursor(-height,height,listSize,listPos,listShift);
            end;
            key_CTRL_Down:begin
               moveCursor(+height,height,listSize,listPos,listShift);
            end;

            key_Up:
               moveCursor(-1,height,listSize,listPos,listShift);
            key_Down:
               moveCursor(+1,height,listSize,listPos,listShift);
            key_ESC, key_BackSpc: begin
               result:=-1;
               break;
            end;
            key_RETURN: begin
               result:=listShift+listPos;
               break;
            end;
         end;
         updateList();
      end;
   until false;
   key:=TKeys($FF);
end;

function messageBox:shortint;
begin
   putMultiText(msgPtr,msgColor);
   if optionsList(menuPtr,menuWidth,menuOpts,defaultOpt,pcKey,ncKey) then
      result:=defaultOpt
   else
      result:=-1;
end;

end.
