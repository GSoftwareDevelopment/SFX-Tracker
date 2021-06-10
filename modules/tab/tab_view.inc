procedure updateTABInfo();
begin
	putValue(1,1,currentTAB,2,1);
	fillchar(@screen[24],8,0);
	putASCIIText(4,1,TABName,1);
	colorHLine(0,1,13,1);
	putValue(17,1,TABLen,3,0);
end;

procedure updateTABSFX();
var addr,scrOfs:word;

begin
	fillchar(@statusBar,20,0);
	scrOfs:=224;
	addr:=SFXPtr[currentSFX];
	if (addr<$ffff) then
		conv2internalP2P(@_mem[addr],@screen[scrOfs],SFXNameLength)
	else
		conv2internalP2P(resptr[str_notDefined],@screen[scrOfs],SFXNameLength);

	statusBar[0]:=$03;
	putValue(1,11,currentSFX,2,0);
	statusBar[3]:=$1a;
end;

procedure updateTAB(showPos:boolean);
var
	scrOfs,i,_note,_fnsfx,noteOfs,noteOct,pos:byte;
	ch,_beat,_nxt:byte;
	note_names:array[0..0] of byte;

begin
	note_names:=resptr[str_NoteNames];
	scrOfs:=vadr[winYStart]+winXStart;
	if song_beat>1 then
		_beat:=cursorShift mod song_beat;

	_nxt:=cursorShift;
	pos:=cursorShift; _fnsfx:=0;
	for i:=2 to 10 do
	begin
		fillchar(@screen[scrOfs],16,0);
		putValue(scrOfs,0,_nxt,3,2); scrOfs:=scrOfs+5;

		_fnsfx:=TAB_fnSFX[pos]; _note:=TAB_notes[pos];
		if (_fnsfx=TABFn_end) and (_note=TABFn_end_param) then
		begin
			move(note_names[33],@screen[scrOfs],6); // ENDTAB
			scrOfs:=scrOfs+9;
		end
		else
			if (_fnsfx and $c0=$00) then
			begin // note
				noteOct:=_note div 12; noteOfs:=(_note mod 12) shl 1;

				// NOTE#
				screen[scrOfs]:=note_names[noteOfs]; noteOfs:=noteOfs+1;
				screen[scrOfs+1]:=note_names[noteOfs];
				scrOfs:=scrOfs+2;
				if (noteOct<=9) then
					screen[scrOfs]:=$10+noteOct
				else
					screen[scrOfs]:=$0d; // hypen sign
				scrOfs:=scrOfs+2;

				// SFX#
				putValue(scrOfs,0,_fnsfx,2,0);
				scrOfs:=scrOfs+5;

			end
			else
				if (_fnsfx and $c0=$40) then
				begin // frequency value
					_fnsfx:=_fnsfx and $3f;
					putValue(scrOfs,0,_note,3,0);
					scrOfs:=scrOfs+3;
					screen[scrOfs]:=$0f; // '/'
					scrOfs:=scrOfs+1;
					putValue(scrOfs,0,_fnsfx,2,0);
					scrOfs:=scrOfs+5;
				end
				else // function
				begin
					if (_fnsfx and $40=$40) then
					begin
						if (_note and $80=TABFn_noteOff_param) then
							move(note_names[45],@screen[scrOfs],6) // === .. (note off)
						else
							move(note_names[39],@screen[scrOfs],6); // ... .. (blank)
						scrOfs:=scrOfs+9;
					end
					else
						if (_fnsfx and $3f=0) then
						begin // jump to
							scrOfs:=scrOfs+2;
							screen[scrOfs]:=$1e;
							scrOfs:=scrOfs+1;
							putValue(scrOfs,0,_note,3,0); // note gives position
							scrOfs:=scrOfs+6;
						end
						else
						begin // repeat
							ch:=(_fnsfx-128); // repeat times is value from 1 to 9
							putValue(scrOfs,0,ch,2,0);
							scrOfs:=scrOfs+2;
							screen[scrOfs]:=$1e;
							scrOfs:=scrOfs+1;
							putValue(scrOfs,0,_note,3,0); // note gives position
							scrOfs:=scrOfs+6;
						end;
				end;

		if (pos>=TABLen) then
			colorHLine(winXStart+3,i,13,2)
		else
		begin
			if showPos and (song_lpb>1) then
			begin
				if _beat=0 then
					colorHLine(winXStart+3,i,13,2)
				else
					colorHLine(winXStart+3,i,13,0);
				_beat:=_beat+1; if _beat=song_beat then _beat:=0;
			end
			else
				colorHLine(winXStart+3,i,13,0);
		end;
		pos:=pos+1;
		scrOfs:=scrOfs+6;
		_nxt:=_nxt+1;
 	end;
end;

procedure TABScreen();
begin
	fillchar(@moduleBar,220,$00);
	VBar(0,1,width_menuBar,0);
	screen[33]:=$07;

	menuBar(resptr[menu_tabs],width_menuBar,1);
end;
