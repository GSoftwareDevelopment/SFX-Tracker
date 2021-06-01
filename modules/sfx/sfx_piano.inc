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

procedure sfx_piano();
var
	ofs,i,chn,Note:byte;
	otm:longint;

begin
	init_pianoVis();
	updatePiano();
	SFX_Start;
	chn:=0;
	repeat
		if (getTime-otm>0) then
		begin
			otm:=getTime;
			updatePianoVis();
		end;
		if kbcode<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			if controlSelectionKeys(key,key_SHIFT_TAB,key_TAB,currentOct,0,3) then
				updatePiano();
			case key of
				key_ESC: break;
				key_INVERS: begin multiChnMode:=not multiChnMode; updatePiano(); end;
			end;
			i:=keyScan(key,keys_notes,34);
			if i<>255 then
			begin
			if i>16 then Note:=i-5 else Note:=i;
				Note:=Note+(currentOct*12);
				// SFX_Note(chn,Note,SFXMode,SFXAddr);
				SFX_Note(chn,Note,currentSFX);
				if multiChnMode then
					if chn<3 then chn:=chn+1 else chn:=0;
			end;
		end;
	until false;
	SFX_End();
	done_pianoVis();
end;
