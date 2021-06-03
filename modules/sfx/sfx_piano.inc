procedure sfx_piano();
var
	i,chn:byte;

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
	SFX_Off();
	reset_pianoVis();
end;
