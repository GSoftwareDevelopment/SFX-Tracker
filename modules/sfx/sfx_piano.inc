procedure sfx_piano();
var
	i,chn:byte;

begin
	if SFXPtr[currentSFX]=$FFFF then
	begin
		showError(msg_UnknownDefinition);
		exit;
	end;
	reset_pianoVis();
	chn:=0; keyClick:=false; PLYtm:=timer; PLYstate:=2;
	repeat
		updatePianoVis();
		if keyPressed then
		begin
			case key of
				key_ESC: break;
				key_INVERS: begin multiChnMode:=not multiChnMode; updatePiano(); end;
			end;
			i:=pianoKeyScan(chn);
			if i<>255 then
				if multiChnMode then
					if chn<3 then chn:=chn+1 else chn:=0;
		end;
		if (timer-ScreenTm)>0 then
		begin
			ScreenTm:=timer;
			PlayFlash();
			move(@screen,@video,240);
		end;
	until false;
	SFX_Off(); keyClick:=true;
	reset_pianoVis();
end;
