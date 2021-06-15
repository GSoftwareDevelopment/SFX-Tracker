procedure NoteTune_setName();
begin
end;

procedure NoteTune_sets();
begin
	noteSetId:=currentNoteTableOfs shr 6;
	if optionsList(resptr[menu_NoteDef],width_menuOptions,NOTEDEFMenu,noteSetId,key_Up,key_Down) then
		currentNoteTableOfs:=NOTE_TABLE_ADDR+noteSetId*64
	else
		currentNoteTableOfs:=SFXNoteSetOfs[currentSFX];
	noteSetId:=currentNoteTableOfs shr 6;
end;

procedure IOLoadDefaultNoteTable();
begin
	{$I-}
	if fileExists(defaultNoteTable) then
	begin
		fillchar(@currentFile,FILEPATHMaxLength,0);
		move(@defaultNoteTable,@currentFile,length(defaultNoteTable)+1);
		LoadData(LOAD_NOTE_DEF,255);
	end;
	{$I+}
end;

procedure NoteTune_load();
begin
	move(@screen,@tmpbuf,240);
	if IOPrompt(currentFile) then
	begin
		fillchar(@statusBar,20,0);
		LoadData(LOAD_NOTE_DEF,noteSetId);
	end;
	move(@tmpbuf,@screen,240);
end;

procedure NoteTune_save();
begin
	move(@screen,@tmpbuf,240);
	if IOPrompt(currentFile) then
	begin
		SFX_End();
		putMultiText(resptr[msg_IO_writing],0);
		{$I-}
		assign(f,currentFile);
		rewrite(f,1);
		if (IOResult=1) then
		begin
			if (not storeNOTEDataInFile(f,noteSetId)) then
				IOError(IOResult);
		end
		else
			IOError(IOResult);
		close(f);
		{$I+}
		SFX_Start();
	end;
	move(@tmpbuf,@screen,240);
end;

procedure NoteTune_Options(defaultOpt:byte);
begin
	if optionsList(resptr[menu_tune_options],width_menuOptions,TUNEMenuOptions,defaultOpt,key_Up,key_Down) then
		case defaultOpt of
			0:NoteTune_setName();
			2:NoteTune_load();
			3:NoteTune_save();
		end;

	currentNoteTableOfs:=noteSetId*64;
	NoteTuneRedraw();
end;
