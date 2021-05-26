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
	addr:=HEAP_GetAddr(SFXPtr[currentSFX]);

	if (addr<$ffff) then
		conv2internalP2P(@_mem[addr],@screen[scrOfs],SFXNameLength)
	else
		conv2internalP2P(resptr[str_SFX_notDefined],@screen[scrOfs],SFXNameLength);

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
	_nxt:=cursorShift;
	_tact:=1+(cursorShift div (song_lpb*song_beat));
	_beat:=1+cursorShift div song_beat mod song_beat;
	_lpb:=1+cursorShift mod song_lpb;
	pos:=cursorShift; _fnsfx:=0;
	for i:=2 to 10 do
	begin
		putValue(winXStart+1,i,_nxt,3,0);

		if (_fnsfx<>255) then
		Begin
			_note:=TAB_notes[pos]; _fnsfx:=TAB_fnSFX[pos];
			case _fnsfx of
				0..99: begin // sfx note
					noteOfs:=(_note and $f) shl 1; noteOct:=_note shr 4;

					if _note<255 then
					begin
						// NOTE#
						screen[scrOfs+4]:=note_names[noteOfs]; noteOfs:=noteOfs+1;
						screen[scrOfs+5]:=note_names[noteOfs];
						if (noteOct<=9) then
							screen[scrOfs+6]:=$10+noteOct
						else
							screen[scrOfs+6]:=$0d; // hypen sign
						screen[scrOfs+7]:=0;
					end
					else
						move(note_names[45],@screen[scrOfs+4],4);

					// SFX#
					putValue(winXStart+9,i,_fnsfx,2,0);
				end;
				128..227: begin // sfx direct value
					_fnsfx:=_fnsfx and $7f;
					putValue(winXStart+5,i,_note,3,0);
					putValue(winXStart+9,i,_fnsfx,2,0);
					screen[scrOfs+7]:=$0f;
				end;
				228: begin // jump to
					move(note_names[24],@screen[scrOfs+4],3); // __>___
					putValue(winXStart+8,i,_note,3,0); // note gives position
				end;
				229..238: begin // repeat
					move(note_names[27],@screen[scrOfs+4],6); // R_>___
					ch:=$10+(_fnsfx-228); // repeat times is value from 1 to 9
					screen[scrOfs+5]:=ch;
					putValue(winXStart+8,i,_note,3,0); // note gives position
				end;
				254: move(note_names[39],@screen[scrOfs+4],6); // ... .. (blank)
				255: move(note_names[33],@screen[scrOfs+4],6); // ENDTAB
			end;
			pos:=pos+1;
		end
		else
			fillchar(@screen[scrOfs+4],7,0);

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

		scrOfs:=scrOfs+20;
		_nxt:=_nxt+1;
 	end;
end;
