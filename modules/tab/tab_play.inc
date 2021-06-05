procedure TAB_Play();
begin
	reset_pianoVis();
	SFX_PlayTAB(0,currentTAB);
	repeat
		updatePianoVis();
		if kbcode<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			case key of
				key_ESC: break;
			end;
		end;
	until false;
	SFX_Off();
	reset_pianoVis();
end;
