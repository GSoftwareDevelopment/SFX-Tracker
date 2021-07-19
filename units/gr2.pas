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

procedure initGraph(dlAddr,videoAddr,bufferAddr:word);
procedure conv2ASCII(var s:string);
procedure conv2Internal(var s:string);
procedure conv2internalP2P(src,dest:pointer; len:byte);

procedure colorHLine(x,y,width,color:byte);
procedure colorVLine(x,y,height,color:byte);
procedure box(x,y,width,height,cCol:byte);
procedure putText(x,y:byte; var s:string; color:byte);
procedure putNText(x,y:byte; strptr:pointer; color:byte);
procedure putASCIIText(x,y:byte; var s:string; color:byte);
procedure strVal2Mem(_dest:pointer; value:smallint; zeros,color:byte);
procedure putValue(x,y:byte; value:smallint; zeros,color:byte);
procedure putHexValue(x,y,value:byte; color:byte);
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

procedure _colorSet(ofs,steps,step,col:byte);
begin
   while steps>0 do
   begin
      screen[ofs]:=(screen[ofs] and $3f) or col;
      ofs:=ofs+step; steps:=steps-1;
   end;
end;

procedure colorHLine;
begin
   _colorSet(vadr[y]+x,width,1,colMask[color]);
end;

procedure colorVLine;
begin
   _colorSet(vadr[y]+x,height,20,colMask[color]);
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
   _src:=src; _dest:=dest; i:=0;
   while i<len do
   begin
      if _src[i]<32 then _dest[i]:=0 else _dest[i]:=_src[i]-32;
      i:=i+1;
   end;
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

procedure strVal2Mem(_dest:pointer; value:smallint; zeros,color:byte);
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
   ptr:=0; color:=colMask[color]+$10;
   base:=500; step:=100; digit();
   if (zeros>=3) then begin dest[ptr]:=color+a; ptr:=ptr+1; end;

   base:=50; step:=10; digit();
   if (zeros>=2) then begin dest[ptr]:=color+a; ptr:=ptr+1; end;

   base:=5; step:=1; digit();
   if (zeros>=1) then dest[ptr]:=color+a;
end;

procedure putValue;
var
   ptr,a:byte;

begin
   ptr:=x+vadr[y];
   strVal2Mem(@screen[ptr],value,zeros,color);
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
