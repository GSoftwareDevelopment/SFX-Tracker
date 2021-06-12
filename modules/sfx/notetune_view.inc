procedure updateNoteTune(noteShift:byte);
const
	indChar:array[0..31] of byte = (
		$3b,$3b,	// f +7
		$3c,$3b,	// e +6
		$3d,$3b,	// d +5
		$3e,$3b,	// c +4
		$00,$3b,	// b +3
		$00,$3c,	// a +2
		$00,$3d,	// 9 +1
		$00,$3e,	// 8 +0
		$1b,$00,	// 7 -1
		$1d,$00,	// 6 -2
		$20,$00,	// 5 -3
		$3b,$00,	// 4 -4
		$3b,$1b,	// 3 -5
		$3b,$1d,	// 2 -6
		$3b,$20,	// 1 -7
		$3b,$3b	// 0 -8
	);

var
	note_val:array[0..0] of byte;
	v,i,scrOfs:byte;

begin
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
				screen[scrOfs]:=indChar[v];
				inc(scrOfs,20); inc(v);
				screen[scrOfs]:=indChar[v];
			end
			else
			begin
				screen[scrOfs]:=indChar[v];
				inc(scrOfs,20); inc(v);
				screen[scrOfs]:=indChar[v];
				inc(scrOfs,20); inc(v);
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
			screen[scrOfs]:=$00; inc(scrOfs,20);
			screen[scrOfs]:=$3f; inc(scrOfs,20);
			screen[scrOfs]:=$00;	inc(scrOfs,20);
			screen[scrOfs]:=$00; inc(scrOfs,20);
			screen[scrOfs]:=$10;
		end;
	end;
end;

procedure NoteTuneScreen();
var scrOfs:byte;

begin
	move(@screen,@tmpbuf,240);
	fillchar(@moduleBar[20],200,0);

	VBar(0,2,width_menuBar,0);
//	menuBar(resptr[menu_note_tune],width_menuBar,1);

	scrOfs:=tunePianoPos;
	move(resptr[pianoTuneOdd],@screen[scrOfs],12);
	scrOfs:=scrOfs+20;
	move(resptr[pianoTuneEven],@screen[scrOfs],12);
end;
