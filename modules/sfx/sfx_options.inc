procedure SFX_Options(defaultOpt:shortint);
begin
	// make possibility to direct invoke option
	if defaultOpt=SFXMenuOption_Back then
		optionsList(resptr[menu_sfx_options],width_menuOptions,SFXMenuOptions,defaultOpt);
	case defaultOpt of
		0: if (inputText(SFXNameX,SFXNumberY,SFXNameLength,SFXName,0,color_choice)) then
			begin
				storeSFXData(currentSFX);
				modified:=false;
			end;
	end;
end;
