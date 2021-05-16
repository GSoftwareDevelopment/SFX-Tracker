unit UI;

interface

type
	TKeys = (
		key_Left = 6,
		key_Right = 7,
		key_RETURN = 12,
		key_Up = 14,
		key_Down = 15,
		key_ESC = 28,
		key_SPACE = 33,
		key_TAB = 44,
		key_BackSpc = 52,
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

	chars_alphaNum:string[38] = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ. '~;
	keys_alphaNum:array[0..37] of byte = (
		50,31,30,26,24,29,27,51,53,48,	// 0-9
		63,21,18,58,42,56,61,57,13,1,5,0,37,35,8,10,47,40,62,45,11,16,46,22,43,23,	// A-Z
		34,	// . (dot)
		33		// space
	);

	keysRange_all			= 38;
	keysRange_hexNum		= 16;
	keysRange_decNum		= 10;

var
	KBCODE:byte absolute 764;
	key:TKeys;

function keyScan(key2Scan:byte; var keyDefs:array[0..0] of byte; keysRange:byte):byte;
procedure moveCursor(ofs:shortint; winSize,overSize:smallint; var curPos,curShift:smallint);
function inputText(x,y,width:byte; var s:string; colEdit,colOut:byte):boolean;
function inputValue(x,y,width:byte; var v:integer; min,max:integer; colEdit,colOut:byte):boolean;
procedure VBar(x,y,width,col:byte);
procedure updateBar(bar:pointer; width:byte; currentSel:shortint; canChoiceColor,choicedColor:byte);
procedure menuBar(bar:Pointer; width,bgColor:byte);
procedure optionsList(optTabs:pointer; optWidth:byte; opts:shortint; var currentOpt:shortint);
function listChoice(x,y,width,height,defaultPos:byte; listPtr:pointer; listSize:byte; showCount:boolean):shortint;

implementation
uses gr2;

function keyScan:byte;
var
	keyOfs:byte;

begin
	result:=255;
	for keyOfs:=0 to keysRange-1 do
	begin
		if (keyDefs[keyOfs]=key2Scan) then
		begin
			result:=keyOfs;
			break;
		end;
	end;
end;

procedure moveCursor;
var _pos:smallint;

begin
	_pos:=curPos+ofs;
	if (_pos>-1) and (_pos<winSize) then
		curPos:=_pos
	else
	begin
		if (_pos<0) then
			curPos:=0
		else
			curPos:=winSize-1;

		_pos:=curShift+ofs;
		if (_pos>-1) and (_pos<overSize-winSize) then
			curShift:=_pos
		else
			if (_pos<0) then
				curShift:=0
			else
				curShift:=overSize-winSize;
	end;
end;

function inputText:boolean;
var
	i,curX,ch,ofs:byte;
	tm:longint;
	firstKey,curState:boolean;

begin
	curX:=length(s)+1; tm:=getTime; curState:=false; firstKey:=true;
	if (curX>width) then curX:=width;
	colorHLine(x,y,width,colEdit);
	putText(x,y,s,colEdit);
	ofs:=vadr[y]+x-1; // -1 becouse curX is counting from 1!
	colEdit:=colMask[colEdit];
	screen2video();
	repeat
		if (getTime-tm>=10) then
		begin
			curState:=not curState;
			if (curState) then
				ch:=$3c
			else
				ch:=byte(s[curX]);
			screen[ofs+curX]:=colEdit+ch;
			screen2video();
			tm:=getTime;
		end;
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode);
			screen[ofs+curX]:=byte(s[curX]) or colEdit;
			case key of
				key_ESC: begin result:=false; break; end;
				key_RETURN: begin result:=true; break; end;
				key_Left:
					if (curX>1) then curX:=curX-1;
				key_Right:
					if (curX<width) then curX:=curX+1;
				key_BackSpc:
				begin
					s[curX]:=#0;
					screen[ofs+curX]:=$00 or colEdit;
					if (curX>1) then curX:=curX-1;
				end;
			end;

			i:=keyScan(key,keys_alphaNum,keysRange_all);
			if (i<>255) then
			begin
				ch:=byte(chars_alphaNum[i+1]);
				if (firstKey) then
				begin
					curX:=1;
					fillchar(@s,width+1,0);
					fillchar(screen[ofs+1],width,colEdit);
					firstKey:=false;
				end;
				s[curX]:=char(ch);
				screen[ofs+curX]:=ch or colEdit;
				if (curX<width) then
					curX:=curX+1;
			end;
			screen2video();
			kbcode:=255; tm:=0; curState:=false;
		end;
	until false;
	i:=width;
	while (i>0) and (s[i]=#0) do i:=i-1;
	s[0]:=char(i);
	colorHLine(x,y,width,colOut);
	kbcode:=255;
end;

function inputValue(x,y,width:byte; var v:integer; min,max:integer; colEdit,colOut:byte):boolean;
var
	s,o:string[3];
	err:byte;
	ok:boolean;

begin
	result:=false;
	repeat
		str(v,s); o:=concat(StringOfChar('0',width-length(s)),s);
		conv2Internal(o);
		ok:=inputText(x,y,width,o,colEdit,colOut);
		if not ok then exit;
		conv2ASCII(o);
		val(o,v,err);
		if (v<min) or (v>max) then err:=255;
	until err=0;
	result:=true;
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

procedure updateBar(bar:pointer; width:byte; currentSel:shortint; canChoiceColor,choicedColor:byte);
var
	i,j,v:byte;
	dataOfs,col:byte;
	scrOfs:word;
	data:array[0..0] of byte;

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

procedure menuBar(bar:Pointer; width,bgColor:byte);
var
	i,j,v:byte;
	dataOfs:byte;
	scrOfs:word;
	data:array[0..0] of byte;

begin
	data:=bar;
	bgColor:=colMask[bgColor];
	dataOfs:=0; i:=0; width:=width-1;
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

		i:=i+1; dataOfs:=dataOfs+1;
	end;
end;

procedure optionsList(optTabs:pointer; optWidth:byte; opts:shortint; var currentOpt:shortint);
begin
	opts:=opts-1;
	menuBar(optTabs,optWidth,color_choice);
	updateBar(optTabs,optWidth,currentOpt,color_choice,color_selected);
	screen2video(); kbcode:=255;
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode);
			case key of
				key_ESC: begin
					currentOpt:=-1;
					break;
				end;
				key_Up: if (currentOpt>0) then currentOpt:=currentOpt-1 else currentOpt:=opts;
				key_Down: if (currentOpt<opts) then currentOpt:=currentOpt+1 else currentOpt:=0;
				key_RETURN: begin
					kbcode:=255;
					break;
				end;
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
	listShift:smallint;
	listPos:smallint;
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
					result:=defaultPos;
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

end.
