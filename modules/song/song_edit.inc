procedure SONGEditLoop();
var
	_pos:byte;

begin
	updateSONG(false);
	colorHLine(winSONGXStart+SONGChn*3,2+SONGPos,2,3);
	screen2video(); kbcode:=255;
	reset_pianoVis();
	repeat
		if (kbcode)<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			colorHLine(winSONGXStart+SONGChn*3,2+SONGPos,2,0);
			_pos:=SONGShift+SONGPos;

			controlSelectionKeys(key,key_LEFT,key_RIGHT,SONGChn,0,3);

			case key of
				key_ESC: break;

				// cursor screen step
				key_CTRL_Up: moveCursor(-9,TAB_winWidth,SONG_maxLength,SONGPos,SONGShift);
				key_CTRL_Down:	moveCursor(9,TAB_winWidth,SONG_maxLength,SONGPos,SONGShift);
				// cursor single step
				key_Up: moveCursor(-1,TAB_winWidth,SONG_maxLength,SONGPos,SONGShift);
				key_Down: moveCursor(1,TAB_winWidth,SONG_maxLength,SONGPos,SONGShift);

//				key_RETURN: if TABEntryEdit then break;
			end;

			//
			updateSONG(false);
			colorHLine(winSONGXStart+SONGChn*3,2+SONGPos,2,3);
			screen2video();
		end;
		updatePianoVis();
	until false;
	reset_pianoVis();
	kbcode:=255;
	updateSONG(true);
end;
