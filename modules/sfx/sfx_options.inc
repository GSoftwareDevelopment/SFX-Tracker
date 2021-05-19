procedure SFX_Options(defaultOpt:shortint);
begin
	optionsList(resptr[menu_sfx_options],width_menuOptions,SFXMenuOptions,defaultOpt);
	case defaultOpt of
		4: begin
				SFXDetermineLength();
				if (sfxLen>0) then
				begin
					if (inputText(SFXNameX,SFXNumberY,SFXNameLength,SFXName,0,color_choice)) then
					begin
						storeSFXData(currentSFX);
						modified:=false;
					end;
				end;
		end;
	end;
end;
