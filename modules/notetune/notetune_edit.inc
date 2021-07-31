procedure NoteTuneLoop();
var
   _pos,notePos,noteShift,noteVal,noteKey,noteOct:byte;

   procedure drawCursor();
   begin
      __scrOfs:=tuneBarsPos+notePos; colorVLine(5,3);
   end;

begin
   notePos:=0; _pos:=0;
   noteOct:=currentOct;
   definedNoteTune:=pointer(NOTE_TABLE_ADDR+currentNoteTableOfs);

   reset_pianoVis();
   updateNoteTune(noteOct);
   drawCursor();
   screen2video();
   repeat
      if keypressed then
      begin
         if not controlSelectionKeys(key_Left,key_Right,_pos,0,63) then
         begin
            noteKey:=pianoKeyScan(0);
            if noteKey<>255 then
               _pos:=noteKey;
         end;

         noteOct:=_pos div 12;
         noteShift:=octShift[noteOct];
         notePos:=_pos mod 12;

         noteVal:=definedNoteTune[_pos];
         if controlSelectionKeys(key_Up,key_Down,noteVal,0,255) then
            definedNoteTune[_pos]:=noteVal;

         updateNoteTune(noteOct);
         drawCursor();
         screen2video();
      end;
      updatePianoVis();
   until checkEscape;
   updateNoteTune(noteOct);
   SFX_Off();
   reset_pianoVis();
end;
