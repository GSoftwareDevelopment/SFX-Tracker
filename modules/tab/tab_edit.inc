{$i modules/tab/tab_entry_edit.inc}

procedure TABEditLoop();
var
   _pos,i,cTAB:byte;
   nSFX:shortint;

   procedure saveIfModified();
   begin
      if modified then
      begin
         TABDetermineLength();
         storeTABData(currentTAB);
         modified:=false;
      end;
   end;

   procedure drawCursor();
   begin
      colorHLine(winXStart,2+cursorPos,20-winXStart,3);
   end;

begin
   drawCursor();
   screen2video();
   reset_pianoVis();
   cTAB:=currentTAB;
   repeat
      if keyPressed then
      begin
         _pos:=cursorShift+cursorPos;
         if controlSelectionKeys(key,key_LEFT,key_RIGHT,currentSFX,0,maxSFXs-1) then
            updateTABSFX();
         if controlSelectionKeys(key,key_CTRL_LEFT,key_CTRL_RIGHT,cTAB,0,maxTABs-1) then
         begin
            saveIfModified();
            currentTab:=cTab;
            getTABData(currentTAB);
            TABDetermineLength();
         end;
         case key of
            key_ESC: break;
            key_CTRL_RETURN:begin
               updateTABSFX();
               nSFX:=dataList(SFXPtr,currentSFX,maxSFXs,SFXNameLength);
               if (nSFX<>-1) and (nSFX<>currentSFX) then
                  currentSFX:=nSFX;
               updateTABSFX();
            end;
            key_CTRL_INSERT: begin
               i:=127-_pos;
               TABdblMove(_pos,0,1);
               TABputIn(_pos,TABFn_blank_param,TABFn_blank);
            end;
            key_CTRL_DELETE: begin
               i:=127-_pos;
               TABdblMove(_pos,1,0);
               TABputIn(127,TABFn_blank_param,TABFn_blank);
            end;
            key_SPACE: begin
               SFX_ChannelOff(0);
               TABputIn(_pos,TABFn_blank_param,TABFn_blank);
               moveCursor(1,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift);
            end;
            key_RETURN: if TABEntryEdit then break;
         end;

         controlEditKeys(key,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift,key_Up,key_Down);

         i:=pianoKeyScan(0);
         if i<>255 then
         begin
            TABputIn(_pos,i,currentSFX);
            moveCursor(1,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift);
         end;

         if modified then TABDetermineLength();

         //
         updateTAB(true);
         drawCursor();
         updateTABInfo();
         screen2video();
      end;
      updatePianoVis();
   until false;
   updateTAB(true);
   SFX_Off();
   saveIfModified();
   reset_pianoVis();
end;
