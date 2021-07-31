{$i modules/tab/tab_entry_edit.inc}

procedure TABEditLoop();
var
   _pos,i,cTAB:byte;
   nSFX:shortint;
	editState:boolean;

	procedure TABputIn(_note,_fn:byte);
	begin
		TAB_notes[_pos]:=_note;
		TAB_fnSFX[_pos]:=_fn;
		modified:=true;
	end;

	procedure TABdblMove(ofsSrc,ofsDest:byte);
	var len:byte;

	begin
		len:=127-_pos;
		ofsSrc:=_pos+ofsSrc; ofsDest:=_pos+ofsdest;
		move(@TAB_notes[ofsSrc],@TAB_notes[ofsDest],len);
		move(@TAB_fnSFX[ofsSrc],@TAB_fnsfx[ofsDest],len);
	end;

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
      __scrOfs:=vadr[2+cursorPos]+winXStart;
      if editState then
			colorHLine(16,3)
		else
			colorHLine(16,1);
      screen2video();
   end;

	procedure setShortcutSFX(SFXKey:byte);
	begin
		i:=SFX_shortcut[SFXKey];
		if i<>255 then
		begin
			currentSFX:=i; nSFX:=i;
			updateTABSFX();
		end;
		key:=TKeys(255); // clear keyboard state; prevent to play/write note (under key 2,3,5,6,7,9,0) in TAB
	end;

begin
   reset_pianoVis();
   cTAB:=currentTAB; editState:=true;
   drawCursor();
   repeat
      if keyPressed then
      begin
         _pos:=cursorShift+cursorPos;
  			i:=controlSFXShortcutKeys; if i<>255 then setShortcutSFX(i);

         if controlSelectionKeys(key_LEFT,key_RIGHT,currentSFX,0,maxSFXs-1) then
            updateTABSFX();					// change the current SFX
         if controlSelectionKeys(key_SHIFT_LEFT,key_SHIFT_RIGHT,cTAB,0,maxTABs-1) then
         begin										// change the currently edited TAB
            saveIfModified();
            currentTab:=cTab;
            getTABData(currentTAB);
            TABDetermineLength();
         end;

			if key=138 (* CTRL+P *) then
			begin
				saveIfModified();
				TAB_Play(_pos); // (_pos shl 1);
			end;


         case key of
				key_SPACE: SFX_ChannelOff(0); // turn off SFX
            key_CAPS: begin
                         editState:=not editState;
                         saveIfModified();
                         TABDetermineLength();
                      end;
            key_CTRL_RETURN:begin			// list of SFXs
               updateTABSFX();
               nSFX:=dataList(SFXPtr,currentSFX,maxSFXs,SFXNameLength);
               if (nSFX<>-1) and (nSFX<>currentSFX) then
                  currentSFX:=nSFX;
               TABScreen();
               updateTABSFX();
            end;
            key_CTRL_X: Cut2Clipboard(false,TABLen);
            key_CTRL_C: Copy2Clipboard(false,TABLen);
            key_CTRL_V: PasteClipboard(false);
         end;

         controlEditKeys(TAB_winHeight,TAB_maxLength,cursorPos,cursorShift,key_Up,key_Down);

			if editState then
			begin
				case key of
					key_CTRL_INSERT: begin			// insert row in TAB
						TABdblMove(0,1);
						TABputIn(TABFn_blank_param,TABFn_blank);
					end;
					key_CTRL_DELETE: begin			// delete row from TAB
						TABdblMove(1,0);
						_pos:=127; TABputIn(TABFn_blank_param,TABFn_blank);
					end;
					key_SPACE: begin					// clear row (put blank entry)
						TABputIn(TABFn_blank_param,TABFn_blank);
						moveCursor(1,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift);
					end;
					key_RETURN: if TABEntryEdit then break;	// call TAB functions list
				end;
			end;

			i:=pianoKeyScan(0);
			if i<>255 then
			begin
				if editState then
				begin// write note in TAB
					TABputIn(i,currentSFX);
					moveCursor(1,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift);
				end;
			end;

			if modified then TABDetermineLength();
         //
         updateTAB(true);
         updateTABInfo();
         drawCursor();
      end;
      updatePianoVis();
   until checkEscape;
   updateTAB(true);
   SFX_Off();
   saveIfModified();
   reset_pianoVis();
end;
