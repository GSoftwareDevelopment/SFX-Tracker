{$i modules/sfx/notetune_view.inc}

procedure NoteTuneLoop();
var
	_pos,notePos,noteShift,noteVal,noteKey:byte;
	note_val:array[0..0] of byte;

begin
	note_val:=pointer(NOTE_TABLE_ADDR);
	notePos:=0; noteShift:=0; _pos:=0;
//	colorVLine(66,0,4,3);
	reset_pianoVis();
	updateNoteTune(noteShift);
	screen2video();
	repeat
		if keypressed then
		begin
			case key of
				key_ESC: break;
			end;

			noteKey:=pianoKeyScan(0);
			if noteKey<>255 then
			begin
				_pos:=noteKey;
				noteShift:=(_pos div 12)*12;
				notePos:=_pos mod 12;
			end;

			noteVal:=note_val[_pos];
			if controlSelectionKeys(key,key_Up,key_Down,noteVal,0,255) then
				note_val[_pos]:=noteVal;

			updateNoteTune(noteShift);
			colorVLine(66+notePos,0,5,3);
			screen2video();
		end;
		updatePianoVis();
	until false;
	SFX_Off();
	reset_pianoVis();
end;

procedure setNoteTune();
begin
	NoteTuneScreen();
	NoteTuneLoop();
	move(@tmpbuf,@screen,240);
end;
