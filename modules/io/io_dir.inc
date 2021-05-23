function IOReadDir(path:string; startRead:smallint):byte;
const
	attrMask:array[0..3] of byte = (faReadOnly,faSysFile,faHidden,faArchive);
	attrChar:array[0..3] of byte = (50,51,40,33); // RSHA

var
	f:TSearchRec;
	listOfs,attrOfs:word;
	i,filesCount,nameLen:byte;

begin
	fillchar(@listBuf,1024,0);
	listOfs:=0; filesCount:=0;
	fillchar(@f.Name,16,0);
	FindFirst(path, faAnyFile, f);
	filesCount:=0; listOfs:=0;
	if IOResult=1 then
	begin
		if (startRead>0) then
		begin
			move(resptr[str_IO_prevPage],@listBuf[listOfs],8); // hardcoded length of string!!!
			filesCount:=filesCount+1; listOfs:=listOfs+width_fileList;
			putMultiText(resptr[str_IO_Skipping],0);
			screen2video();
			while (startRead>0) and (IOResult=1) do
			begin
				findNext(f); startRead:=startRead-1;
			end;
		end;
		putMultiText(resptr[str_IO_Reading],0);
		screen2video();
		while (IOResult=1) and (filesCount<IO_pageLimit) do
		begin
			fillchar(@listBuf[listOfs],width_fileList,0); // clear list entry

			nameLen:=length(f.Name);
			// convert all characters in name to internal code and, if nessesert to uppercase
			for i:=1 to nameLen do
				if byte(f.Name[i])>95 then
					f.Name[i]:=char(byte(f.Name[i])-64)
				else
					f.Name[i]:=char(byte(f.Name[i])-32);
			// add folder icon to list buffer if, entry have directory attribute set
			if f.attr and faDirectory=faDirectory then
				listBuf[listOfs]:=$1e; // more (>)
			// copy name to list buffer
			move(@f.Name[1],@listBuf[listOfs+1],nameLen);

			// create attribute field in list buffer
			attrOfs:=listOfs+width_filePath-sizeOf(attrChar);
			for i:=0 to 3 do
				if f.attr and attrMask[i]=attrMask[i] then
					listBuf[attrOfs+i]:=attrChar[i]	// attribute character - is set
				else
					listBuf[attrOfs+i]:=$0d;			// hypen - attribute is not set

			fillchar(@f.Name,nameLen+1,0); // clear name variable

			listOfs:=listOfs+width_fileList;
			filesCount:=filesCount+1;

			findNext(f);
		end;
		fillchar(@screen[20],20,0);
		if (IOResult=1) then
		begin
			if (filesCount=IO_pageLimit) then
			begin
				move(resptr[str_IO_nextPage],@listBuf[listOfs],8); // hardcoded length of string!!!
				filesCount:=filesCount+1;
			end
		end
		else
			if filesCount>0 then filesCount:=filesCount-1;
	end;
	if (filesCount<=IO_pageLimit) and (IOResult<>136) then IOError(IOResult);
	FindClose(f);
	result:=filesCount;
end;

function IODirectory():string;
var
	f:TSearchRec;
	selFile,
	filesCount:byte;
	searchPath:string[FILEPATHMaxLength] = 'H:';
	wild:string[1] = '*';
	listStart:smallint;
	listPage:byte;

	procedure updateListPage();
	begin
		putValue(17,1,listPage+1,3,0);
	end;

begin
	box(0,2,20,9,$40);
	putMultiText(resptr[str_IO_Prompt],1);
	if inputLongText(0,11,width_filePath,FILEPATHMaxLength,searchPath,0,0) then
	begin
		fillchar(@searchPath[1+length(searchPath)],FILEPATHMaxLength-length(searchPath),0);
		if (indexOf(searchPath,wild,1)=-1) then
			searchPath:=concat(searchPath,'*.*');

		if length(searchPath)<width_filePath then
			conv2internalP2P(@searchPath[1],@screen[220],length(searchPath))
		else
			conv2internalP2P(@searchPath[1],@screen[220],width_filePath);

		listStart:=0; listPage:=0;
		repeat
			box(0,2,width_fileList,9,$40);
			filesCount:=IOReadDir(searchPath,listStart);
			if filesCount>0 then
			begin
				selFile:=0;
				selFile:=listChoice(0,2,width_fileList,9,selFile,listBuf,filesCount,false);
				if (filesCount>IO_pageLimit) and (selFile=IO_pageLimit) then
				begin
					listStart:=listStart+IO_pageLimit;
					listPage:=listPage+1;
					updateListPage();
				end
				else
					if (listStart>0) and (selFile=0) then
					begin
						listStart:=listStart-IO_pageLimit;
						listPage:=listPage-1;
					updateListPage();
					end
					else
						break;
			end
			else
			begin
				putMultiText(resptr[str_IO_noFiles],1);
				break;
			end;
		until false;
	end;
end;
