procedure SONG_Play();
begin
	SFX_PlaySONG(0);
	reset_pianoVis(); // keyClick:=false;
	PLYtm:=timer; PLYstate:=2;
	repeat
		updatePianoVis();
		PlayFlash();
		updateSong(true);
		if keyPressed then
		begin
			case key of
				key_ESC: break;
			end;
		end;
		if (timer-ScreenTm)>0 then
		begin
			ScreenTm:=timer;
			move(@screen,@video,240);
		end;
	until false;
	SFX_Off(); keyClick:=true;
	reset_pianoVis();
end;
