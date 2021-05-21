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

procedure SFXMenuBar(section:byte);
begin
	case section of
		0: if SFXList then modified:=false;
		1: section:=3;
		7: SFX_Options(5-byte(modified));
	end;

	if (section>=2) and (section<=5) then
	begin
		colorVLine(winXStart+cursorPos,winYStart,8,3);
		repeat
			section:=section and $f;
			updateBar(resptr[menu_sfx],width_menuBar,section,color_choice,color_selected);
			case section of
				3: SFXEditLoop(SFX_vol_dist,0,modified);
				4: SFXEditLoop(SFX_vol_dist,1,modified);
				5: SFXEditLoop(SFX_modulate,1,modified);
				6: SFXEditLoop(SFX_modulate,0,modified);
			end;
		until section and $10=0;
		colorVLine(winXStart+cursorPos,winYStart,8,0);
	end;

	SFXDetermineLength();
	updateSFXView();
end;
