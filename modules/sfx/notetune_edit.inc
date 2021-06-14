procedure NoteTuneLoop();
var
	_pos,notePos,noteShift,noteVal,noteKey:byte;

begin
	notePos:=0; _pos:=0;
	noteShift:=currentOct*12;
	definedNoteTune:=pointer(NOTE_TABLE_ADDR+currentNoteTableOfs);

	reset_pianoVis();
	updateNoteTune(noteShift);
	colorVLine(tuneBarsPos+notePos,0,5,3);
	screen2video();
	repeat
		if keypressed then
		begin
			case key of
				key_ESC: break;
			end;

			if controlSelectionKeys(key,key_Left,key_Right,_pos,0,63) then
			begin
				noteShift:=(_pos div 12)*12;
				notePos:=_pos mod 12;
			end
			else
			begin
				noteKey:=pianoKeyScan(0);
				if noteKey<>255 then
				begin
					_pos:=noteKey;
					noteShift:=(_pos div 12)*12;
					notePos:=_pos mod 12;
				end;
			end;

			noteVal:=definedNoteTune[_pos];
			if controlSelectionKeys(key,key_Up,key_Down,noteVal,0,255) then
				definedNoteTune[_pos]:=noteVal;

			updateNoteTune(noteShift);
			colorVLine(tuneBarsPos+notePos,0,5,3);
			screen2video();
		end;
		updatePianoVis();
	until false;
	updateNoteTune(noteShift);
	SFX_Off();
	reset_pianoVis();
end;
