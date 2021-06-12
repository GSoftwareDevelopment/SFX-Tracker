procedure TAB_Play();
var
	oldCurPos,oldCurShift:byte;

begin
	if TABPtr[currentTAB]=$FFFF then
	begin
		showError(resptr[msg_UnknownDefinition]);
		exit;
	end;
	oldCurPos:=cursorPos; oldCurShift:=cursorShift;
	reset_pianoVis();
	SFX_PlayTAB(0,currentTAB);
	repeat
		updatePianoVis();

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
		colorHLine(winXStart,2+cursorPos,16,3);

		if keyPressed then
		begin
			if controlSelectionKeys(key,key_LEFT,key_RIGHT,currentSFX,0,maxSFXs-1) then
				updateTABSFX();
			pianoKeyScan(2);

			case key of
				key_ESC: break;
			end;
		end;
		move(@screen,@video,240);
	until false;
	SFX_Off();
	reset_pianoVis();
	updateTAB(true);
end;
