procedure SFX_Options(defaultOpt:shortint);
var
	mod_mode:shortint;

begin
	// make possibility to direct invoke option
	if defaultOpt=SFXMenuOption_Back then
		if optionsList(resptr[menu_sfx_options],width_menuOptions,SFXMenuOptions,defaultOpt,key_Up,key_Down) then
			case defaultOpt of
				0: if (inputText(SFXNameX,SFXNumberY,SFXNameLength,SFXName,0,color_choice)) then
					begin
						storeSFXData(currentSFX);
						modified:=false;
					end;
				1: begin
						mod_mode:=SFXModMode[currentSFX];
						if optionsList(resptr[menu_sfx_mode],width_menuModType,SFXMenuModType,mod_mode,key_Up,key_Down) then
							if (mod_mode<SFXMenuModType_Back) then
							begin
								SFXModMode[currentSFX]:=mod_mode;
							end;
					end;
			end;
end;
