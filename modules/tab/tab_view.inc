procedure updateTABBar();
begin
	updateBar(menu_tabs,width_menuBar,section,color_choice,color_selected);
end;

procedure updateTABInfo();
begin
   putValue(1,1,currentTAB,2,1);
   fillchar(@screen[24],8,0);
   putASCIIText(4,1,TABName,1);
   colorHLine(0,1,13,1);
   putValue(17,1,TABRealLen,3,0);
   moduleBar[16]:=$0f; // '/'
   putValue(13,1,TABLen,3,0);
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

   statusBar[0]:=$03;
   putValue(1,11,currentSFX,2,0);
   statusBar[3]:=$1a;
   updatePiano();
end;

procedure updateTAB(showPos:boolean);
var
   scrOfs,i,_note,_fnsfx,noteOfs,noteOct,pos:byte;
   ch,_beat,_nxt:byte;
   realPos,addr:word;

begin
   scrOfs:=44;
   if song_beat>1 then
      _beat:=cursorShift mod song_beat;

   _nxt:=cursorShift;
   pos:=cursorShift; _fnsfx:=0;
   clearWorkarea();
   for i:=2 to 10 do
   begin
      putValue(scrOfs,0,_nxt,3,2); scrOfs:=scrOfs+5;

      _fnsfx:=TAB_fnSFX[pos]; _note:=TAB_notes[pos];
      if (_fnsfx=TABFn_end) and (_note=TABFn_end_param) then
      begin
         move(note_names[33],@screen[scrOfs],6); // ENDTAB
         scrOfs:=scrOfs+8;
      end
      else
         if (_fnsfx and $c0=$00) then
         begin // note
            if SFXModMode[_fnsfx]=3 then
            begin
               scrOfs:=scrOfs-1;
               addr:=SFXPtr[_fnsfx]-SFXNameLength;
               conv2internalP2P(pointer(addr),@screen[scrOfs],8);
               scrOfs:=scrOfs+9;
            end
            else
            begin
               noteOct:=_note div 12; noteOfs:=(_note mod 12) shl 1;

               // NOTE#
               screen[scrOfs]:=note_names[noteOfs]; noteOfs:=noteOfs+1;
               screen[scrOfs+1]:=note_names[noteOfs];
               scrOfs:=scrOfs+2;
               if (noteOct<=9) then ch:=$10+noteOct else ch:=$0d; // hypen sign
               screen[scrOfs]:=ch;
               scrOfs:=scrOfs+2;

               // SFX#
               putValue(scrOfs,0,_fnsfx,2,0);
               scrOfs:=scrOfs+4;
            end;
         end
         else
            if (_fnsfx and $c0=$40) then
            begin // frequency value
               _fnsfx:=_fnsfx and $3f;
               putValue(scrOfs,0,_note,3,0);
               scrOfs:=scrOfs+3;
               screen[scrOfs]:=$0f; // '/'
               scrOfs:=scrOfs+1;
               putValue(scrOfs,0,_fnsfx,2,0);
               scrOfs:=scrOfs+4;
            end
            else // function
            begin
               if (_fnsfx and $40=$40) then
               begin
                  if (_note and $80=TABFn_noteOff_param) then
                     ch:=45   // === .. (note off)
                  else
                     ch:=39;  // ... .. (blank)
                  move(note_names[ch],@screen[scrOfs],6);
                  scrOfs:=scrOfs+8;
               end
               else
                  if (_fnsfx and $3f>0) then
                  begin // repeat
                     ch:=(_fnsfx-128); // repeat times is value from 1 to 9
                     putValue(scrOfs,0,ch,2,0);
                     scrOfs:=scrOfs+2;
                     screen[scrOfs]:=$1e;
                     scrOfs:=scrOfs+1;
                     putValue(scrOfs,0,_note,3,0); // note gives position
                     scrOfs:=scrOfs+5;
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
         putValue(scrOfs,0,RealPos,3,2);

      colorHLine(winXStart+3,i,13,ch);

      pos:=pos+1;
      scrOfs:=scrOfs+7;
      _nxt:=_nxt+1;
   end;
end;

procedure TABScreen();
begin
   clearModule();
   VBar(0,1,width_menuBar,10,0);
   screen[32]:=$06;

   menuBar(menu_tabs,width_menuBar,1);

   getTABData(currentTAB);
   TABDetermineLength();
   updateTABInfo();
   updateTAB(true);
   updateTABBar();

	screen2video();
end;
