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

	procedure setShortcutSFX(SFXKey:byte);
	begin
		i:=SFX_shortcut[SFXKey];
		if i<>255 then
		begin
			currentSFX:=i; nSFX:=i;
			updateTABSFX();
		end;
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
  			i:=controlSFXKeys; if i<>255 then setShortcutSFX(i);

         if controlSelectionKeys(key,key_LEFT,key_RIGHT,currentSFX,0,maxSFXs-1) then
            updateTABSFX();					// change the current SFX
         if controlSelectionKeys(key,key_SHIFT_LEFT,key_SHIFT_RIGHT,cTAB,0,maxTABs-1) then
         begin										// change the currently edited TAB
            saveIfModified();
            currentTab:=cTab;
            getTABData(currentTAB);
            TABDetermineLength();
         end;

			if key=138 (* CTRL+P *) then
			begin
				saveIfModified();
				TAB_Play(_pos shl 1);
			end;

         case key of
            key_ESC: break;
            key_CTRL_RETURN:begin			// list of SFXs
               updateTABSFX();
               nSFX:=dataList(SFXPtr,currentSFX,maxSFXs,SFXNameLength);
               if (nSFX<>-1) and (nSFX<>currentSFX) then
                  currentSFX:=nSFX;
               TABScreen();
               updateTABSFX();
            end;
            key_CTRL_INSERT: begin			// insert row in TAB
               i:=127-_pos;
               TABdblMove(_pos,0,1);
               TABputIn(_pos,TABFn_blank_param,TABFn_blank);
            end;
            key_CTRL_DELETE: begin			// delete row from TAB
               i:=127-_pos;
               TABdblMove(_pos,1,0);
               TABputIn(127,TABFn_blank_param,TABFn_blank);
            end;
            key_SPACE: begin					// clear row (put blank entry)
               SFX_ChannelOff(0);
               TABputIn(_pos,TABFn_blank_param,TABFn_blank);
               moveCursor(1,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift);
            end;
            key_RETURN: if TABEntryEdit then break;	// call TAB functions list

            key_CTRL_X: Cut2Clipboard(false,TABLen);
            key_CTRL_C: Copy2Clipboard(false,TABLen);
            key_CTRL_V: PasteClipboard(false);
         end;

         controlEditKeys(key,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift,key_Up,key_Down);

         i:=pianoKeyScan(0);
         if i<>255 then
         begin											// write note in TAB
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
