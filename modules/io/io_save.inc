function makeCompletteFile(var f:file):byte;
var
	section:string[5];

begin
	// prepare main section data
	move(@section_main,@tmpbuf,5);
	tmpbuf[5]:=SFXMM_VERSION;
	tmpbuf[6]:=SONGNameLength;
	move(@SONGName[1],@tmpbuf[7],SONGNameLength);
	BlockWrite(f,tmpbuf,5+1+1+SONGNameLength);
	result:=IOResult;
end;

procedure IOSave();
var
	f:file;

begin
	if inputLongText(0,11,20,FILEPATHMaxLength,currentFile,0,0) then
	begin
		conv2ASCII(currentFile);
		{$I-}
		Assign(f,filename);
		Rewrite(f,1);
		if IOResult then
			makeCompletteFile(f)
		else
			IOError();
		close(f);
		{$I+}
		conv2Internal(currentFile);
	end
end;
