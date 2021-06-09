{$i modules/tab/tab_entry_edit.inc}

procedure TABEditLoop();
var
	_pos,i:byte;
	nSFX:shortint;

begin
	colorHLine(winXStart,2+cursorPos,20-winXStart,3);
	screen2video(); kbcode:=255;
	reset_pianoVis();
	repeat
		if (kbcode)<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
//			colorHLine(winXStart,2+cursorPos,20-winXStart,0);
			_pos:=cursorShift+cursorPos;
			if controlSelectionKeys(key,key_LEFT,key_RIGHT,currentSFX,0,maxSFXs-1) then
				updateTABSFX();
			if controlSelectionKeys(key,key_SHIFT_TAB,key_TAB,currentOct,0,3) then
				updatePiano();
			case key of
				key_ESC: break;
				key_CTRL_RETURN:begin
					updateTABSFX();
					nSFX:=dataList(SFXPtr,currentSFX,maxSFXs,SFXNameLength);
					if (nSFX<>-1) and (nSFX<>currentSFX) then
						currentSFX:=nSFX;
					updateTABSFX();
				end;
				key_SPACE: begin
					SFX_ChannelOff(0);
					TAB_notes[_pos]:=TABFn_blank_param;
					TAB_fnSFX[_pos]:=TABFn_blank;
					modified:=true;
					moveCursor(1,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift);
				end;
				key_RETURN: if TABEntryEdit then break;
			end;

			controlEditKeys(key,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift,key_Up,key_Down);

			i:=pianoKeyScan(0);
			if i<>255 then
			begin
				TAB_notes[_pos]:=i;
				TAB_fnSFX[_pos]:=currentSFX;
				modified:=true;
				moveCursor(1,TAB_winHeight,TAB_maxLength,cursorPos,cursorShift);
			end;

			if modified then
			begin
				TABDetermineLength();
				storeTABData(currentTAB);
				modified:=false;
			end;

			//
			updateTAB(true);
			colorHLine(winXStart,2+cursorPos,20-winXStart,3);
			updateTABInfo();
			screen2video();
		end;
		updatePianoVis();
	until false;
	updateTAB(true);
	reset_pianoVis();
	kbcode:=255;
end;
