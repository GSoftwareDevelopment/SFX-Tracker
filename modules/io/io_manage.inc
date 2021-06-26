var
	IOErr:byte;
	IOOfs:word;

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
	if (IOResult<>1) then
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
	blockRead(f,dest,size);
	IOErr:=IOResult;
	result:=IOErr<>IO_OK;
end;

procedure push2IOBuf(data:pointer; size:word);
begin
	move(data,@IOBuf[IOOfs],size); inc(IOOfs,size);
end;

function putBlock(var dest; size:word):boolean;
begin
	blockWrite(f,dest,size);
	IOErr:=IOResult;
	result:=IOResult=1;
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
