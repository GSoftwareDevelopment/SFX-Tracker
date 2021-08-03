{$i modules/tab/tab_entry_edit.inc}

procedure TABEditLoop();
var
   _pos,i,cTAB:byte;
   okey:TKeys;
   nSFX:shortint;

	procedure TABputIn(_note,_fn:byte);
	begin
		TAB_notes[_pos]:=_note;
		TAB_fnSFX[_pos]:=_fn;
		modified:=true;
	end;

	procedure dblMove(ofsSrc,ofsDest:byte);
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
      if TABStep>0 then
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

	procedure checkTABStepKeys();
	var
		v:smallint;

	begin
		if key and $80=$80 then
		begin
			if key=149 (* CTRL+B *) then
			begin
				v:=TABStep;
				putMultiText(msg_TAB_StepPrompt,0);
				if inputValue(winXStart+5,11,3,v,1,32,0,color_selected) then
					TABStep:=byte(v);
				clearStatusBar();
			end
			else
			begin
				okey:=key; key:=TKeys(key and $3f);
				i:=keyScan(keys_alphaNum,keysRange_decNum);
				if (i<>255) then
				begin
					if i=0 then
						TABStep:=0
					else
						inc(TABStep,i);
					if TABStep>32 then TABStep:=0;
					drawError(msg_TAB_StepChange);
					__scrOfs:=237; putValue(TABStep,2,0);
				end;
				key:=okey;
			end;
		end;
	end;

begin
   reset_pianoVis();
   cTAB:=currentTAB;
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

			if key=key_CONTROL+key_P then
			begin
				saveIfModified();
				TAB_Play(_pos); // (_pos shl 1);
			end;
			if key=key_CONTROL+key_N then setTABName();

         case key of
				key_SPACE: SFX_ChannelOff(0); // turn off SFX
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

			if TABStep>0 then
			begin
				case key of
					key_CTRL_INSERT: begin			// insert row in TAB
						dblMove(0,1);
						TABputIn(TABFn_blank_param,TABFn_blank);
					end;
					key_CTRL_DELETE: begin			// delete row from TAB
						dblMove(1,0);
						_pos:=127; TABputIn(TABFn_blank_param,TABFn_blank);
					end;
					key_SPACE: begin					// clear row (put blank entry)
						TABputIn(TABFn_blank_param,TABFn_blank);
						moveCursor(TABStep,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift);
					end;
					key_RETURN: if TABEntryEdit then break;	// call TAB functions list
				end;
			end;

			checkTABStepKeys();

			i:=pianoKeyScan(0);
			if i<>255 then
			begin
				if TABStep>0 then
				begin// write note in TAB
					TABputIn(i,currentSFX);
					moveCursor(TABStep,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift);
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
