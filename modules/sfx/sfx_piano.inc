procedure sfx_piano();
var
	i,chn:byte;

begin
	if SFXPtr[currentSFX]=$FFFF then
	begin
		showError(resptr[msg_UnknownDefinition]);
		exit;
	end;
	reset_pianoVis();
	chn:=0;
	repeat
		updatePianoVis();
		if kbcode<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
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
