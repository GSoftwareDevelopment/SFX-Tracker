procedure IOLoadDefaultNoteTable();
begin
   if fileExists(defaultNoteTable) then
      LoadData(defaultNoteTable,LOAD_NOTE_DEF,255);
end;

procedure NoteTune_load();
begin
   if IOPrompt(otherFile) then
   begin
      LoadData(otherFile,LOAD_NOTE_DEF,noteSetId);
      if IOErr<>3 then IOError();
   end;
end;

procedure NoteTune_save();
begin
   if IOPrompt(otherFile) then
   begin
      if openFile(otherFile,file_write)=IO_OK then
      begin
         if (not storeNOTEDataInFile(noteSetId)) then
            IOError();
         closeFile();
      end
      else
         IOError();
   end;
end;

procedure NoteTune_IO_Options();
begin
   case IO_Options of
      0:NoteTune_load();
      1:NoteTune_save();
   end;
end;
