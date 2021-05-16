function splice(var s:string; start:smallint; len:smallint):string;
var i,slen:byte;
	ostr:string;

(* jest błąd, jeżeli length(s)=1 i len>1 *)

begin								// start=-1 len=3;
	slen:=length(s);			// slen=1;
	if (start<1) then			// -1<1=true
		start:=1					// start=1
	else
		if (start>slen) then start:=slen+1;
	if (len>slen) then len:=slen;	// 3>1=true; len=1
	if (len+start-1>=slen) then len:=slen-start+1;	// 1+1-1>=1?(2-1)>=1?-1>=1=false;
	if len<=0 then // (1<=0)=false
	begin
		result:='';
		exit;
	end;
	i:=0;
	ostr[0]:=char(len);
	while len>0 do
	begin
		ostr[1+i]:=s[start+i];
		i:=i+1;
		len:=len-1;
	end;
	result:=ostr;
end;

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
var
	f:TSearchRec;
	listOfs:word;
	i,filesCount,nameLen:byte;

begin
	fillchar(@listBuf,1024,0);
	listOfs:=0; filesCount:=0;
	fillchar(@f.Name,16,0);
	IOErr:=FindFirst(path, faAnyFile, f);
	if IOErr=0 then
	begin
		repeat
			conv2Internal(f.Name);
			nameLen:=length(f.Name);
			for i:=1 to nameLen do
				if byte(f.Name[i])>63 then f.Name[i]:=char(byte(f.Name[i])-32);
			move(@f.Name[1],@listBuf[listOfs],15);
			fillchar(@listBuf[listOfs+16],4,$0d); // hypen
			if f.attr and faDirectory=faDirectory then
				listBuf[listOfs+15]:=$1e; // &gt;
			if f.attr and faReadOnly=faReadOnly then
				listBuf[listOfs+16]:=byte('R')-32;
			if f.attr and faSysFile=faSysFile then
				listBuf[listOfs+17]:=byte('S')-32;
			if f.attr and faHidden=faHidden then
				listBuf[listOfs+18]:=byte('H')-32;
			if f.attr and faArchive=faArchive then
				listBuf[listOfs+19]:=byte('A')-32;

			fillchar(@f.Name,nameLen+1,0);
			listOfs:=listOfs+20;
			filesCount:=filesCount+1;

			IOErr:=findNext(f);
		until IOErr<>0;
		FindClose(f);
	end;
	result:=filesCount;
end;

procedure IODirectory();
var
	f:TSearchRec;
	selFile,filesCount,IOErr:byte;
	FilePath:string[255];
	wild:string[3];

begin
	box(0,2,20,9,$40);
	putNText(0,10,resptr[str_IO_Prompt],1);
	if inputLongText(0,11,20,128,FilePath,0,0) then
	begin
		conv2ASCII(FilePath);
		wild:=splice(FilePath,1+length(FilePath)-3,3);
		if (wild<>'*.*') then
			FilePath:=concat(FilePath,'*.*');
		conv2Internal(FilePath);
		if length(FilePath)<20 then
			move(@FilePath[1],@screen[220],length(FilePath))
		else
			move(@FilePath[1],@screen[220],20);
		conv2ASCII(FilePath);
		box(0,2,20,9,$40);
		filesCount:=IOReadDir(FilePath,IOErr);
		if IOErr=136 then
		begin
			selFile:=0;
			listChoice(0,2,20,9,selFile,listBuf,filesCount,false);
		end;
	end;
	conv2Internal(FilePath); IOErr:=0;
	if (IOErr<>0) and (IOErr<>136) then IOError();
end;
