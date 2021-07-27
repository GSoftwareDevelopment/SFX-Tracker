procedure updateTABBar();
begin
	menuBar(menu_tabs,width_menuBar,section,color_choice,color_selected);
end;

procedure updateTABInfo();
begin
   __scrOfs:=21; putValue(currentTAB,2,1);
	strVal2Mem(pointer(word(resptr[menu_tabs])+2),currentTAB,2,0);

   fillchar(@screen[24],8,0);
   __scrOfs:=24; putASCIIText(TABName,1);
   __scrOfs:=20; colorHLine(13,1);

   putValue(TABLen,3,0);
   putChar($0f);
   putValue(TABRealLen,3,0);
end;

procedure updateTABSFX();
var addr:word;
   tptr:pointer;

begin
   clearStatusBar();
   addr:=SFXPtr[currentSFX];
   if (addr<$ffff) then
      tptr:=pointer(addr-SFXNameLength)
   else
      tptr:=resptr[str_notDefined];
   conv2internalP2P(tptr,@statusBar[4],SFXNameLength);

   __scrOfs:=220;
   putChar($03);
   putValue(currentSFX,2,0);
   putChar($1a);
   updatePiano();
end;

procedure updateTAB(showPos:boolean);
var
   i,_note,_fnsfx,noteOfs,noteOct,pos:byte;
   ch,_beat,_nxt:byte;
   realPos,addr:word;

	procedure cmd2scr(cmd,len:byte);
	begin
		move(note_names[cmd],@screen[__scrOfs],len);
		inc(__scrOfs,len);
	end;

begin
   if song_beat>1 then
      _beat:=cursorShift mod song_beat;

   _nxt:=cursorShift;
   pos:=cursorShift; _fnsfx:=0;
   clearWorkarea();
   __scrOfs:=44;
   for i:=2 to 10 do
   begin
      putValue(_nxt,3,2); inc(__scrOfs,2);

      _fnsfx:=TAB_fnSFX[pos]; _note:=TAB_notes[pos];
      if (_fnsfx=TABFn_end) and (_note=TABFn_end_param) then
      begin
			cmd2scr(33,6); // ENDTAB;
         inc(__scrOfs,2);
      end
      else
         if (_fnsfx and $c0=$00) then
         begin // note
            if SFXModMode[_fnsfx]=3 then
            begin
               dec(__scrOfs);
               addr:=SFXPtr[_fnsfx]-SFXNameLength;
               conv2internalP2P(pointer(addr),@screen[__scrOfs],8);
               inc(__scrOfs,9);
            end
            else
            begin
               noteOct:=_note div 12; noteOfs:=(_note mod 12) shl 1;

               // NOTE#
               cmd2scr(noteOfs,2);
               if (noteOct<=9) then ch:=$10+noteOct (* octave *) else ch:=$0d; // hypen sign
               putchar(ch);
               inc(__scrOfs);

               // SFX#
               putValue(_fnsfx,2,0);
               inc(__scrOfs,2);
            end;
         end
         else
            if (_fnsfx and $c0=$40) then
            begin // frequency value
               _fnsfx:=_fnsfx and $3f;
               putValue(_note,3,0);
               putChar($0f); // '/'
               putValue(_fnsfx,2,0);
               __scrOfs:=__scrOfs+2;
            end
            else // function
            begin
               if (_fnsfx and $40=$40) then
               begin
                  if (_note and $80=TABFn_noteOff_param) then
                     ch:=45   // === .. (note off)
                  else
                     ch:=39;  // ... .. (blank)
                  cmd2scr(ch,6);
                  inc(__scrOfs,2);
               end
               else
                  if (_fnsfx and $3f>0) then
                  begin // repeat
                     ch:=(_fnsfx-128); // repeat times is value from 1 to 9
                     putValue(ch,2,0);
                     putChar($1e);
                     putValue(_note,3,0); // note gives position
                     inc(__scrOfs,2);
                  end;
            end;

      if (pos>=TABLen) then
         ch:=2
      else
      begin
         if showPos and (SONG_Beat>1) then
         begin
            if _beat=0 then ch:=2 else ch:=0;
            _beat:=_beat+1; if _beat=SONG_Beat then _beat:=0;
         end
         else
            ch:=0;
      end;

      realPos:=TAB_RealPos[pos];
      if realPos<>$ffff then
         putValue(RealPos,3,2)
		else
			inc(__scrOfs,3);

      dec(__scrOfs,13); colorHLine(13,ch);

      inc(pos);
      inc(__scrOfs,4);
      inc(_nxt);
   end;
end;

procedure TABScreen();
begin
   clearModule();
   VBar(0,1,width_menuBar,10,0);
   screen[32]:=$06;

   menuBar(menu_tabs,width_menuBar,section,1,1);

   getTABData(currentTAB);
   TABDetermineLength();
   updateTABInfo();
   updateTAB(true);
   updateTABBar();

	screen2video();
end;
