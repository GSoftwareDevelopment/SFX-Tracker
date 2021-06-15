function IOReadDir(var path:string; startRead:smallint):byte;
const
	attrMask:array[0..3] of byte = (faReadOnly,faSysFile,faHidden,faArchive);
	attrChar:array[0..3] of byte = (50,51,40,33); // RSHA

var
	f:TSearchRec;
	listOfs,attrOfs:word;
	i,filesCount,nameLen:byte;

begin
	SFX_End();
	fillchar(@listBuf,LIST_BUFFER_SIZE,0);
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
			putMultiText(resptr[msg_IO_Skipping],0);
			screen2video();
			while (startRead>0) and (IOResult=1) do
			begin
				findNext(f); dec(startRead);
			end;
		end;
		putMultiText(resptr[msg_IO_Reading],0);
		screen2video();
		while (IOResult=1) and (filesCount<IO_pageLimit) do
		begin
			fillchar(@listBuf[listOfs],width_fileList,0); // clear list entry

			nameLen:=length(f.Name);
			// convert all characters in name to internal code and to uppercase, if nessesery.
			for i:=1 to nameLen do
				if byte(f.Name[i])>95 then
					f.Name[i]:=char(byte(f.Name[i])-64)
				else
					f.Name[i]:=char(byte(f.Name[i])-32);
			// add folder icon to list buffer if, entry have directory attribute set
			if f.attr and faDirectory=faDirectory then
				listBuf[listOfs]:=$0c; // folder icon
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
		fillchar(@moduleBar,20,0);
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
	fillchar(@statusBar,20,0);
	SFX_Start();
	result:=filesCount;
end;

function IODirLoop(var choicedFName:string[16]):boolean;
var
	selFile:shortint;
	filesCount:byte;
	listStart,len:smallint;
	listPage:byte;
	listOfs:word;

	procedure updateListPage();
	begin
		putValue(17,11,listPage+1,3,0);
	end;

begin
	listStart:=0; listPage:=0;
	fillchar(@moduleBar,20,$40);
	putMultiText(resptr[msg_IO_DirPrompt],1);
	repeat
		fillchar(@moduleBar[20],180,$40);
		filesCount:=IOReadDir(searchPath,listStart);
		if filesCount>0 then
		begin
			selFile:=0;
			selFile:=listChoice(0,2,width_fileList,9,selFile,listBuf,filesCount,false);
			if (selFile=-1) then
			begin
				result:=false; break; // exit(false);
			end;
			if (filesCount>IO_pageLimit) and (selFile=IO_pageLimit) then
			begin // next page
				listStart:=listStart+IO_pageLimit;
				listPage:=listPage+1;
				updateListPage();
			end
			else
				if (listStart>0) and (selFile=0) then
				begin // previous page
					listStart:=listStart-IO_pageLimit;
					listPage:=listPage-1;
					updateListPage();
				end
				else
				begin	// file choice
					listOfs:=selFile*width_filePath+1;	// calculate list offset for selected entry
					len:=0;
					// the below loop, extracts the file name from the list, converting it from screen codes to ASCII
					while (listBuf[listOfs]<>0) and (len<16) do
					begin
						len:=len+1;
						choicedFName[len]:=char(listBuf[listOfs]+32);
						listOfs:=listOfs+1;
					end;
					choicedFName[0]:=char(len);

					result:=true;
					break;
				end;
		end
		else
		begin
			putMultiText(resptr[msg_IO_noFiles],1);
			result:=false;
			break;
		end;
	until false;
end;
(*
procedure IODirectory();
begin
	fillchar(@moduleBar,200,$40);
	if filenamePrompt(resptr[msg_IO_Prompt],1,searchPath)<>-1 then
		if IODirLoop(searchPath,FName) then
			joinPathName(searchPath,FName,currentFile)
end;
*)
function IOPrompt(var promptFile:string):boolean;
var
	is_star,is_dot:boolean;
	FName:string[16];
	len:byte;

begin
	repeat
		if inputLongText(0,11,20,FILEPATHMaxLength,promptFile,0,0) then
		begin
			// Recognize the entered name
			is_star:=findChar(promptFile,'*',1)>0;
			is_dot:=findChar(promptFile,'.',1)>0;
			if (not is_star) and (not is_dot) then
			begin
				len:=byte(promptFile[0]);
				move(resptr[wild_allFiles],@promptFile[1+len],3);
				promptFile[0]:=char(len+3);
			end;
			// update name on screen
			conv2internalP2P(@promptFile[1],@statusBar,width_filePath);
			if ((not is_star) and (is_dot)) then
			begin
				result:=true; break;
			end
			else
			begin
				fillchar(@FName,16,255);
				move(@promptFile,@searchPath,FILEPATHMaxLength+1);
				if IODirLoop(FName) then
					joinPathName(searchPath,FName,promptFile)
				else
				begin
					result:=false; break;
				end;
			end;

		end
		else
		begin
			result:=false; break;
		end;
	until false;
end;
