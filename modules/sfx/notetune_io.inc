procedure IOLoadDefaultNoteTable();
begin
	if fileExists(defaultNoteTable) then
	begin
		setFilename(defaultNoteTable,currentFile);
//		fillchar(@currentFile,FILEPATHMaxLength,0);
//		move(@defaultNoteTable,@currentFile,length(defaultNoteTable)+1);
		LoadData(LOAD_NOTE_DEF,255);
	end;
end;

procedure NoteTune_load();
begin
	move(@screen,@tmpbuf,240);
	if IOPrompt(currentFile) then
		LoadData(LOAD_NOTE_DEF,noteSetId);
	move(@tmpbuf,@screen,240);
end;

procedure NoteTune_save();
begin
	move(@screen,@tmpbuf,240);
	if IOPrompt(currentFile) then
	begin
		if openFile(currentFile,file_write) then
		begin
			if (not storeNOTEDataInFile(noteSetId)) then
				IOError(IOResult);
			closeFile();
		end
		else
			IOError(IOResult);
	end;
	move(@tmpbuf,@screen,240);
end;
