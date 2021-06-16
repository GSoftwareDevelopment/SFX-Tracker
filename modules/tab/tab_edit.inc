{$i modules/tab/tab_entry_edit.inc}

procedure TABEditLoop();
var
	_pos,i:byte;
	nSFX:shortint;

begin
	colorHLine(winXStart,2+cursorPos,20-winXStart,3);
	screen2video();
	reset_pianoVis();
	repeat
		if keyPressed then
		begin
			_pos:=cursorShift+cursorPos;
			if controlSelectionKeys(key,key_LEFT,key_RIGHT,currentSFX,0,maxSFXs-1) then
				updateTABSFX();
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

			if modified then TABDetermineLength();

			//
			updateTAB(true);
			colorHLine(winXStart,2+cursorPos,20-winXStart,3);
			updateTABInfo();
			screen2video();
		end;
		updatePianoVis();
	until false;
	updateTAB(true);
	SFX_Off();
	if modified then
	begin
		TABDetermineLength();
		storeTABData(currentTAB);
		modified:=false;
	end;
	reset_pianoVis();
end;
