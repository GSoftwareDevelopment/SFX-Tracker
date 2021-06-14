procedure NoteTune_sets();
begin
	noteSetId:=currentNoteTableOfs shr 6;
	if optionsList(resptr[menu_NoteDef],width_menuOptions,NOTEDEFMenu,noteSetId,key_Up,key_Down) then
		currentNoteTableOfs:=NOTE_TABLE_ADDR+noteSetId*64
	else
		currentNoteTableOfs:=SFXNoteSetOfs[currentSFX];
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
			1:NoteTune_load();
			2:NoteTune_save();
		end;
	NoteTuneRedraw();
end;
