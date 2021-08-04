unit GR2;

interface
type
   byteArray=array[0..0] of byte;

const
   vadr:array[0..11] of byte = (0,20,40,60,80,100,120,140,160,180,200,220);
   colMask:array[0..3] of byte = ($00,$40,$80,$c0);

var
   screen:byteArray;
   video:byteArray;
   DLIST:word absolute 560;
   VCOUNT:byte absolute $d40b;
   PFCols:array[0..4] of byte absolute 708;
   CHBAS:byte absolute 756;
	color:byte = 0;

	__scrOfs:byte;

procedure initGraph(dlAddr,videoAddr,bufferAddr:word);
procedure conv2ASCII(var s:string);
procedure conv2Internal(var s:string);
procedure conv2internalP2P(src,dest:pointer; len:byte);

procedure setPos(x,y:byte);
procedure setColor(nCol:byte);
procedure putChar(ch:byte);
procedure putCChar(ch:byte);
procedure colorHLine(width:byte);
procedure colorVLine(height:byte);
procedure box(width,height:byte);
procedure putText(var s:string);
procedure putNText(strptr:pointer);
procedure putASCIIText(var s:string);
procedure strVal2Mem(_dest:pointer; value:smallint; zeros:byte);
procedure putValue(value:smallint; zeros:byte);
procedure putHexValue(value:byte);
procedure wait4screen();
procedure screen2video();

implementation
procedure initGraph;
begin
   DLIST:=dlAddr;
   video:=pointer(videoAddr);
   screen:=pointer(bufferAddr);
   fillchar(@video,240,0);
   fillchar(@screen,240,0);
   __scrOfs:=0;
end;

procedure conv2Internal;
var i:byte;

begin
   i:=1;
   while i<=length(s) do //for i:=1 to length(s) do
   begin
      s[i]:=char(byte(s[i])-32);
      i:=i+1;
   end;
end;

procedure conv2ASCII;
var i:byte;

begin
   i:=1;
   while i<=length(s) do
   begin
      s[i]:=char(byte(s[i])+32);
      i:=i+1;
   end;
end;

function getTime:longint; assembler;
asm
   mva :rtclok+2 Result
   mva :rtclok+1 Result+1
   mva :rtclok Result+2
   mva #$00 Result+3
end;

procedure setPos(x,y:byte);
begin
	__scrOfs:=vadr[y]+x;
end;

procedure setColor(nCol:byte);
begin
	color:=colMask[nCol];
end;

procedure putChar(ch:byte);
begin
	screen[__scrOfs]:=ch; inc(__scrOfs);
end;

procedure putCChar(ch:byte);
begin
	screen[__scrOfs]:=ch or color; inc(__scrOfs);
end;

procedure _colorSet(steps,step:byte);
begin
   while steps>0 do
   begin
      screen[__scrOfs]:=(screen[__scrOfs] and $3f) or color;
      inc(__scrOfs,step); dec(steps);
   end;
end;

procedure colorHLine;
begin
   _colorSet(width,1);
end;

procedure colorVLine;
begin
   _colorSet(height,20);
end;

procedure box;
begin
   while height>0 do
   begin
      fillchar(@screen[__scrOfs],width,color);
      inc(__scrOfs,20); dec(height);
   end;
end;

procedure conv2internalP2P;
var i:byte;
   _src,_dest:array[0..0] of byte;

begin
   _src:=src; _dest:=dest; i:=0;
   while i<len do
   begin
      if _src[i]<32 then _dest[i]:=0 else _dest[i]:=_src[i]-32;
      i:=i+1;
   end;
end;

procedure putText;
var
   i:byte;

begin
   i:=1;// color:=colMask[color];
   while i<=length(s) do
   begin
      screen[__scrofs]:=byte(s[i]) or color;
      inc(__scrofs); inc(i);
   end;
end;

procedure putASCIIText;
var
   i:byte;

begin
   i:=1;// color:=colMask[color];
   while i<=length(s) do
   begin
      screen[__scrofs]:=(byte(s[i])-32) or color;
      inc(__scrofs); inc(i);
   end;
end;

procedure putNText;
var
   i:byte;
   ns:array[0..0] of byte;

begin
   ns:=strptr;
   i:=0;// color:=colMask[color];
   while ns[i]<>255 do
   begin
      screen[__scrofs]:=ns[i] or color;
      inc(__scrofs); inc(i);
   end;
end;

procedure strVal2Mem(_dest:pointer; value:smallint; zeros:byte);
var
   ptr,a,v:byte;
   dest:array[0..0] of byte;
   base,step:smallint;

   procedure digit();
   begin
      if (value>=base) then begin a:=5; value:=value-base; end else a:=0;
      v:=4;
      while value>=step do
      begin
         dec(base,step);
         if (value>=base) then begin a:=a+v; value:=value-base; exit; end;
         dec(v);
      end;
   end;

begin
   dest:=_dest;
   ptr:=0; inc(color,$10); // color:=colMask[color]+$10;
   base:=500; step:=100; digit();
   if (zeros>=3) then begin dest[ptr]:=color+a; ptr:=ptr+1; end;

   base:=50; step:=10; digit();
   if (zeros>=2) then begin dest[ptr]:=color+a; ptr:=ptr+1; end;

   base:=5; step:=1; digit();
   if (zeros>=1) then dest[ptr]:=color+a;
   dec(color,$10);
end;

procedure putValue;
begin
//   setPos(x,y);
   strVal2Mem(@screen[__scrOfs],value,zeros);
   inc(__scrOfs,zeros);
End;

procedure putHexValue;
var
   v:byte;

	procedure putHexChar();
	begin
		if (v<10) then
			inc(v,$10)
		else
			inc(v,$17);
		screen[__scrOfs]:=v or color;
		inc(__scrOfs);
	end;

begin
//   color:=colMask[color];
   v:=value shr 4; putHexChar();
   v:=value and $0f; putHexChar();
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
