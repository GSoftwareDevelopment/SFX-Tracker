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
		key_SHIFT_TAB = 108,
		key_CTRL_Left = 134,
		key_CTRL_Right = 135,
		key_CTRL_Up = 142,
		key_CTRL_Down = 143,
		key_CTRL_BackSpc = 180,
		key_DELETE = 180,
		key_INSERT = 183,

		key_SHIFT_Up = 206,
		key_SHIFT_Down = 207
);

const
	color_white			= 0;
	color_choice		= 1;
	color_background	= 2;
	color_selected		= 3;

	chars_alphaNum:string[49] = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ. !#%*-/:<>?_'~;
	keys_alphaNum:array[0..48] of byte = (
		50,31,30,26,24,29,27,51,53,48,	// 0-9
		63,21,18,58,42,56,61,57,13,1,5,0,37,35,8,10,47,40,62,45,11,16,46,22,43,23,	// A-Z
		34,	// . (dot)
		33,		// space
		95,	// exclamation mark (!)
		90,	// hash (#)
		93,	// percent (%)
		7,		// star (*)
		14,	// hypen (-)
		38,	// slash (/)
		66,	// colon (:)
		54,	// less sign (<)
		55,	// more sign (>)
		102,	// question mark (?)
		78		// underscore mark (_)
	);

	keysRange_all			= 49;
	keysRange_hexNum		= 16;
	keysRange_decNum		= 10;

var
	KBCODE:byte absolute 764;
	key:TKeys;
	timer:byte absolute $14;

function keyScan(key2Scan:byte; var keyDefs:byteArray; keysRange:byte):byte;
function controlSelectionKeys(var keyIn:byte; decKey,incKey:byte; var value:byte; min,max:byte):boolean;
procedure moveCursor(ofs:shortint; winSize,overSize:byte; var curPos,curShift:byte);
function inputText(x,y,width:byte; var s:string; colEdit,colOut:byte):boolean;
function inputLongText(x,y,width:byte; maxLen:byte; var s:string; colEdit,colOut:byte):boolean;
function inputValue(x,y,width:byte; var v:smallint; min,max:smallint; colEdit,colOut:byte):boolean;
procedure putMultiText(bar:pointer; bgColor:byte);
procedure VBar(x,y,width,col:byte);
procedure updateBar(bar:pointer; width:byte; currentSel:shortint; canChoiceColor,choicedColor:byte);
procedure menuBar(bar:pointer; width,bgColor:byte);
function optionsList(optTabs:pointer; optWidth:byte; opts:shortint; var currentOpt:shortint; pcKey,ncKey:byte):boolean;
function listChoice(x,y,width,height,defaultPos:byte; listPtr:pointer; listSize:byte; showCount:boolean):shortint;
function messageBox(msgPtr:pointer; msgColor:byte;	menuPtr:pointer; menuWidth,menuOpts:byte; defaultOpt:shortint; pcKey,ncKey:byte):byte;

implementation
uses gr2;

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
		if (_pos>=0) and (_pos<winSize) then
			curPos:=_pos
		else
		begin
			if (_pos<0) then
				curPos:=0
			else
				curPos:=winSize-1;

			_pos:=curShift+ofs;
			if (_pos>=0) and (_pos<=overSize-winSize) then
				curShift:=_pos
			else
				if (_pos<0) then
					curShift:=0
				else
					curShift:=overSize-winSize;
		end;
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
	conv2internal(s);
	move(@s[1],@buf,len);
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
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode);
			ofs:=shiftX+curX;
			screen[scrOfs+curX]:=buf[ofs] or colEdit;
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
						len:=len-1;	buf[len]:=0;
						ofs:=ofs-1;
					end
			end;

			if (ofs<maxLen) then
			begin
				i:=keyScan(key,keys_alphaNum,keysRange_all);
				if (i<>255) then
				begin
					move(buf[ofs],buf[ofs+1],len-ofs);
					ch:=byte(chars_alphaNum[i+1]);
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
		if len>0 then move(@buf,@s[1],len);
		move(@buf,@screen[scrOfs],width);
	end
	else
		move(@s[1],@screen[scrOfs],width);
	conv2ASCII(s);
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

procedure VBar(x,y,width,col:byte);
var i,ofs:byte;

begin
	col:=colMask[col]; width:=width-1;
	ofs:=x+vadr[y];
	for i:=y to 10 do
	begin
		fillchar(@screen[ofs],width,col);
		screen[ofs+width]:=$06+col;
		ofs:=ofs+20;
	end;
end;

procedure updateBar;
var
	i,j,v:byte;
	dataOfs,col:byte;
	scrOfs:word;
	data:byteArray;

begin
	data:=bar;
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
	data:=bar;
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
	screen2video(); kbcode:=255;
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode);
			controlSelectionKeys(key,pckey,ncKey,currentOpt,0,opts);
			case key of
				key_ESC:	begin result:=false; break; end;
				key_RETURN: begin result:=true; break;	end;
			end;
			updateBar(optTabs,optWidth,currentOpt,color_choice,color_selected);
			screen2video();
			kbcode:=255;
		end;
	until false;
	kbcode:=255;
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
		for i:=0 to 8 do listEntry(y+i,listShift+i);
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
		if (kbcode)<>255 then
		begin
			colorHLine(x,y+listPos,width+3*byte(showCount),1);
			key:=TKeys(kbcode);
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
			kbcode:=255;
		end;
	until false;
	kbcode:=255;
end;

function messageBox:byte;
begin
	putMultiText(msgPtr,msgColor);
	optionsList(menuPtr,menuWidth,menuOpts,defaultOpt,pcKey,ncKey);
	result:=defaultOpt;
end;

end.
