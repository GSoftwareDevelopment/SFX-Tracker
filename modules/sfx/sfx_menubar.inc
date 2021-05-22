function SFXList():boolean;
var
	nSFX:shortint;

begin
	move(@screen[20],@tmpbuf,200);
	box(0,2,20,9,$40);
	prepareSFXsList();
	nSFX:=listChoice(1,2,SFXNameLength,9,currentSFX,listBuf,maxSFXs,true);
	move(@tmpbuf,@screen[20],200);
	if (nSFX<>-1) and (nSFX<>currentSFX) then
	begin
		getSFXData(nSFX);
		SFXDetermineLength();
		result:=true;
	end
	else
		result:=false;
end;

procedure SFXChangeMenuBarOpt(section:byte);
begin
	if (section=0) then
	begin
		controlSelectionKeys(key,key_Left,Key_Right,currentSFX,0,maxSFXs-1);
		getSFXData(currentSFX);
		updateSFXView();
		modified:=false;
	end;
end;

procedure SFXSelectMenuBar(var section:byte);
begin
	case section of
		0: if SFXList then modified:=false;
		1: section:=3;
		7: SFX_Options(SFXMenuOption_Back);
	end;

	if (section>=3) and (section<=6) then
	begin
		colorVLine(winXStart+cursorPos,winYStart,8,3);
		repeat
			section:=section and $f;
			updateBar(resptr[menu_sfx],width_menuBar,section,color_choice,color_selected);
			screen2video();
			case section of
				3: SFXEditLoop(SFX_vol_dist,0,modified);
				4: SFXEditLoop(SFX_vol_dist,1,modified);
				5: SFXEditLoop(SFX_modulate,1,modified);
				6: SFXEditLoop(SFX_modulate,0,modified);
			end;
		until section and $10=0;

		// store SFX set after edit leave, if its modified
		if modified then
		begin
			SFXDetermineLength();
			storeSFXData(currentSFX);
			modified:=false;
		end;

		colorVLine(winXStart+cursorPos,winYStart,8,0);
	end;

	SFXDetermineLength();
	updateSFXView();
end;
