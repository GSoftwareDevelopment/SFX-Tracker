procedure TAB_Play(startPos:byte);
var i:byte;

   procedure updateTabView();
   begin
      cursorPos:=channels[_tabOfs] shr 1;
      if (cursorPos>4) then
      begin
         if (cursorPos>TAB_maxLength-TAB_winHeight) then
         begin
             cursorShift:=TAB_maxLength-TAB_winHeight;
             cursorPos:=cursorPos-cursorShift;
         end
         else
         begin
            cursorShift:=cursorPos-4; cursorPos:=4;
         end;
      end
      else
         cursorShift:=0;

      updateTAB(true);
      __scrOfs:=vadr[2+cursorPos]+winXStart; colorHLine(16,3);
   end;

	procedure setShortcutSFX(SFXKey:byte);
	begin
		i:=SFX_shortcut[SFXKey];
		if i<>255 then
		begin
			currentSFX:=i;
			updateTABSFX();
		end;
		key:=TKeys(255); // clear keyboard state; prevent to play/write note (under key 2,3,5,6,7,9,0) in TAB
	end;

begin
   if TABPtr[currentTAB]=$FFFF then
   begin
      showError(msg_UnknownDefinition);
      exit;
   end;
   reset_pianoVis(); keyClick:=false; PLYtm:=timer; PLYstate:=0;
   SFX_PlayTAB(0,currentTAB);
   channels[$00+_tabOfs]:=startPos;
   repeat
      updatePianoVis();

      if keyPressed then
      begin
         case key of
            key_ESC: break;
            key_SPACE: SFX_ChannelOff(2);
         end;

         i:=controlSFXShortcutKeys; if i<>255 then setShortcutSFX(i);

         if controlSelectionKeys(key,key_LEFT,key_RIGHT,currentSFX,0,maxSFXs-1) then
            updateTABSFX();
         pianoKeyScan(2);
      end;
      if (timer-ScreenTm)>0 then
      begin
         updateTabView();
         ScreenTm:=timer;
         PlayFlash();
         move(@screen,@video,240);
      end;
   until false;
   PLYtm:=0; PLYstate:=2; PlayFlash();
   SFX_Off(); keyClick:=true; key:=TKeys($FF);
   reset_pianoVis();
   updateTAB(true);
end;
