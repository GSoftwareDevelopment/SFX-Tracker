procedure TAB_Options(defaultOpt:shortint);
begin
	optionsList(resptr[menu_tabs_options],width_menuOptions,TABMenuOptions,defaultOpt);
	case defaultOpt of
		3: begin
				TABDetermineLength();
				if (TABLen>0) then
				begin
					if (inputText(SFXNameX,SFXNumberY,TABNameLength,TABName,0,color_choice)) then
					begin
						storeTABData(currentTAB);
						modified:=false;
					end;
				end;
			end;
	end;
	box(4,2,16,9,0);
	updateTAB(true);
end;
