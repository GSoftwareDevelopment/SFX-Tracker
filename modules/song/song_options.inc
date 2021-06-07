procedure SONG_Options(defaultOpt:byte);
begin
	// make possibility to direct invoke option
	if defaultOpt=SONGMenuOption_Back then
		if optionsList(resptr[menu_SONG_options],width_menuOptions,SONGMenuOptions,defaultOpt,key_Up,key_Down) then
		begin
//			case defaultOpt of
//				0: if (inputText(SFXNameX,SFXNumberY,TABNameLength,TABName,0,color_choice)) then
//					begin
//						storeTABData(currentTAB);
//						modified:=false;
//					end;
//			end;
		end;
	box(4,2,16,9,0);
	updateSONG(true);
end;
