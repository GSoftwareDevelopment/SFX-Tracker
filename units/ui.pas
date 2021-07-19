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
      key_INSERT = 55,
      key_CAPS = 60,
      key_SHIFT_RETURN = 76,
      key_SHIFT_TAB = 108,
      key_CTRL_Left = 134,
      key_CTRL_Right = 135,
      key_CTRL_RETURN = 140,
      key_CTRL_Up = 142,
      key_CTRL_Down = 143,
      key_CTRL_BackSpc = 180,
      key_CTRL_DELETE = 180,
      key_CTRL_INSERT = 183,

      key_SHIFT_Up = 206,
      key_SHIFT_Down = 207
);

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
   KRPDEL:byte absolute $2d9;
   KEYREP:byte absolute $2da;
   CONSOL:byte absolute 53279;
   key:TKeys;
   timer:byte absolute $14;
   keyClick:boolean = true;

   resptr:array[0..0] of pointer; // pointers list to resources
   chars_alphaNum,
   keys_alphaNum:byteArray;

procedure Init_UI(resAddr:word);
function keyPressed():boolean;
function keyScan(key2Scan:byte; var keyDefs:byteArray; keysRange:byte):byte;
function controlSelectionKeys(var keyIn:byte; decKey,incKey:byte; var value:byte; min,max:byte):boolean;
procedure moveCursor(ofs:shortint; winSize,overSize:byte; var curPos,curShift:byte);
function inputText(x,y,width:byte; var s:string; colEdit,colOut:byte):boolean;
function inputLongText(x,y,width:byte; maxLen:byte; var s:string; colEdit,colOut:byte):boolean;
function inputValue(x,y,width:byte; var v:smallint; min,max:smallint; colEdit,colOut:byte):boolean;
procedure putMultiText(multitext:byte; bgColor:byte);
procedure VBar(x,y,width,height,col:byte);
procedure updateBar(bar:byte; width:byte; currentSel:shortint; canChoiceColor,choicedColor:byte);
procedure menuBar(bar:byte; width,bgColor:byte);
function optionsList(optTabs:byte; optWidth:byte; opts:byte; var currentOpt:byte; pcKey,ncKey:byte):boolean;
function listChoice(x,y,width,height,defaultPos:byte; listPtr:pointer; listSize:byte; showCount:boolean):shortint;
function messageBox(msgPtr:byte; msgColor:byte; menuPtr:byte; menuWidth,menuOpts:byte; defaultOpt:byte; pcKey,ncKey:byte):shortint;

implementation
uses gr2;

procedure Init_UI;
begin
   resptr:=pointer(resAddr);
end;

function keyPressed:boolean;
var i:byte;

begin
   if kbcode<>255 then
   begin
      key:=TKeys(kbcode); kbcode:=255;
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

function keyScan:byte;
var
   keyOfs:byte;

begin
   result:=255;
   for keyOfs:=0 to keysRange-1 do
      if (keyDefs[keyOfs]=key2Scan) then
         exit(keyOfs);
end;

function controlSelectionKeys:boolean;
begin
   if keyIn=decKey then
   begin
      if value>min then value:=value-1 else value:=max;
      exit(true);
   end;
   if keyIn=incKey then
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
            curPos:=oversize;
   end
   else
      if (_pos<0) then
      begin
         if (curPos=0) then
         begin
            _pos:=curShift+ofs;
            if (_pos>=0) then
               curShift:=_pos
            else
               curShift:=0
         end;
         curPos:=0;
      end
      else
      if (_pos>winSize-1) then
      begin
         if (curPos=winSize-1) then
         begin
            _pos:=curShift+ofs;
            if (_pos<=overSize-winSize) then
               curShift:=_pos
            else
               curShift:=overSize-winSize;
         end;
         curPos:=winSize-1;
      end
      else
         curPos:=_pos
end;

function inputText:boolean;
begin
   result:=inputLongText(x,y,width,width,s,colEdit,colOut);
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
      move(@buf[shiftX],@screen[scrOfs],width);
      if width<20 then
         colorHLine(x,y,width+1,colEdit)
      else
         colorHLine(x,y,width,colEdit)
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
         // screen[scrOfs+curX]:=buf[ofs] or colEdit;
         if curState then screen[scrOfs+curX]:=screen[scrOfs+curX] xor $C0;
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
            i:=keyScan(key,keys_alphaNum,keysRange_all);
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
         screen[scrOfs+curX]:=screen[scrOfs+curX] xor $C0;
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
   colorHLine(x,y,width,colOut);
   kbcode:=255;
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
(*
procedure putMultiText;
var
   v:byte;
   dataOfs:byte;
   scrOfs:word;
   data:array[0..0] of byte;

begin
   data:=bar;
   bgColor:=colMask[bgColor];
   dataOfs:=0;
   while (data[dataOfs]<>255) do
   begin
      scrOfs:=data[dataOfs]; dataOfs:=dataOfs+1; // get screen offset
      repeat
         v:=data[dataOfs];
         if (v=255) then break; // entry end

         screen[scrOfs]:=bgColor+v;
         scrOfs:=scrOfs+1;
         dataOfs:=dataOfs+1;
      until false;

      dataOfs:=dataOfs+1;
   end;
end;
*)

