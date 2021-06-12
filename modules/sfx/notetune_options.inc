procedure NoteTune_preset();
begin
end;

procedure NoteTune_user();
begin
end;

procedure NoteTune_load();
begin
end;

procedure NoteTune_save();
begin
end;

procedure NoteTune_Options(defaultOpt:byte);
begin
	if optionsList(resptr[menu_tune_options],width_menuOptions,TUNEMenuOptions,defaultOpt,key_Up,key_Down) then
		case defaultOpt of
			0:NoteTune_preset();
			1:NoteTune_user();
			3:NoteTune_load();
			4:NoteTune_save();
		end;
	box(4,2,16,9,0);
	updateNoteTune(currentOct*12);
end;
