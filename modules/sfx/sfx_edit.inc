{$i modules/sfx/sfx_entry_edit.inc}

function SFXEditLoop(var tab:byteArray; nibble:byte; var modified:boolean):shortint;
var i,v:byte;
	at:byte;

	procedure storeValue(v:byte);
	begin
		if (nibble=1) then
		begin
			v:=v shl 4;
			tab[at]:=(tab[at] and $0f) or v;
		end
		else
			tab[at]:=(tab[at] and $f0) or v;
	end;

begin
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			if controlSelectionKeys(key,key_SHIFT_TAB,key_TAB,section,3,6) then
			begin
				section:=section or $10;
				break;
			end;
			colorVLine(winXStart+cursorPos,winYStart,8,0);
			case key of
				key_ESC: break;
				key_RETURN: if SFXEntryEdit then break;
				key_CTRL_Left: moveCursor(-SFX_winWidth,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift);
				key_CTRL_Right: moveCursor(SFX_winWidth,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift);
				key_LEFT: moveCursor(-1,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift);
				key_RIGHT: moveCursor(1,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift);
			end;

			at:=cursorShift+cursorPos;
			v:=tab[at];
			if (nibble=1) then v:=v shr 4 else v:=v and $0f;

			// value change
			if controlSelectionKeys(key,key_Down,Key_Up,v,0,15) then
			begin
				storeValue(v);
				modified:=true;
			end
			else
			begin
				i:=keyScan(key,keys_alphaNum,keysRange_hexNum);
				if (i<>255) then
				begin
					storeValue(i);
					moveCursor(1,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift);
					modified:=true;
				end;
			end;

			SFXDetermineLength();
			updateSFXView();
			colorVLine(winXStart+cursorPos,winYStart,8,3);
			screen2video();
		end;
	until false;
end;