procedure putMultiText;
begin
   menuBar(multitext,0,bgColor);
end;

procedure VBar;
var ofs:byte;

begin
   col:=colMask[col]; width:=width-1;
   ofs:=x+vadr[y];
   while height>0 do
   begin
      fillchar(@screen[ofs],width,col);
      screen[ofs+width]:=$06+col;
      ofs:=ofs+20; height:=height-1;
   end;
end;

procedure updateBar;
var
   i,j,v:byte;
   dataOfs,col:byte;
   scrOfs:word;
   data:byteArray;

begin
   data:=resptr[bar];
   canChoiceColor:=colMask[canChoiceColor];
   choicedColor:=colMask[choicedColor];
   dataOfs:=0; i:=0; width:=width-1;
   while (data[dataOfs]<>255) do
   begin
      if (i=currentSel) then
         col:=choicedColor
      else
         col:=canChoiceColor;

      scrOfs:=data[dataOfs]; dataOfs:=dataOfs+1;
      for j:=0 to width do
      begin
         if (data[dataOfs]=255) then // entry end
         begin
            if (width=255) then break
         end
         else
            dataOfs:=dataOfs+1;

         v:=screen[scrOfs] and $3f;
         screen[scrOfs]:=col+v;
         scrOfs:=scrOfs+1;
      end;
      i:=i+1; dataOfs:=dataOfs+1;
   end;
end;

procedure menuBar;
var
   j,v:byte;
   dataOfs:byte;
   scrOfs:word;
   data:byteArray;

begin
   data:=resptr[bar];
   bgColor:=colMask[bgColor];
   dataOfs:=0; width:=width-1;
   while (data[dataOfs]<>255) do
   begin
      scrOfs:=data[dataOfs]; dataOfs:=dataOfs+1;
      for j:=0 to width-1 do
      begin
         v:=data[dataOfs];
         if (v=255) then // entry end
         begin
            if (width=255) then break;
            v:=0;
         end
         else
            dataOfs:=dataOfs+1;

         screen[scrOfs]:=bgColor+v;
         scrOfs:=scrOfs+1;
      end;
      if (width<>255) then
         screen[scrOfs]:=bgColor+$06;

      dataOfs:=dataOfs+1;
   end;
end;

function optionsList:boolean;
begin
   opts:=opts-1;
   menuBar(optTabs,optWidth,color_choice);
   updateBar(optTabs,optWidth,currentOpt,color_choice,color_selected);
   screen2video();
   repeat
      if keyPressed then
      begin
         controlSelectionKeys(key,pckey,ncKey,currentOpt,0,opts);
         case key of
            key_ESC: begin result:=false; break; end;
            key_RETURN: begin result:=true; break; end;
         end;
         updateBar(optTabs,optWidth,currentOpt,color_choice,color_selected);
         screen2video();
      end;
   until false;
end;

function listChoice:shortint;
var
   listShift,listPos:byte;
   listData:array[0..0] of byte;

   procedure listEntry(ey,n:byte);
   var
      scrOfs:byte;
      ofs:word;

   begin
      ofs:=n*width;
      scrOfs:=vadr[ey]+x;
      if showCount then
      begin
         putValue(x,ey,n,2,1);
         scrOfs:=scrOfs+3;
      end;

      move(@listData[ofs],@screen[scrOfs],width);
      colorHLine(x,ey,width+4*byte(showCount),1);
   end;

   procedure updateList();
   var i:byte;

   begin
      for i:=0 to height-1 do listEntry(y+i,listShift+i);
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

   colorHLine(x,y+listPos,width+3*byte(showCount),3);
   kbcode:=255;
   screen2video();

   repeat
      if keyPressed then
      begin
         colorHLine(x,y+listPos,width+3*byte(showCount),1);
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
            key_ESC: begin
               result:=-1;
               break;
            end;
            key_RETURN: begin
               result:=listShift+listPos;
               break;
            end;
         end;
         updateList();
         colorHLine(x,y+listPos,width+3*byte(showCount),3);
         screen2video();
      end;
   until false;
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
