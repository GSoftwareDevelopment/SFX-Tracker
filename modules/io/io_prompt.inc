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
	wild: string[1] = '*';
	fileDot: string[1] = '.';
	is_star,is_dot:boolean;

begin
	putMultiText(promptMsgPtr,promptMsgColor);
	if inputLongText(0,11,20,FILEPATHMaxLength,fn,0,0) then
	begin
		fillchar(@fn[1+length(fn)],FILEPATHMaxLength-length(fn),0);

		// Recognize the entered name
		is_star:=indexOf(fn,wild,1)>-1;
		is_dot:=indexOf(fn,fileDot,1)>-1;
		if (not is_star) and (not is_dot) then
			fn:=concat(fn,'*.*'); // add '*.*'
		result:=byte((not is_star) and (is_dot));

		// update name on screen
		if length(fn)<width_filePath then
			conv2internalP2P(@fn[1],@screen[220],length(fn))
		else
			conv2internalP2P(@fn[1],@screen[220],width_filePath);
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
