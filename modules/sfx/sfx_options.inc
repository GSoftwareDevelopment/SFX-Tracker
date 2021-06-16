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
			SFXDetermineLength();
			storeSFXData(currentSFX);
		end;
end;

{$i modules/sfx/notetune.pas}

procedure SFX_IO_Options();
var
	IOOpt:byte;

begin
	IOOpt:=0;
	if optionsList(resptr[menu_IO_options],width_menuIOOptions,IOOptions,IOOpt,key_Up,key_Down) then
	case IOOpt of
		0:exit;
		1:exit;
	end;
end;

procedure SFX_Options(defaultOpt:byte);
begin
	if optionsList(resptr[menu_sfx_options],width_menuOptions,SFXMenuOptions,defaultOpt,key_Up,key_Down) then
		case defaultOpt of
			0: setSFXName();
			1: begin
				currentNoteTableOfs:=SFXNoteSetOfs[currentSFX];
				NoteTune_sets();
				SFXNoteSetOfs[currentSFX]:=currentNoteTableOfs;
				currentNoteTableOfs:=$FF;
			end;
			2: setSFXModMode();
			4: if SFXPtr[currentSFX]<>$FFFF then
					setNoteTune()
				else
					showError(resptr[msg_UnknownDefinition]);
			5: SFX_IO_Options();
		end;
end;
