(*
const
	__blank		= $FEFF;	// 1111 1110|1111 1111
	__note 		= $0000; // 0000 0000|0000 0000 $ssnn - ss-SFX; nn-note number ($on - o-octave; n-note in octave)
	__noteValue	= $8000;	// 1000 0000|0000 0000 $ssvv - ss-sfx with 7bit set ($80-$e3); vv - value ($00-$ff) frequency divider
	__repeat		= $e500;	// 1110 0101|0000 0000 $rptp - tp-repeat times 1-9 ($e5-$ee); tp - tab position
	__jump		= $e400; // 1110 0100|0000 0000 $e4tp - tp-tab position
	__end			= $ffff; // 1111 1111|1111 1111
*)

procedure setEndTab();
begin
	TAB_notes[cursorShift+cursorPos]:=255;
	TAB_fnSFX[cursorShift+cursorPos]:=255;
	modified:=true;
end;

procedure setJumpTo();
begin
	if (TAB_fnSFX[cursorShift+cursorPos]<>228) then
	begin
		TAB_notes[cursorShift+cursorPos]:=0;
		TAB_fnSFX[cursorShift+cursorPos]:=228;
		modified:=true;
	end;
end;

procedure editJumpTo(jumpPos:byte);
var
	v:integer;

begin
	v:=jumpPos;
	if inputValue(winXStart+8,2+cursorPos,3,v,0,TAB_maxLength-1,0,color_selected) then
	begin
		jumpPos:=byte(v);
		TAB_notes[cursorShift+cursorPos]:=jumpPos;
		TAB_fnSFX[cursorShift+cursorPos]:=228;
		modified:=true;
  	end;
end;

//

procedure setRepeat();
begin
	if (TAB_fnSFX[cursorShift+cursorPos]<229) or (TAB_fnSFX[cursorShift+cursorPos]>238) then
	begin
		TAB_notes[cursorShift+cursorPos]:=0;
		TAB_fnSFX[cursorShift+cursorPos]:=229;
		modified:=true;
	end;
end;

procedure editRepeat(times,repeatPos:byte);
var
	v:integer;

begin
	times:=times-228;
	v:=times;
	if inputValue(winXStart+6,2+cursorPos,1,v,1,9,0,color_selected) then
	begin
		times:=byte(v);
		v:=repeatPos;
		if inputValue(winXStart+8,2+cursorPos,3,v,0,TAB_maxLength-1,0,color_selected) then
		begin
			repeatPos:=byte(v);
			TAB_notes[cursorShift+cursorPos]:=repeatPos;
			TAB_fnSFX[cursorShift+cursorPos]:=228+times;
			modified:=true;
  		end;
  	end;
end;

//

procedure setNoteValue();
begin
	if (TAB_fnSFX[cursorShift+cursorPos] and $80<>$80) then
	begin
		TAB_notes[cursorShift+cursorPos]:=0;
		TAB_fnSFX[cursorShift+cursorPos]:=currentSFX or $80;
		modified:=true;
	end;
end;

procedure editNoteValue(value:byte);
var
	v:integer;

begin
	v:=value;
	if inputValue(winXStart+5,2+cursorPos,3,v,0,255,0,color_selected) then
	begin
		value:=byte(v);
		v:=currentSFX and $7f;
		if inputValue(winXStart+9,2+cursorPos,2,v,0,99,0,color_selected) then
		begin;
			TAB_notes[cursorShift+cursorPos]:=value;
			TAB_fnSFX[cursorShift+cursorPos]:=byte(v) or $80;
			modified:=true;
  		end;
  	end;
end;

//

procedure setSFXChange();
begin
	// TODO: seach up from cursor position, first SFX set, and use it to set funtion
	TAB_fnSFX[cursorShift+cursorPos]:=0;
end;

procedure editSFXChange(nSFX:byte);
var
	v:integer;

begin
	v:=nSFX and $7f;
	if inputValue(winXStart+9,2+cursorPos,2,v,0,99,0,color_selected) then
	begin;
		TAB_fnSFX[cursorShift+cursorPos]:=byte(v);
		modified:=true;
	end;
end;

//

procedure setNote(note,sfx:byte);
begin
	TAB_notes[cursorShift+cursorPos]:=note;
	TAB_fnSFX[cursorShift+cursorPos]:=sfx;
	modified:=true;
end;

//

procedure setBlank();
begin
	TAB_notes[cursorShift+cursorPos]:=$ff;
	TAB_fnSFX[cursorShift+cursorPos]:=$fe;
	modified:=true;
end;

//
//
//

procedure entryEdit();
var _note,_fnsfx,_pos:byte;
	opt:shortint;

begin
	_pos:=cursorShift+cursorPos;
	_note:=TAB_notes[_pos];
	_fnsfx:=TAB_fnSFX[_pos];
	case _fnsfx of
		128..227: opt:=3;	// note value
		228: opt:=1;		// jump to
		229..238: opt:=2;	// repeat
		254: opt:=5; 		// blank
		255: opt:=0; 		// end-tab
	end;
	if (_note=0) and (_fnsfx<228) then opt:=4;
	optionsList(resptr[menu_tab_edit],width_menuTABFunc,7,opt);
	case opt of
		0: setEndTab();
		1: setJumpTo();
		2: setRepeat();
		3: setNoteValue();
		4: setSFXChange();
		5: setBlank();
	end;
	box(4,2,16,9,0);
	updateTAB(true);
	colorHLine(winXStart,2+cursorPos,20-winXStart,color_selected);
	_note:=TAB_notes[_pos];
	_fnsfx:=TAB_fnSFX[_pos];
	case opt of
		1: editJumpTo(_note);
		2: editRepeat(_fnsfx,_note);
		3: editNoteValue(_note);
		4: editSFXChange(_fnsfx and $7f);
	end;
end;
