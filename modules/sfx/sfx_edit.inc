{$i modules/sfx/sfx_entry_edit.inc}

procedure SFXEditLoop(var tab:byteArray; nibble:byte);
var i,v,scrofs,cSFX:byte;
   at:byte;

	procedure updateCursor();
	begin
		scrofs:=winXStart+cursorPos+vadr[winYStart];
		colorVLine(scrofs,0,8,3);
		inc(scrofs,vadr[1+section]);
		i:=screen[scrofs];
		screen[scrofs]:=i and $3f or $80;
	end;

   procedure storeValue(v:byte);
   begin
      i:=tab[at];
      if (nibble=1) then
      begin
         v:=v shl 4;
         i:=i and $0f;
      end
      else
         i:=i and $f0;
      tab[at]:=i or v;
      SFXDetermineLength();
      modified:=true;
   end;

begin
	updateCursor(); screen2video();
	cSFX:=currentSFX;
   repeat
      if keyPressed then
      begin
         if controlSelectionKeys(key,key_Up,key_Down,section,3,6) then
         begin
            section:=section or $10;
            break;
         end;

         if controlSelectionKeys(key,key_SHIFT_LEFT,key_SHIFT_RIGHT,cSFX,0,maxSFXs-1) then
         begin										// change the currently edited SFX
				if modified then
				begin
					// store SFX set after edit leave, if its modified
					storeSFXData(currentSFX);
					modified:=false;
				end;
				getSFXData(cSFX);
				SFXDetermineLength();
			end;

         case key of
            key_ESC: break;
            key_RETURN: if SFXEntryEdit then break;
            key_CTRL_X: Cut2Clipboard(true,SFXLen);
            key_CTRL_C: Copy2Clipboard(true,SFXLen);
            key_CTRL_V: PasteClipboard(true);
         end;

         controlEditKeys(key,SFX_winWidth,SFX_maxLength,cursorPos,cursorShift,key_Left,key_Right);

         at:=cursorShift+cursorPos;
         v:=tab[at];
         if (nibble=1) then v:=v shr 4 else v:=v and $0f;

         // value change
         if controlSelectionKeys(key,key_CTRL_Down,Key_CTRL_Up,v,0,15) then
            storeValue(v)
         else
         begin
            i:=keyScan(key,keys_alphaNum,keysRange_hexNum);
            if (i<>255) then storeValue(i);
         end;

			if modified then SFXDetermineLength();
         updateSFXView();
         updateCursor();
         screen2video();
      end;
   until false;
end;

