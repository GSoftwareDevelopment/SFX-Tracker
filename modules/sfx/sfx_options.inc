procedure SFX_Options(defaultOpt:shortint);
var
	mod_mode:shortint;

begin
	// make possibility to direct invoke option
	if defaultOpt=SFXMenuOption_Back then
		optionsList(resptr[menu_sfx_options],width_menuOptions,SFXMenuOptions,defaultOpt,key_Up,key_Down);
	case defaultOpt of
		0: if (inputText(SFXNameX,SFXNumberY,SFXNameLength,SFXName,0,color_choice)) then
			begin
				storeSFXData(currentSFX);
				modified:=false;
			end;
		2: begin
				mod_mode:=SFXModMode[currentSFX];
				optionsList(resptr[menu_sfx_mode],width_menuModType,SFXMenuModType,mod_mode,key_Up,key_Down);
				if (mod_mode<SFXMenuModType_Back) then
					SFXModMode[currentSFX]:=mod_mode;
			end;
	end;
end;
