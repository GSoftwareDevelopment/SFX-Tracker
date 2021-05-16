procedure IODirectory();
var
	f : TSearchRec;
	listOfs:word;
	selFile,filesCount,nameLen:byte;

begin
	box(0,2,20,9,$40);
	fillchar(@listBuf,1024,0);
	listOfs:=0; filesCount:=0;
	fillchar(@f.Name,16,0);
	if FindFirst('D:*.*', faAnyFile, f) = 0 then
	begin
		repeat
			conv2Internal(f.Name);
			nameLen:=length(f.Name);
			move(@f.Name[1],@listBuf[listOfs],12);
			fillchar(@listBuf[listOfs+12],4,$0d); // hypen
			if f.attr and faReadOnly=faReadOnly then
				listBuf[listOfs+12]:=byte('R')-32;
			if f.attr and faSysFile=faSysFile then
				listBuf[listOfs+13]:=byte('S')-32;
			if f.attr and faHidden=faHidden then
				listBuf[listOfs+14]:=byte('H')-32;
			if f.attr and faArchive=faArchive then
				listBuf[listOfs+15]:=byte('A')-32;
			if f.attr and faDirectory=faDirectory then
				listBuf[listOfs+nameLen+1]:=$1e; // &gt;

			fillchar(@f.Name,16,0);
			listOfs:=listOfs+16;
			filesCount:=filesCount+1;
		until FindNext(f) <> 0;
		FindClose(f);
	end;
	selFile:=0;
	listChoice(2,2,16,9,selFile,listBuf,filesCount,false);
end;
