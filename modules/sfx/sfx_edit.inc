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
			key:=TKeys(kbcode);
			if controlSelectionKeys(key,key_SHIFT_TAB,key_TAB,section,3,6) then
			begin
				section:=section or $10;
				break;
			end;
			case key of
				key_RETURN,key_ESC: break;
				key_CTRL_Left: begin
					colorVLine(winXStart+cursorPos,winYStart,8,0);
					if (cursorPos=0) then
						moveCursor(-SFX_winWidth,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift)
					else
						cursorPos:=0;
					updateSFXView();
				end;
				key_CTRL_Right: begin
					colorVLine(winXStart+cursorPos,winYStart,8,0);
					if (cursorPos=SFX_winWidth-1) then
						moveCursor(SFX_winWidth,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift)
					else
						cursorPos:=SFX_winWidth-1;
					updateSFXView();
				end;
				key_LEFT: begin
					colorVLine(winXStart+cursorPos,winYStart,8,0);
					moveCursor(-1,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift)
				end;
				key_RIGHT: begin
					colorVLine(winXStart+cursorPos,winYStart,8,0);
					moveCursor(1,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift)
				end;
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

			updateSFXView();
			colorVLine(winXStart+cursorPos,winYStart,8,3);
			screen2video();
			kbcode:=255;
		end;
	until false;
	kbcode:=255;
end;

