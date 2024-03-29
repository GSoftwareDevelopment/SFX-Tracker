{$i modules/sfx/sfx_entry_edit.inc}
procedure SFXSectionEditLoop(var tab:byteArray; nibble:byte);
var i,v,cSFX:byte;
   at:byte;

	procedure SFXputNOP();
	begin
		SFX_modulate[at]:=0;
		modified:=true;
	end;

	procedure SFXputBlank();
	begin
		SFX_vol_dist[at]:=0;
		SFXputNOP();
	end;

	procedure dblMove(ofsSrc,ofsDest:byte);
	var len:byte;

	begin
		len:=127-at;
		ofsSrc:=at+ofsSrc; ofsDest:=at+ofsdest;
		move(@SFX_vol_dist[ofsSrc],@SFX_vol_dist[ofsDest],len);
		move(@SFX_modulate[ofsSrc],@SFX_modulate[ofsDest],len);
	end;

	procedure updateCursor();
	begin
		i:=winXStart+cursorPos;
		__scrofs:=i+vadr[winYStart];
		color:=$c0; colorVLine(9);
		__scrofs:=i+vadr[3+section];
		if section>4 then inc(__scrofs,20);
		i:=screen[__scrofs];
		screen[__scrofs]:=i and $3f or $80;
      screen2video();
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
	updateCursor();
	cSFX:=currentSFX;
   repeat
      if keyPressed then
      begin
         if controlSelectionKeys(key_Up,key_Down,section,3,6) then
         begin
            section:=section or $10;
            break;
         end;

         if controlSelectionKeys(key_SHIFT_LEFT,key_SHIFT_RIGHT,cSFX,0,maxSFXs-1) then
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

			if key=key_CONTROL+key_N then setSFXName();

			i:=controlSFXShortcutKeys;
			if i<>255 then begin
				setShortcut2currentSFX(i);
// TIP: uncomment below lines to active status bar info about assigned fast key
				updateSFXView();
				SFXshortcutMessage(i);
				updateCursor();
				continue;
			end;

         controlEditKeys(SFX_winWidth,SFX_maxLength,cursorPos,cursorShift,key_Left,key_Right);

         at:=cursorShift+cursorPos;

         case key of
            key_RETURN: if SFXEntryEdit then break;
            key_SPACE: SFXputNOP();
				key_CTRL_INSERT: begin			// insert column in SFX
//					at:=cursorShift+cursorPos;
					dblMove(0,1);
					SFXputBlank();
				end;
				key_CTRL_DELETE: begin			// delete column in SFX
					dblMove(1,0);
					at:=127; SFXputBlank();
					at:=cursorShift+cursorPos;
				end;
            key_CTRL_X: Cut2Clipboard(true,SFXLen);
            key_CTRL_C: Copy2Clipboard(true,SFXLen);
            key_CTRL_V: PasteClipboard(true);
         end;

         v:=tab[at];
         if (nibble=1) then v:=v shr 4 else v:=v and $0f;

         // value change
         if controlSelectionKeys(key_CTRL_Down,Key_CTRL_Up,v,0,15) then
            storeValue(v)
         else
         begin
            i:=keyScan(keys_alphaNum,keysRange_hexNum);
            if (i<>255) then storeValue(i);
         end;

			if modified then SFXDetermineLength();
         updateSFXView();
         updateCursor();
      end;
   until checkEscape;
end;

procedure SFXEditLoop();
begin

	repeat
		section:=section and $f;
		menuBar(menu_sfx,width_menuBar,section,color_choice,color_selected);
		case section of
			3: SFXSectionEditLoop(SFX_vol_dist,0);
			4: SFXSectionEditLoop(SFX_vol_dist,1);
			5: SFXSectionEditLoop(SFX_modulate,1);
			6: SFXSectionEditLoop(SFX_modulate,0);
		end;
	until section and $10=0;
end;
