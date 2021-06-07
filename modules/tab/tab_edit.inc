{$i modules/tab/tab_entry_edit.inc}

procedure TABEditLoop();
var
	_pos,i:byte;

begin
	colorHLine(winXStart,2+cursorPos,20-winXStart,3);
	screen2video(); kbcode:=255;
	reset_pianoVis();
	repeat
		if (kbcode)<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			colorHLine(winXStart,2+cursorPos,20-winXStart,0);
			_pos:=cursorShift+cursorPos;
			if controlSelectionKeys(key,key_LEFT,key_RIGHT,currentSFX,0,maxSFXs-1) then
				updateTABSFX();
			if controlSelectionKeys(key,key_SHIFT_TAB,key_TAB,currentOct,0,3) then
				updatePiano();
			case key of
				key_ESC: break;

				// cursor screen step
				key_CTRL_Up: moveCursor(-9,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
				key_CTRL_Down:	moveCursor(9,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
				// cursor single step
				key_Up: moveCursor(-1,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
				key_Down: moveCursor(1,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);

				key_SPACE: begin
					SFX_ChannelOff(0);
//					if key=key_SPACE then
//					begin
						TAB_notes[_pos]:=TABFn_blank_param;
						TAB_fnSFX[_pos]:=TABFn_blank;
//					end
//					else
//					begin
//						TAB_notes[_pos]:=TABFn_noteOff_param;
//						TAB_fnSFX[_pos]:=TABFn_noteOff;
//					end
					modified:=true;
					moveCursor(1,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
				end;
				key_RETURN: if TABEntryEdit then break;
			end;


			i:=pianoKeyScan(0);
			if i<>255 then
			begin
				TAB_notes[_pos]:=i;
				TAB_fnSFX[_pos]:=currentSFX;
				modified:=true;
				moveCursor(1,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
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
	reset_pianoVis();
	kbcode:=255;
end;
