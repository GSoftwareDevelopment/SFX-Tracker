procedure updateSONG(indPos:boolean);
var
	i,chn,scrOfs,songOfs,_line,_pos:byte;
	order:byte;

begin
	scrOfs:=44; songOfs:=SONGShift*4; _line:=SONGShift;
	clearWorkarea();
	for i:=0 to 8 do
	begin
		putValue(scrOfs,0,_line,2,2);
		scrOfs:=scrOfs+2;

		order:=SONGData[songOfs];
		if order<chnOrder then
		begin
			chn:=0;
			repeat
				if indPos then
				begin
					_pos:=channels[chn*$10+_trackOfs] shr 2;
					if _pos=_line then
						screen[scrOfs]:=$1e;
				end;
				scrOfs:=scrOfs+1;
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
				scrOfs:=scrOfs+2;
				songOfs:=songOfs+1;
				order:=SONGData[songOfs];
				chn:=chn+1;
			until chn=4;
			scrOfs:=scrOfs+6;
		end
		else
		begin
			scrOfs:=scrOfs+1;
			case Order of
				chnOrder_Tempo: begin
						screen[scrOfs  ]:=$34; // T
						screen[scrOfs+1]:=$1a; // :
						scrOfs:=scrOfs+3;
						putValue(scrOfs,0,SONGData[songOfs+1],2,0);
						scrOfs:=scrOfs+14;
					end;
				chnOrder_JumpTo: begin
						screen[scrOfs  ]:=$2a; // J
						screen[scrOfs+1]:=$1e; // >
						scrOfs:=scrOfs+3;
						putValue(scrOfs,0,SONGData[songOfs+1],2,0);
						scrOfs:=scrOfs+14;
					end;
				chnOrder_Repeat: begin
						screen[scrOfs  ]:=$32; // R
						screen[scrOfs+1]:=$1a; // :
						scrOfs:=scrOfs+3;
						putValue(scrOfs,0,SONGData[songOfs+1],2,0); scrOfs:=scrOfs+2;
						screen[scrOfs]:=$1e; scrOfs:=scrOfs+1; // >
						putValue(scrOfs,0,SONGData[songOfs+2],2,0);
						scrOfs:=scrOfs+11;
					end;
				chnOrder_EndSong: begin
						move(resptr[str_EndSONGOrder],@screen[scrOfs],8); // 8 - hardcoded string length!
						scrOfs:=scrOfs+17;
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
	clearModule(); // fillchar(@moduleBar,220,$00);
	VBar(0,1,width_menuBar,10,0);
	menuBar(menu_song,width_menuBar,1);
	updateSONG(true);
end;
