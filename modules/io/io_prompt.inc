(* function IODirLoop(var searchPath:string; var resultFName:string[16]):boolean; Forward; // its needed! *)

function filenamePrompt(promptMsgPtr:pointer; promptMsgColor:byte; var fn:string):shortint;
(* result:
* -1 - exit from Prompt (ESC pressed)
*  0 - recognized name is file name
*  1 - recognized name have wildcard (possibility to run Dist Directory)

How is the entered name recognized?

Truth table
---------------+---+---+----------------------------
	name			| * | . | list directory
---------------+---+---+----------------------------
	D:				| 0 | 0 | true (add '*.*')
	D:F			| 0 | 0 | true (add '*.*')
	D:F*			| 1 | 0 | true	{don't add wildcards)
	D:F*.*		| 1 | 1 | true (don't add wildcards)
	D:F*.			| 1 | 1 | true (don't add wildcards}
	D:.*			| 1 | 1 | true (don't add wildcards)
---------------+---+---+----------------------------
	D:FILE.EXT	| 0 | 1 | false
	D:.EXT		| 0 | 1 | false
---------------+---+---+----------------------------
*)
var
	is_star,is_dot:boolean;

begin
	putMultiText(promptMsgPtr,promptMsgColor);
	if inputLongText(0,11,20,FILEPATHMaxLength,fn,0,0) then
	begin
		// Recognize the entered name
		is_star:=findChar(fn,'*',1)>0;
		is_dot:=findChar(fn,'.',1)>0;
		if (not is_star) and (not is_dot) then
		begin
			move(resptr[wild_allFiles],@fn[1+byte(fn[0])],3);
			fn[0]:=char(byte(fn[0])+3);
		end;
		result:=byte((not is_star) and (is_dot));

		// update name on screen
		conv2internalP2P(@fn[1],@statusBar,width_filePath);
	end
	else
		result:=-1;
end;

(*
function IOPrompt(promptMsgPtr:pointer; var fn:string):shortint;
var
	res:shortint;
	fName:string[16];

begin
	move(@screen,@tmpbuf,240);
	box(0,2,20,10,$40);
	res:=filenamePrompt(promptMsgPtr,1,currentFile);
	if res=0 then
	begin
		if IODirLoop(currentFile,fName) then
			currentFile:=joinPathName(currentFile,fName)
		else
			res:=-1;
	end;
	move(@tmpbuf,@screen,240);
end;
*)
