var
	multiChnMode:boolean = false;

procedure updatePiano();
begin
	if multiChnMode then
		pianoScreen[0]:=byte('M'~)
	else
		pianoScreen[0]:=byte('S'~);
	pianoScreen[20]:=$10+currentOct;
	pianoScreen[39]:=$12+currentOct;
end;

function pianoKeyScan(playChn:byte):byte;
begin
	result:=keyScan(key,keys_notes,34);
	if result<>255 then
	begin
		if result>16 then result:=result-5;
		result:=result+(currentOct*12);
		if playChn<>255 then
			SFX_Note(playChn,result,currentSFX);
	end;
end;

procedure sfx_piano();
var
	i,chn,Note:byte;

begin
	reset_pianoVis();
	chn:=0;
	repeat
		updatePianoVis();
		if kbcode<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			if controlSelectionKeys(key,key_SHIFT_TAB,key_TAB,currentOct,0,3) then
				updatePiano();
			case key of
				key_ESC: break;
				key_INVERS: begin multiChnMode:=not multiChnMode; updatePiano(); end;
			end;
			i:=pianoKeyScan(chn);
			if i<>255 then
				if multiChnMode then
					if chn<3 then chn:=chn+1 else chn:=0;
		end;
	until false;
	reset_pianoVis();
end;
