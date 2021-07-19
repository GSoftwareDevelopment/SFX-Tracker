var
   IOErr:byte;
   IOOfs,IOLength:word;

procedure clearFilename(var dest:string);
begin
   fillchar(@dest,FILEPATHMaxLength+1,0);
end;

procedure setFilename(var fn,dest:string);
begin
   clearFilename(dest);
   move(@fn,@dest,byte(fn[0])+1);
end;

procedure closeFile();
begin
   if IO_messages then clearStatusBar();
   close(f);
   if IO_preventSFX then SFX_Start();
end;

function openFile(var fn:string; mode:byte):byte;
var
   msgRes:byte;

begin
   if IO_preventSFX then SFX_End();

   assign(f,fn);
   case mode of
      file_read: begin
                     reset(f,1);
                     msgRes:=msg_IO_reading;
                  end;
      file_write: begin
                     rewrite(f,1);
                     msgRes:=msg_IO_writing;
                  end;
   end;
   result:=IOResult;
   if (IOResult<>IO_OK) then
      closeFile()
   else
   begin
      if IO_messages then
      begin
        putMultiText(msgRes,0);
        screen2video();
      end;
   end;
   IOErr:=result;
end;

function getBlock(var dest; size:word):boolean;
begin
   blockRead(f,dest,size,IOLength);
   IOErr:=IOResult;
   result:=IOErr<>IO_OK;
end;

function getByte(var dest):boolean;
begin
   result:=getBlock(dest,1);
end;

procedure push2IOBuf(data:pointer; size:word);
begin
   move(data,@IOBuf[IOOfs],size); inc(IOOfs,size);
end;

function putBlock(var dest; size:word):boolean;
begin
   blockWrite(f,dest,size,IOLength);
   IOErr:=IOResult;
   result:=IOResult=IO_OK;
end;

function putIOBuf():boolean;
begin
   result:=putBlock(IOBuf,IOOfs);
end;

procedure IOError();
begin
   drawError(msg_IO_Error);
   putValue(14,11,IOErr,3,0);
   flashError();
end;

//

{$ifndef INCLUDE_RESOURCES}
procedure GotoXY(x,y: byte); assembler;
asm
rowcrs   =  $54   ; wiersz  kursora
colcrs   =  $55   ; (2) Kolumna kursora

   ldy x
   sty colcrs
   mvy #$00 colcrs+1
   ldy y
   sty rowcrs
end;

procedure Resources_Load();
var
   ofs,readed:word;

begin
   if openFile(resourcesFile,file_read)=IO_OK then
   begin
      ofs:=RESOURCES_ADDR;
      repeat
         getBlock(listBuf,1024);
         move(@listBuf,pointer(ofs),IOLength);
         inc(ofs,IOLength);
      until IOLength<>IO_BUFFER_ADDR;
      closeFile();
   end
   else
   begin
      writeLn('RESOURCES FILE NOT FOUND :(');
      write('Press the key to return to DOS...');
      repeat until kbcode<>255; kbcode:=255;
      exit2DOS();
   end;
end;
{$endif}
