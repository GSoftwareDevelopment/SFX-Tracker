procedure showTABInfo(nTAB:byte);
var
   addr:word;

begin
   if nTAB>=maxTABs then exit;
   addr:=TABPtr[nTAB];
   if addr=$FFFF then exit;

   statusBar[0]:=$03;
   putValue(1,11,nTAB,2,0);
   statusBar[3]:=$1a;
   conv2internalP2P(pointer(addr-TABNameLength),@statusBar[4],TABNameLength);
   getTABData(nTAB);
   TABDetermineLength();
   putValue(17,11,TABRealLen,3,0);
end;

procedure updateSONG(indPos:boolean);
var
   i,chn,scrOfs,songOfs,_line,_pos:byte;
   order:byte;

begin
   scrOfs:=44; songOfs:=SONGShift*4; _line:=SONGShift;
   clearWorkarea();
   for i:=0 to 8 do
   begin
      putValue(scrOfs,0,_line,2,2);
      scrOfs:=scrOfs+3;

      order:=SONGData[songOfs];
      if order<chnOrder then
      begin
         chn:=0;
         if indPos then
         begin
            _pos:=SONG_Ofs shr 2;
            if _pos=_line then
               screen[scrOfs]:=$1e;
         end;
         repeat
            scrOfs:=scrOfs+1;
            if order<maxTABs then
               putValue(scrOfs,0,order,2,0)
            else
            begin
               if order=chnBlank then
               begin
                  screen[scrOfs]:=$3f;
                  screen[scrOfs+1]:=$3f;
               end
               else
               if order=chnOff then
               begin
                  screen[scrOfs]:=$3c;
                  screen[scrOfs+1]:=$3c;
               end
            end;
            scrOfs:=scrOfs+2;
            songOfs:=songOfs+1;
            order:=SONGData[songOfs];
            chn:=chn+1;
         until chn=4;
         scrOfs:=scrOfs+5;
      end
      else
      begin
         scrOfs:=scrOfs+1;
         case Order of
            chnOrder_Tempo: begin
                  screen[scrOfs  ]:=$34; // T
                  screen[scrOfs+1]:=$1a; // :
                  scrOfs:=scrOfs+3;
                  putValue(scrOfs,0,SONGData[songOfs+1],2,0);
                  scrOfs:=scrOfs+13;
               end;
            chnOrder_JumpTo: begin
                  screen[scrOfs  ]:=$2a; // J
                  screen[scrOfs+1]:=$1e; // >
                  scrOfs:=scrOfs+3;
                  putValue(scrOfs,0,SONGData[songOfs+1],2,0);
                  scrOfs:=scrOfs+13;
               end;
            chnOrder_Repeat: begin
                  screen[scrOfs  ]:=$32; // R
                  screen[scrOfs+1]:=$1a; // :
                  scrOfs:=scrOfs+3;
                  putValue(scrOfs,0,SONGData[songOfs+1],2,0); scrOfs:=scrOfs+2;
                  screen[scrOfs]:=$1e; scrOfs:=scrOfs+1; // >
                  putValue(scrOfs,0,SONGData[songOfs+2],2,0);
                  scrOfs:=scrOfs+10;
               end;
            chnOrder_EndSong: begin
                  move(resptr[str_EndSONGOrder],@screen[scrOfs],8); // 8 - hardcoded string length!
                  scrOfs:=scrOfs+16;
               end;
         end;
         songOfs:=songOfs+4;
      end;
      _line:=_line+1;
   end;
   conv2internalP2P(@SONGTitle[1],@moduleBar,20);
end;

procedure updateSongStatus();
var
   x,chn,pos:byte;

begin
// 01234567890123456789
// T00 R00 00 00 00 00
   statusBar[0]:=$34;
   putHexValue(1,11,SONG_Tempo,0);

   statusBar[4]:=$32;
   if SONG_RepCount>0 then
      putHexValue(5,11,SONG_RepCount,0)
   else
   begin
      statusBar[5]:=$0d;
      statusBar[6]:=$0d;
   end;

   x:=8; chn:=$00;
   repeat
      pos:=channels[chn+_tabOfs];
      if pos<>$ff then
      begin
         pos:=pos shr 1;
         putHexValue(x,11,pos,0);
         inc(x,2);
      end
      else
      begin
         statusBar[x]:=$0d; inc(x);
         statusBar[x]:=$0d; inc(x);
      end;
      if channels[chn+_tabRep]>0 then
         statusBar[x]:=$09 // R
      else
         statusBar[x]:=$00;
      inc(x,1);
      inc(chn,$10);
   until chn=$40;
end;

procedure SONGScreen();
begin
   clearModule();
   VBar(0,1,width_menuBar,10,0);
   menuBar(menu_song,width_menuBar,1);
   updateSONG(true);
end;
