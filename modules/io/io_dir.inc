procedure IOError();
var
	flashTM:longint;
	flashState:boolean;

begin
	fillchar(@screen[220],20,0);
	putNText(3,11,resptr[str_IO_Error],0);
	putValue(14,11,IOResult,3,0);
	kbcode:=255; flashState:=false; flashTM:=0;
	repeat
		if (getTime-flashTM>=25) then
		begin
			flashState:=not flashState;
			colorHLine(0,11,20,3*byte(flashState));
			screen2video();
			flashTM:=getTime;
		end;
	until kbcode<>255; kbcode:=255;
	fillchar(@screen[220],20,0);
	screen2video();
end;

function IOReadDir(path:String; var IOErr:byte):byte;
const
	attrMask:array[0..3] of byte = (faReadOnly,faSysFile,faHidden,faArchive);
	attrChar:array[0..3] of byte = ('R'~,'S'~,'H'~,'A'~);

var
	f:TSearchRec;
	listOfs,attrOfs:word;
	i,filesCount,nameLen:byte;

begin
	fillchar(@listBuf,1024,0);
	listOfs:=0; filesCount:=0;
	fillchar(@f.Name,16,0);
	IOErr:=FindFirst(path, faAnyFile, f);
	if IOErr=0 then
	begin
		repeat
			conv2Internal(f.Name); nameLen:=length(f.Name);
			// convert all characters in name to uppercase
			for i:=1 to nameLen do if byte(f.Name[i])>63 then f.Name[i]:=char(byte(f.Name[i])-32);
			// add folder icon to list buffer if, entry have directory attribute set
			if f.attr and faDirectory=faDirectory then
				listBuf[listOfs]:=byte('<')-32;
			// copy name to list buffer
			move(@f.Name[1],@listBuf[listOfs+1],width_filePath-(sizeOf(attrChar)+1));

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

			IOErr:=findNext(f);
		until IOErr<>0;
		FindClose(f);
	end;
	result:=filesCount;
end;

function IODirectory():string;
var
	f:TSearchRec;
	selFile,filesCount,IOErr:byte;
	searchPath:string[FILEPATHMaxLength] = 'D:'~;
	wild:string[1];

begin
	box(0,2,20,9,$40);
	putNText(0,10,resptr[str_IO_Prompt],1);
	if inputLongText(0,11,width_filePath,FILEPATHMaxLength,searchPath,0,0) then
	begin
		fillchar(@searchPath[1+length(searchPath)],FILEPATHMaxLength-length(searchPath),0);
		conv2ASCII(searchPath); wild:='*';
		if (indexOf(searchPath,wild,1)=-1) then
			searchPath:=concat(searchPath,'*.*');
		conv2Internal(searchPath);
		if length(searchPath)<width_filePath then
			move(@searchPath[1],@screen[220],length(searchPath))
		else
			move(@searchPath[1],@screen[220],width_filePath);
		conv2ASCII(searchPath);
		box(0,2,width_fileList,9,$40);
		filesCount:=IOReadDir(searchPath,IOErr);
		if IOErr=136 then
		begin
			selFile:=0;
			listChoice(0,2,width_fileList,9,selFile,listBuf,filesCount,false);
		end;
	end;
	conv2Internal(searchPath);
	if (IOErr<>0) and (IOErr<>136) then IOError();
end;
