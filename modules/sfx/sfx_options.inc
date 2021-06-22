{$i modules/sfx/notetune.pas}

procedure setSFXNoteTable();
begin
	currentNoteTableOfs:=SFXNoteSetOfs[currentSFX];
	NoteTune_sets();
	SFXNoteSetOfs[currentSFX]:=currentNoteTableOfs;
	currentNoteTableOfs:=$FF;
end;

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
	if optionsList(menu_sfx_mode,width_menuModType,SFXMenuModType,mod_mode,key_Up,key_Down) then
		if (mod_mode<SFXMenuModType_Back) then
		begin
			SFXModMode[currentSFX]:=mod_mode;
			SFXDetermineLength();
			storeSFXData(currentSFX);
		end;
end;

procedure SFX_IO_Load();
begin
	if IOPrompt(otherFile) then
		LoadData(otherFile,LOAD_SFX_DEF,currentSFX);
end;

procedure SFX_IO_Save();
begin
	if IOPrompt(otherFile) then
	begin
		if openFile(otherFile,file_write)=IO_OK then
		begin
			if (not storeHDataInFile(section_SFX,SFXNameLength,currentSFX)) then
				IOError(IOResult);
			closeFile();
		end
		else
			IOError(IOResult);
	end;
end;

procedure SFX_IO_Options();
begin
	case IO_Options of
		0:SFX_IO_Load();
		1:SFX_IO_Save();
	end;
	SFXScreen();
end;

procedure SFX_Options(defaultOpt:byte);
begin
	if optionsList(menu_sfx_options,width_menuOptions,SFXMenuOptions,defaultOpt,key_Up,key_Down) then
		case defaultOpt of
			0: setSFXName();
			1: setSFXNoteTable();
			2: setSFXModMode();
			4: if SFXPtr[currentSFX]<>$FFFF then
					setNoteTune()
				else
					showError(msg_UnknownDefinition);
			5: SFX_IO_Options();
		end;
end;
