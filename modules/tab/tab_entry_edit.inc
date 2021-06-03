function TABEntryEdit():boolean;
var _note,_fnsfx,_pos:byte;
	opt:shortint;

	procedure setEndTab();
	begin
		TAB_notes[_pos]:=TABFn_blank_param;
		TAB_fnSFX[_pos]:=TABFn_blank;
		modified:=true;
	end;

	procedure setJumpTo();
	begin
		if (TAB_fnSFX[_pos]<>128) then
		begin
			TAB_notes[_pos]:=0;
			TAB_fnSFX[_pos]:=TABFn_jump;
			modified:=true;
		end;
	end;

	procedure editJumpTo(jumpPos:byte);
	var
		v:smallint;

	begin
		v:=jumpPos;
		if inputValue(winXStart+8,2+cursorPos,3,v,0,TAB_maxLength-1,0,color_selected) then
		begin
			jumpPos:=byte(v);
			TAB_notes[_pos]:=jumpPos;
			modified:=true;
		end;
	end;

	//

	procedure setRepeat();
	begin
		if (TAB_fnSFX[_pos]<229) or (TAB_fnSFX[_pos]>238) then
		begin
			TAB_notes[_pos]:=0;
			TAB_fnSFX[_pos]:=TABFn_repeat;
			modified:=true;
		end;
	end;

	procedure editRepeat(times,repeatPos:byte);
	var
		v:smallint;

	begin
		times:=times-$80;
		v:=times;
		if inputValue(winXStart+5,2+cursorPos,2,v,1,TAB_maxRepeats,0,color_selected) then
		begin
			times:=byte(v);
			v:=repeatPos;
			if inputValue(winXStart+8,2+cursorPos,3,v,0,TAB_maxLength-1,0,color_selected) then
			begin
				repeatPos:=byte(v);
				TAB_notes[_pos]:=repeatPos;
				TAB_fnSFX[_pos]:=TABFn_repeat+times-1;
				modified:=true;
			end;
		end;
	end;

	//

	procedure setNoteValue();
	begin
		if (TAB_fnSFX[_pos] and $C0<>TABFn_freq) then
		begin
			TAB_notes[_pos]:=0;
			TAB_fnSFX[_pos]:=currentSFX or TABFn_freq;
			modified:=true;
		end;
	end;

	procedure editNoteValue(value:byte);
	var
		v:smallint;

	begin
		v:=value;
		if inputValue(winXStart+5,2+cursorPos,3,v,0,255,0,color_selected) then
		begin
			value:=byte(v);
			v:=currentSFX and $3f;
			if inputValue(winXStart+9,2+cursorPos,2,v,0,maxSFXs-1,0,color_selected) then
			begin;
				TAB_notes[_pos]:=value;
				TAB_fnSFX[_pos]:=byte(v) or TABFn_freq;
				modified:=true;
			end;
		end;
	end;

	//

	procedure setNoteOff();
	begin
		TAB_notes[_pos]:=TABFn_noteOff_param;
		TAB_fnSFX[_pos]:=TABFn_noteOff;
		modified:=true;
	end;

	//

	procedure setBlank();
	begin
		TAB_notes[_pos]:=TABFn_blank_param;
		TAB_fnSFX[_pos]:=TABFn_blank;
		modified:=true;
	end;

begin
	_pos:=cursorShift+cursorPos;
	_note:=TAB_notes[_pos];
	_fnsfx:=TAB_fnSFX[_pos];
	if (_fnsfx and $c0=$00) then
		opt:=6				// note - default option is Back
	else
		if (_fnsfx and $c0=$40) then
			opt:=3			// frequency value
		else
			if (_fnsfx and $40=$40) then
			begin
				if (_note and $80=TABFn_noteOff_param) then
					opt:=4	// note off
				else
					opt:=5;	// blank
			end
			else
			begin
				if _note=0 then
					opt:=1	// jump to
				else
					opt:=2;	// repeat
			end;

	if (_fnsfx=TABFn_end) and (_note=TABFn_end_param) then
		opt:=0;			// end-tab
(*
	case _fnsfx of
		 $40..$7f: opt:=3;	// note frequency value
		      $80: opt:=1;	// jump to
		 $81..$bf: opt:=2;	// repeat
		 $00..$3f: opt:=4;	// note off
		      $00: opt:=5; // blank
		      $00: opt:=0; // end-tab
	end;
	if (_note=255) and (_fnsfx<228) then opt:=4;
*)

	if optionsList(resptr[menu_tab_edit],width_menuTABFunc,TABEditOptions,opt,key_Up,key_Down) then
	begin
		case opt of
			0: setEndTab();
			1: setJumpTo();
			2: setRepeat();
			3: setNoteValue();
			4: setNoteOff();
			5: setBlank();
		end;
		box(4,2,16,9,0);
		updateTAB(true);
		if opt=TABEditOption_BackToEdit then
		begin
			colorHLine(winXStart,2+cursorPos,20-winXStart,color_selected);
			exit(false);
		end;
		if opt=TABEditOption_BackToMenuBar then exit(true);
		_note:=TAB_notes[_pos];
		_fnsfx:=TAB_fnSFX[_pos];
		case opt of
			1: editJumpTo(_note);
			2: editRepeat(_fnsfx,_note);
			3: editNoteValue(_note);
		end;
		result:=false;
	end
	else
	begin
		box(4,2,16,9,0);
		updateTAB(true);
	end;
end;
