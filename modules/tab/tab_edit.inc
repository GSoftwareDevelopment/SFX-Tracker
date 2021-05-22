{$i modules/tab/tab_entry_edit.inc}

procedure TABEditLoop();
var
	i,v:byte;

begin
	colorHLine(winXStart,2+cursorPos,20-winXStart,3);
	screen2video(); kbcode:=255;
	repeat
		if (kbcode)<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			colorHLine(winXStart,2+cursorPos,20-winXStart,0);
			case key of
				key_ESC,	key_TAB,key_SHIFT_TAB: break;

				// cursor screen step
				key_CTRL_Up: moveCursor(-9,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
				key_CTRL_Down:	moveCursor(9,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
				// cursor single step
				key_Up: moveCursor(-1,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
				key_Down: moveCursor(1,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);

				key_SPACE: begin
					setBlank();
					moveCursor(1,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
				end;
				key_RETURN: if entryEdit then break;
			end;

			i:=keyScan(key,keys_notes,34);
			if (i<>255) then
			begin
				v:=key2Note[i];
				setNote(currentOct+v,currentSFX);
				moveCursor(1,TAB_winWidth,TAB_maxLength,cursorPos,cursorShift);
			end;

			//
			TABDetermineLength();
			updateTAB(true);
			colorHLine(winXStart,2+cursorPos,20-winXStart,3);
			updateTABInfo();
			screen2video();
		end;
	until false;
	kbcode:=255;
end;

procedure TABScreen();
begin
	fillchar(@screen[20],220,$00);
	VBar(0,1,width_menuBar,0);
	screen[223]:=$06;
	screen[33]:=$07;

	fillchar(@TAB_notes,256,255);
	fillchar(@TAB_fnSFX,256,254);

	menuBar(resptr[menu_tabs],width_menuBar,1);
end;
