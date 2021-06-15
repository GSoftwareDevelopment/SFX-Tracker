procedure updateNoteTune(noteShift:byte);
var
	indChar:array[0..0] of byte;
	v,i,scrOfs,_pos:byte;

begin
	definedNoteTune:=pointer(NOTE_TABLE_ADDR+currentNoteTableOfs);
	indChar:=resptr[tuneIndChars];
	for i:=0 to 11 do
	begin
		scrOfs:=tuneBarsPos+i;
		_pos:=noteShift+i;
		if (_pos<64) then
		begin
			v:=(definedNoteTune[_pos] and $0f) shl 1;
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
			v:=definedNoteTune[_pos] shr 4;
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
end;

procedure NoteTunePiano();
begin
	move(resptr[pianoTuneOdd],@screen[tunePianoPos],12);
	move(resptr[pianoTuneEven],@screen[tunePianoPos+20],12);
end;

procedure NoteTuneRedraw();
begin
	box(4,2,16,9,0);
	noteSetId:=currentNoteTableOfs shr 6;
	putNText(4,2,pointer(word(resptr[menu_NoteDef])+4+noteSetId*16),1);
	updateNoteTune(currentOct*12);
	NoteTunePiano();
end;

procedure NoteTuneScreen();
begin
	VBar(0,2,width_menuBar,0);
end;
