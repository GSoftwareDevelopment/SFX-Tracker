procedure updateTABBar();
begin
	menuBar(menu_tabs,width_menuBar,section,color_choice,color_selected);
end;

procedure updateTABInfo();
begin
	__scrOfs:=21; color:=$40; putValue(currentTAB,2);
	color:=$00; strVal2Mem(pointer(word(resptr[menu_tabs])+2),currentTAB,2);

   fillchar(@screen[24],8,0);
   color:=$40;
   __scrOfs:=24; putASCIIText(TABName);
   __scrOfs:=20; colorHLine(13);

	color:=$00;
   putValue(TABLen,3);
   putChar($0f);
   putValue(TABRealLen,3);
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
	color:=0; putValue(currentSFX,2);
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
      color:=$80; putValue(_nxt,3); inc(__scrOfs,2);

		color:=0;
      _fnsfx:=TAB_fnSFX[pos]; _note:=TAB_notes[pos];
      if (_fnsfx=TABFn_end) and (_note=TABFn_end_param) then
      begin
			cmd2scr(33,6); // ENDTAB
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
               putValue(_fnsfx,2);
               inc(__scrOfs,2);
            end;
         end
         else
            if (_fnsfx and $c0=$40) then
            begin // frequency value
               _fnsfx:=_fnsfx and $3f;
               putValue(_note,3);
               putChar($0f); // '/'
               putValue(_fnsfx,2);
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
                     putValue(ch,2);
                     putChar($1e);
                     putValue(_note,3); // note gives position
                     inc(__scrOfs,2);
                  end;
            end;


      realPos:=TAB_RealPos[pos];
      if realPos<>$ffff then
         putValue(realPos,3)
		else
			inc(__scrOfs,3);

      if (pos>=TABLen) then
         color:=$80
      else
      begin
         if showPos and (SONG_Beat>1) then
         begin
            if _beat=0 then color:=$80 else color:=0;
            _beat:=_beat+1; if _beat=SONG_Beat then _beat:=0;
         end
         else
            color:=0;
      end;

      dec(__scrOfs,13); colorHLine(13);

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
end;
