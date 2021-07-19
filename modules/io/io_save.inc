function storeHDataInFile(var section:TTag; nameLength,id:byte):boolean;
var
   dataSize,
   dataAddr:word;
   noteTabId:byte;

begin
   IOOfs:=0;
   push2IOBuf(section,5);
   push2IOBuf(@id,1);

   if (id<maxSFXs) then // only for SFX!
   begin
      push2IOBuf(@SFXModMode[id],1);
      noteTabId:=SFXNoteSetOfs[id] shr 6;
      push2IOBuf(@noteTabId,1);
   end;

   dataAddr:=HEAP_PTR[id]-nameLength;  // get data address with name
   dataSize:=_heap_sizes[id]+nameLength; // data length with name
   push2IOBuf(@dataSize,2);
   push2IOBuf(pointer(dataAddr),dataSize);

// write to disk
   result:=putIOBuf;
end;

function storeNOTEDataInFile(noteTabId:byte):boolean;
var
   addr:word;

begin
   IOOfs:=0;
   push2IOBuf(@section_NOTE,5);
   push2IOBuf(@noteTabId,1);

   addr:=word(resptr[menu_NoteDef])+4+noteTabId*16;
   push2IOBuf(pointer(addr),NOTETABnameLength);
   addr:=NOTE_TABLE_ADDR+noteTabId*64;
   push2IOBuf(pointer(addr),64);

// write to disk
   result:=putIOBuf;
end;

function storeSONGDataInFile():boolean;
var
   dataSize:word;

begin
   IOOfs:=0;
   push2IOBuf(@section_SONG,5);
   dataSize:=$100;
   push2IOBuf(@dataSize,2);
   push2IOBuf(@SONGData,dataSize);

// write to disk
   result:=putIOBuf;
end;

function makeCompletteFile():boolean;
var
   i:byte;

begin
   IOOfs:=0;
   // prepare main section data
   push2IOBuf(@section_main,5);
   i:=SFXMM_VER1_1; push2IOBuf(@i,1);
   i:=SONGNameLength; push2IOBuf(@i,1);
   push2IOBuf(@SONGTitle[1],SONGNameLength);
   if not putIOBuf then exit(false);

   // store SONG data
   if not storeSONGDataInFile then exit(false);

   // store NOTE TABLE data
   for i:=0 to 3 do
      if not storeNOTEDataInFile(i) then exit(false);

   // store SFXs data
   for i:=0 to maxSFXs-1 do
      if SFXPtr[i]<>$FFFF then
         if not storeHDataInFile(section_SFX,SFXNameLength,i) then exit(false);

   // store TABs data
   for i:=0 to maxTABs-1 do
      if TABPtr[i]<>$FFFF then
         if not storeHDataInFile(section_TAB,TABNameLength,64+i) then exit(false);

   result:=true;
end;

procedure saveSong();
begin
   if openFile(currentFile,file_write)=IO_OK then
   begin
      if (not makeCompletteFile) then
         IOError();
      closeFile();
   end
   else
      IOError();
end;

procedure IOSave();
begin
   clearModule();
   putMultiText(msg_IO_SavePrompt,0);
   if IOPrompt(currentFile) then
   begin
      clearModule();
      saveSong();
   end;
end;
