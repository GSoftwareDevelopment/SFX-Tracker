procedure setFilename(var fn,dest:string);
begin
	fillchar(@dest,FILEPATHMaxLength+1,0);
	move(@fn,@dest,byte(fn[0])+1);
end;

procedure closeFile();
begin
	fillchar(@statusBar,20,0);
	close(f);
	SFX_Start();
end;

function openFile(var fn:string; mode:byte):byte;
var
	msgPtr:byte;

begin
	SFX_End();

	assign(f,fn);
	case mode of
		file_read: begin
			reset(f,1);
			msgPtr:=msg_IO_reading;
		end;
		file_write: begin
			rewrite(f,1);
			msgPtr:=msg_IO_writing;
		end;
	end;
	result:=IOResult;
	if (IOResult<>1) then
	begin
		closeFile();
	end
	else
	begin
		result:=IO_OK;
		putMultiText(msgPtr,0);
		screen2video();
	end;
end;
