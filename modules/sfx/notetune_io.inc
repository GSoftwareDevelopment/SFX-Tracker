procedure IOLoadDefaultNoteTable();
begin
	if fileExists(defaultNoteTable) then
		LoadData(defaultNoteTable,LOAD_NOTE_DEF,255);
end;

procedure NoteTune_load();
begin
	if IOPrompt(otherFile) then
		LoadData(otherFile,LOAD_NOTE_DEF,noteSetId);
end;

procedure NoteTune_save();
begin
	if IOPrompt(otherFile) then
	begin
		if openFile(otherFile,file_write)=IO_OK then
		begin
			if (not storeNOTEDataInFile(noteSetId)) then
				IOError(IOResult);
			closeFile();
		end
		else
			IOError(IOResult);
	end;
end;
