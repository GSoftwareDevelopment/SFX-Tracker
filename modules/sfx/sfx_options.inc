procedure setSFXName();
begin
	if (inputText(SFXNameX,SFXNumberY,SFXNameLength,SFXName,0,color_choice)) then
	begin
		storeSFXData(currentSFX);
		modified:=false;
	end;
end;

procedure setSFXModMode();
var
	mod_mode:byte;

begin
	mod_mode:=SFXModMode[currentSFX];
	if optionsList(resptr[menu_sfx_mode],width_menuModType,SFXMenuModType,mod_mode,key_Up,key_Down) then
		if (mod_mode<SFXMenuModType_Back) then
		begin
			SFXModMode[currentSFX]:=mod_mode;
		end;
end;

{$i modules/sfx/notetune.pas}

procedure SFX_Options(defaultOpt:byte);
begin
	// make possibility to direct invoke option
	if defaultOpt=SFXMenuOption_Back then
		if optionsList(resptr[menu_sfx_options],width_menuOptions,SFXMenuOptions,defaultOpt,key_Up,key_Down) then
			case defaultOpt of
				0: if SFXPtr[currentSFX]<>$FFFF then
						setNoteTune()
					else
						showError(resptr[msg_UnknownDefinition]);
				1: setSFXName();
				2: setSFXModMode();
			end;
end;
