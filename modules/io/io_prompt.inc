function filenamePrompt(promptMsgPtr:pointer; promptMsgColor:byte; var fn:string):boolean;
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
		result:=(not is_star) and (is_dot);

		// update name on screen
		if length(fn)<width_filePath then
			conv2internalP2P(@fn[1],@screen[220],length(fn))
		else
			conv2internalP2P(@fn[1],@screen[220],width_filePath);
	end
	else
		result:=false;
end;

(* How is the entered name recognized?

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
