procedure updateSONG(indPos:boolean);
var
	i,chn,scrOfs,songOfs,_line,_pos:byte;
	order:byte;

begin
	scrOfs:=44; songOfs:=SONGShift*4; _line:=SONGShift;
	_pos:=SONGShift+SONGPos;
	for i:=0 to 8 do
	begin
		screen[scrOfs]:=$00;
		if (indPos) and (_line=_pos) then
			screen[scrOfs]:=$1e;
		scrOfs:=scrOfs+1;

		putValue(scrOfs,0,_line,2,0); scrOfs:=scrOfs+2;
		screen[scrOfs]:=$1a; scrOfs:=scrOfs+1;

		order:=SONGData[songOfs];
		if order<chnOrder then
		begin
			chn:=1;
			repeat
				if order<maxTABs then
					putValue(scrOfs,0,order,2,0)
				else
				begin
					if order=chnBlank then
					begin
						screen[scrOfs]:=$3f;
						screen[scrOfs+1]:=$3f;
					end
					else
					if order=chnOff then
					begin
						screen[scrOfs]:=$3c;
						screen[scrOfs+1]:=$3c;
					end
				end;
				scrOfs:=scrOfs+3;
				songOfs:=songOfs+1;
				order:=SONGData[songOfs];
				chn:=chn+1;
			until chn>4;
			scrOfs:=scrOfs+4;
		end
		else
		begin
			case Order of
				chnOrder_Tempo: begin
					screen[scrOfs]:=$34; scrOfs:=scrOfs+1; // T
					screen[scrOfs]:=$1a; scrOfs:=scrOfs+2;	// :
					scrOfs:=scrOfs+13;
				end;
				chnOrder_JumpTo: begin
					screen[scrOfs]:=$2a; scrOfs:=scrOfs+1; // J
					screen[scrOfs]:=$1a; scrOfs:=scrOfs+2; // :
					scrOfs:=scrOfs+13;
				end;
				chnOrder_Repeat: begin
					screen[scrOfs]:=$32; scrOfs:=scrOfs+1; // R
					screen[scrOfs]:=$1a; scrOfs:=scrOfs+2; // :
					scrOfs:=scrOfs+13;
				end;
//				chnOrder_Title: begin
//
//				end;
				chnOrder_EndSong: begin
					move(resptr[str_EndSONGOrder],@screen[scrOfs],8); // 8 - hardcoded string length!
					scrOfs:=scrOfs+16;
				end;
			end;
			songOfs:=songOfs+4;
		end;
		_line:=_line+1;
	end;
	conv2internalP2P(@SONGTitle[1],@moduleBar,20);
end;

procedure SONGScreen();
begin
	fillchar(@moduleBar,220,$00);
	VBar(0,1,width_menuBar,0);
	screen[23]:=0;
	updateSONG(true);
	menuBar(resptr[menu_song],width_menuBar,1);
end;
