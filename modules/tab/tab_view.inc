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
	scrOfs:=224;
	// addr:=HEAP_GetAddr(SFXPtr[currentSFX]);
	addr:=SFXPtr[currentSFX];
	if (addr<$ffff) then
		conv2internalP2P(@_mem[addr],@screen[scrOfs],SFXNameLength)
	else
		conv2internalP2P(resptr[str_notDefined],@screen[scrOfs],SFXNameLength);

	putValue(1,11,currentSFX,2,1);
	colorHLine(0,11,20,1);
end;

procedure updateTAB(showPos:boolean);
var
	scrOfs,i,_note,_fnsfx,noteOfs,noteOct,pos:byte;
	ch,_tact,_beat,_lpb,_nxt:byte;
	note_names:array[0..0] of byte;

begin
	note_names:=resptr[str_NoteNames];
	scrOfs:=vadr[winYStart]+winXStart+1;
	_tact:=1+(cursorShift div (song_lpb*song_beat));
	_beat:=1+cursorShift div song_beat mod song_beat;
	_lpb:=1+cursorShift mod song_lpb;
	_nxt:=cursorShift;
	pos:=cursorShift; _fnsfx:=0;
	for i:=2 to 10 do
	begin
		putValue(winXStart+1,i,_nxt,3,0);

		_fnsfx:=TAB_fnSFX[pos]; _note:=TAB_notes[pos];
		if (_fnsfx=TABFn_end) and (_note=TABFn_end_param) then
			move(note_names[33],@screen[scrOfs+4],6) // ENDTAB
		else
			if (_fnsfx and $c0=$00) then
			begin // note
				noteOct:=_note div 12; noteOfs:=(_note mod 12) shl 1;

				// NOTE#
				screen[scrOfs+4]:=note_names[noteOfs]; noteOfs:=noteOfs+1;
				screen[scrOfs+5]:=note_names[noteOfs];
				if (noteOct<=9) then
					screen[scrOfs+6]:=$10+noteOct
				else
					screen[scrOfs+6]:=$0d; // hypen sign
				screen[scrOfs+7]:=0;

				// SFX#
				putValue(winXStart+9,i,_fnsfx,2,0);
			end
			else
				if (_fnsfx and $c0=$40) then
				begin // frequency value
					_fnsfx:=_fnsfx and $3f;
					putValue(winXStart+5,i,_note,3,0);
					putValue(winXStart+9,i,_fnsfx,2,0);
					screen[scrOfs+7]:=$0f;
				end
				else // function
					if (_fnsfx and $40=$40) then
					begin
						if (_note and $80=TABFn_noteOff_param) then
							move(note_names[45],@screen[scrOfs+4],6) // === .. (note off)
						else
							move(note_names[39],@screen[scrOfs+4],6); // ... .. (blank)
					end
					else
						if (_fnsfx and $3f=0) then
						begin // jump to
							move(note_names[24],@screen[scrOfs+4],3); // __>___
							putValue(winXStart+8,i,_note,3,0); // note gives position
						end
						else
						begin // repeat
							move(note_names[24],@screen[scrOfs+4],3); // __>___
							ch:=(_fnsfx-128); // repeat times is value from 1 to 9
							putValue(winXStart+5,i,ch,2,0);
	//								screen[scrOfs+5]:=ch;
							putValue(winXStart+8,i,_note,3,0); // note gives position
						end;

		if (showPos) then
		begin
			ch:=$10+_lpb;
			screen[scrOfs+13]:=ch;
			if (_lpb=1) then
			begin
				ch:=$10+_beat;
				screen[scrOfs+12]:=ch;
				if (_beat=1) then ch:=$10+_tact else ch:=0;
				screen[scrOfs+11]:=ch;
			end
			else
			begin
				screen[scrOfs+12]:=0;
				screen[scrOfs+11]:=0;
			end;

			if (_lpb<song_lpb) then _lpb:=_lpb+1
			else
			begin
				_lpb:=1;
				if (_beat<song_beat) then _beat:=_beat+1
				else
				begin
					_beat:=1;
					if (_tact<song_tact) then _tact:=_tact+1 else _tact:=1;
				end;
			end;
		end;

		if (pos>=TABLen) then
			colorHLine(winXStart,i,16,2)
		else
			colorHLine(winXStart,i,16,0);

		pos:=pos+1;
		scrOfs:=scrOfs+20;
		_nxt:=_nxt+1;
 	end;
end;

procedure TABScreen();
begin
	fillchar(@screen[20],220,$00);
	VBar(0,1,width_menuBar,0);
	screen[223]:=$06;
	screen[33]:=$07;

	menuBar(resptr[menu_tabs],width_menuBar,1);
end;
