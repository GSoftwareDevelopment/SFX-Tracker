function TABEntryEdit():boolean;
var _note,_fnsfx,_pos:byte;
	opt:byte;

	procedure editJumpTo(jumpPos:byte);
	var
		v:smallint;

	begin
		fillchar(@statusBar,20,0);
		putMultiText(msg_TAB_JumpToPrompt,0);
		putValue(17,11,_pos,3,0);
		v:=jumpPos;
		if (_fnsfx<>TABFn_jump) then v:=0;
		if inputValue(winXStart+8,2+cursorPos,3,v,0,_pos,0,color_selected) then
		begin
			jumpPos:=byte(v);
			TABPutIn(_pos,jumpPos,TABFN_jump);
		end;
	end;

	//

	procedure editRepeat(times,repeatPos:byte);
	var
		v:smallint;

	begin
		times:=times-$80;
		v:=times;
		if (_fnsfx<TABFn_repeat) or (_fnsfx>TABFn_repeatMax) then v:=1;
		fillchar(@statusBar,20,0);
		putMultiText(msg_TAB_RepeatPrompt,0);
		if inputValue(winXStart+5,2+cursorPos,2,v,1,TAB_maxRepeats,0,color_selected) then
		begin
			editJumpTo(repeatPos);
			times:=byte(v)-1;
			TAB_fnSFX[_pos]:=TABFN_repeat+times;
		end;
	end;

	//
(*
	procedure setNoteValue();
	begin
		if (TAB_fnSFX[_pos] and $C0<>TABFn_freq) then
		begin
			if (TAB_fnSFX[_pos] and $C0=TABFn_note) then
				TAB_notes[_pos]:=note_val[TAB_notes[_pos]]
			else
				TAB_notes[_pos]:=0;
			TAB_fnSFX[_pos]:=currentSFX or TABFn_freq;
			modified:=true;
		end;
	end;
*)
	procedure editNoteValue(value:byte);
	var
		v:smallint;
		note_val:byteArray;

	begin
		v:=value;
		if (_fnsfx and $C0=TABFn_note) then
		begin
			note_val:=pointer(NOTE_TABLE_ADDR+SFXNoteSetOfs[_fnsfx and $3f]);
			v:=note_val[v];
		end
		else
			if (_fnsfx and $C0<>TABFn_freq) then
				v:=0;
		fillchar(@statusBar,20,0);
		putMultiText(msg_TAB_FreqPrompt,0);
		if inputValue(winXStart+5,2+cursorPos,3,v,0,255,0,color_selected) then
		begin
			value:=byte(v);
			v:=currentSFX and $3f;
			fillchar(@statusBar,20,0);
			putMultiText(msg_TAB_SFXIdPrompt,0);
			if inputValue(winXStart+9,2+cursorPos,2,v,0,maxSFXs-1,0,color_selected) then
			begin
				if SFXPtr[byte(v)]=$FFFF then
				begin
					showError(msg_UnknownDefinition);
					exit;
				end;

				TABPutIn(_pos,value,byte(v) or TABFn_freq);
			end;
		end;
	end;

	//

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
				if _fnsfx and $3f=0 then
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

	if optionsList(menu_tab_edit,width_menuTABFunc,TABEditOptions,opt,key_Up,key_Down) then
	begin
		case opt of
			0: TABPutIn(_pos,TABFN_end_param,TABFN_end);// setEndTab();
//			1: setJumpTo();
//			2: setRepeat();
//			3: setNoteValue();
			4: TABPutIn(_pos,TABFn_noteOff_param,TABFn_noteOff);// setNoteOff();
			5: TABPutIn(_pos,TABFn_blank_param,TABFn_blank);// setBlank();
		end;
		clearWorkarea();
		updateTAB(true);
		if opt>=TABEditOption_BackToEdit then exit(opt=TABEditOption_BackToMenuBar);

		_note:=TAB_notes[_pos];
		_fnsfx:=TAB_fnSFX[_pos];
		case opt of
			1: editJumpTo(_note);
			2: editRepeat(_fnsfx,_note);
			3: editNoteValue(_note);
		end;
		fillchar(@statusBar,20,0);
		result:=false;
	end
	else
	begin
		box(4,2,16,9,0);
		updateTAB(true);
	end;
end;
