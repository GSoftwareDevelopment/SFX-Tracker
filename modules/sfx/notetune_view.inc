procedure updateNoteTune(noteShift:byte);
var
	indChar:array[0..0] of byte;
	note_val:array[0..0] of byte;
	v,i,scrOfs:byte;

begin
	indChar:=resptr[tuneIndChars];
	note_val:=pointer(NOTE_TABLE_ADDR+noteShift);
	for i:=0 to 11 do
	begin
		scrOfs:=tuneBarsPos+i;
		if (noteShift+i<64) then
		begin
			v:=(note_val[i] and $0f) shl 1;
			// tune bar
			if v>15 then
			begin
				screen[scrOfs]:=$00; inc(scrOfs,20);
				screen[scrOfs]:=$3f; inc(scrOfs,20);
				screen[scrOfs]:=indChar[v]; inc(scrOfs,20);
				screen[scrOfs]:=indChar[v+1];
			end
			else
			begin
				screen[scrOfs]:=indChar[v]; inc(scrOfs,20);
				screen[scrOfs]:=indChar[v+1]; inc(scrOfs,20);
				screen[scrOfs]:=0; inc(scrOfs,20);
				screen[scrOfs]:=0;
			end;
			inc(scrOfs,20);
			// high nibbel of note frequency
			v:=note_val[i] shr 4;
			if (v<10) then
				screen[scrOfs]:=$10+v
			else
				screen[scrOfs]:=$17+v;
		end
		else
		begin
			screen[scrOfs]:=$00;// inc(scrOfs,20);
			screen[scrOfs+20]:=$3f;// inc(scrOfs,20);
			screen[scrOfs+40]:=$00;//	inc(scrOfs,20);
			screen[scrOfs+60]:=$00;// inc(scrOfs,20);
			screen[scrOfs+80]:=$0d;
		end;
	end;
	scrOfs:=tunePianoPos;
	move(resptr[pianoTuneOdd],@screen[scrOfs],12);
	move(resptr[pianoTuneEven],@screen[scrOfs+20],12);
end;

procedure NoteTuneScreen();
var scrOfs:byte;

begin
	move(@screen,@tmpbuf,240);
	fillchar(@moduleBar[20],200,0);

	VBar(0,2,width_menuBar,0);
//	menuBar(resptr[menu_note_tune],width_menuBar,1);

end;
