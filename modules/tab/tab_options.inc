procedure TAB_Options(defaultOpt:shortint);
begin
	// make possibility to direct invoke option
	if defaultOpt=TABMenuOption_Back then
		optionsList(resptr[menu_tabs_options],width_menuOptions,TABMenuOptions,defaultOpt);

	case defaultOpt of
		0: if (inputText(SFXNameX,SFXNumberY,TABNameLength,TABName,0,color_choice)) then
			begin
				storeTABData(currentTAB);
				modified:=false;
			end;
	end;
	box(4,2,16,9,0);
	updateTAB(true);
end;
