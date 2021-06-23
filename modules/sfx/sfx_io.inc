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
