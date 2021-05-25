unit GR2;

interface
const
	vadr:array[0..11] of byte = (0,20,40,60,80,100,120,140,160,180,200,220);
	colMask:array[0..3] of byte = ($00,$40,$80,$c0);

var
	screen:array[0..0] of byte;
	video:array[0..0] of byte;
	DLIST:word absolute 560;
	VCOUNT:byte absolute $d40b;
	PFCols:array[0..4] of byte absolute 708;

procedure initGraph(dlAddr,videoAddr,bufferAddr:word);
function getTime():longint;
procedure conv2ASCII(var s:string);
procedure conv2Internal(var s:string);
procedure conv2internalP2P(src,dest:pointer; len:byte);

procedure colorHLine(x,y,width,color:byte);
procedure colorVLine(x,y,height,color:byte);
procedure box(x,y,width,height,cCol:byte);
procedure putText(x,y:byte; var s:string; color:byte);
procedure putNText(x,y:byte; strptr:pointer; color:byte);
procedure putASCIIText(x,y:byte; var s:string; color:byte);
procedure putValue(x,y:byte; value:longint; zeros,color:byte);
procedure putHexValue(x,y,value:byte; color:byte);
procedure wait4screen();
procedure screen2video();

implementation
procedure initGraph;
begin
	DLIST:=dlAddr;
	video:=pointer(videoAddr);
	screen:=pointer(bufferAddr);
end;

procedure conv2Internal;
var i:byte;

begin
	for i:=1 to length(s) do
		s[i]:=char(byte(s[i])-32);
end;

procedure conv2ASCII;
var i:byte;

begin
	for i:=1 to length(s) do
		s[i]:=char(byte(s[i])+32);
end;

function getTime:longint;
var
	time:array[0..2] of byte absolute $12;

begin
  result:=time[0] shl 16+time[1] shl 8+time[2];
end;

procedure colorHLine;
var ofs:byte;

begin
	ofs:=vadr[y]+x;
	color:=colMask[color];
	while width>0 do
	begin
		screen[ofs]:=color+(screen[ofs] and $3f);
		ofs:=ofs+1; width:=width-1;
	end;
end;

procedure colorVLine;
var ofs,i:byte;

begin
	ofs:=vadr[y]+x;
	color:=colMask[color];
	while height>0 do
	begin
		screen[ofs]:=color+(screen[ofs] and $3f);
		ofs:=ofs+20; height:=height-1;
	end;
end;

procedure box;
var
	scrOfs:byte;

begin
	scrOfs:=vadr[y]+x;
	while height>0 do
	begin
		fillchar(@screen[scrOfs],width,cCol);
		height:=height-1; scrOfs:=scrOfs+20;
	end;
end;

procedure conv2internalP2P;
var i:byte;
	_src,_dest:array[0..0] of byte;

begin
	_src:=src; _dest:=dest;
	for i:=0 to len-1 do
		if _src[i]<32 then _dest[i]:=0 else _dest[i]:=_src[i]-32;
end;

procedure putText;
var
	scrofs,i:byte;

begin
	scrofs:=vadr[y]+x;
	i:=1; color:=colMask[color];
	while i<=length(s) do
	begin
		screen[scrofs]:=byte(s[i]) or color;
		scrofs:=scrofs+1; i:=i+1;
	end;
end;

procedure putASCIIText;
var
	scrofs,i:byte;

begin
	scrofs:=vadr[y]+x;
	i:=1; color:=colMask[color];
	while i<=length(s) do
	begin
		screen[scrofs]:=(byte(s[i])-32) or color;
		scrofs:=scrofs+1; i:=i+1;
	end;
end;

procedure putNText;
var
	scrofs,i:byte;
	ns:array[0..0] of byte;

begin
	ns:=strptr;
	scrofs:=vadr[y]+x;
	i:=0; color:=colMask[color];
	while ns[i]<>255 do
	begin
		screen[scrofs]:=ns[i] or color;
		scrofs:=scrofs+1; i:=i+1;
	end;
end;

procedure putValue;
var
	ptr,a:byte;

begin
	ptr:=x+vadr[y]; color:=colMask[color]+$10;
	if (zeros=3) then begin
		if (value>=200) then
		begin
			a:=2; value:=value-200;
		end
		else
			if (value>=100) then
			begin
				a:=1; value:=value-100;
			end
			else
				a:=0;
		screen[ptr]:=color+a; ptr:=ptr+1;
	end;
	if (value>=50) then begin a:=5; value:=value-50; end else a:=0;
	if (value>=40) then begin a:=a+4; value:=value-40; end
	else
		if (value>=30) then begin a:=a+3; value:=value-30; end
		else
			if (value>=20) then begin a:=a+2; value:=value-20; end
			else
				if (value>=10) then begin a:=a+1; value:=value-10; end;
	screen[ptr]:=color+a; ptr:=ptr+1;
	if (value>=5) then begin a:=5; value:=value-5; end else a:=0;
	if (value>=4) then begin a:=a+4; value:=value-4; end
	else
		if (value>=3) then begin a:=a+3; value:=value-3; end
		else
			if (value>=2) then begin a:=a+2; value:=value-2; end
			else
				if (value>=1) then begin a:=a+1; value:=value-1; end;
	screen[ptr]:=color+a;
End;

procedure putHexValue;
var
	scrOfs,v:byte;

begin
	color:=colMask[color];
	scrOfs:=vadr[y]+x;
	v:=value shr 4;
	if (v<10) then
		screen[scrOfs]:=$10+v
	else
		screen[scrOfs]:=$17+v;
	scrOfs:=scrOfs+1;

	v:=value and $0f;
	if (v<10) then
		screen[scrOfs]:=$10+v
	else
		screen[scrOfs]:=$17+v;
end;

procedure wait4screen;
begin
	repeat until VCOUNT=0;
end;

procedure screen2video;
begin
	repeat until VCOUNT=0;
	move(@screen,@video,240);
end;

end.
