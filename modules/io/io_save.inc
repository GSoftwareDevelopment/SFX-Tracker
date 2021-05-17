procedure IOSave();
var
	f:file;
	section:string[5];

begin
	Assign(f,'D:TEST.SMM');
	Rewrite(f,1);

	// prepare main section data
	move(@section_main,@tmpbuf,5);
	tmpbuf[5]:=SFXMM_VERSION;
	tmpbuf[6]:=SONGNameLength;
	move(@SONGName[1],@tmpbuf[7],SONGNameLength);
	BlockWrite(f,section_main,5+1+1+SONGNameLength);

	Close(f);
end;
