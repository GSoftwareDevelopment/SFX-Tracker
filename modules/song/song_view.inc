procedure showTABInfo(nTAB:byte);
var
   addr:word;

begin
   if nTAB>=maxTABs then exit;
   addr:=TABPtr[nTAB];
   if addr=$FFFF then exit;

	color:=0;
	__scrOfs:=220; putChar($03); putValue(nTAB,2); putChar($1a);
   conv2internalP2P(pointer(addr-TABNameLength),@statusBar[4],TABNameLength);
   getTABData(nTAB);
   TABDetermineLength();
   __scrOfs:=237; putValue(TABRealLen,3);
end;

procedure updateSONG(indPos:boolean);
var
   i,chn,songOfs,_line,_pos:byte;
   order:byte;

	procedure putDblChar(ch1,ch2:byte);
	begin
		screen[__scrOfs]:=ch1;
		screen[__scrOfs+1]:=ch2;
		inc(__scrOfs,3);
	end;

begin
   clearWorkarea();
   __scrOfs:=44; songOfs:=SONGShift*4; _line:=SONGShift;
   _pos:=SONG_Ofs shr 2;
   for i:=0 to 8 do
   begin
      color:=$80; putValue(_line,2);
      inc(__scrOfs);

      order:=SONGData[songOfs];
      if order<chnOrder then
      begin
			color:=0;
         chn:=0;
         if indPos then
         begin
            if _pos=_line then
               screen[__scrOfs]:=$1e;
         end;
         inc(__scrOfs);
         repeat
            if order<maxTABs then
            begin
               putValue(order,2); inc(__scrOfs);
            end
            else
            begin
               if order=chnBlank then
						putDblChar($3f,$3f) // __
               else
						if order=chnOff then
							putDblChar($3c,$3c); // ==
            end;
            inc(songOfs); order:=SONGData[songOfs];
            inc(chn);
         until chn=4;
         inc(__scrOfs,4);
      end
      else
      begin
         inc(__scrOfs);
         case Order of
            chnOrder_Tempo: begin
						putDblChar($34,$1a); // T:
                  putValue(SONGData[songOfs+1],2);
                  inc(__scrOfs,11);
               end;
            chnOrder_JumpTo: begin
						putDblChar($2a,$1e); // J>
                  putValue(SONGData[songOfs+1],2);
                  inc(__scrOfs,11);
               end;
            chnOrder_Repeat: begin
						putDblChar($32,$1a); // R:
                  putValue(SONGData[songOfs+1],2);
                  putChar($1e); // >
                  putValue(SONGData[songOfs+2],2);
                  inc(__scrOfs,8);
               end;
            chnOrder_EndSong: begin
                  move(resptr[str_EndSONGOrder],@screen[__scrOfs],8); // 8 - hardcoded string length!
                  inc(__scrOfs,16);
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
   ch,chn,pos:byte;

	procedure putBlackValue();
	begin
		putChar($0d); putChar($0d);
	end;

begin
// 01234567890123456789
// T00 R00 00 00 00 00
	__scrOfs:=220; // status bar offset;

	putChar($34); color:=0;
   putHexValue(SONG_Tempo);
	inc(__scrOfs);

   putChar($32);
   if SONG_RepCount>0 then
      putHexValue(SONG_RepCount)
   else
		putBlackValue();
	inc(__scrOfs);

   chn:=$00;
   repeat
      pos:=channels[chn+_tabOfs];
      if pos<>$ff then
         putHexValue(pos)
      else
			putBlackValue();

      if channels[chn+_tabRep]>0 then
         ch:=$09
      else
         ch:=$00;
		putChar(ch);
      inc(chn,$10);
   until chn=$40;
end;

procedure SONGScreen();
begin
   clearModule();
   VBar(0,1,width_menuBar,10,0);
   menuBar(menu_song,width_menuBar,0,1,1);
   updateSONG(true);
end;
