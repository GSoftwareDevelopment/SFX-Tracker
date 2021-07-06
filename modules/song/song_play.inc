procedure SONG_Play();
var
	oldTempo:byte;

begin
	oldTempo:=SONG_Tempo;
	SFX_PlaySONG(0,SONG_Tempo);
	reset_pianoVis(); // keyClick:=false;
	PLYtm:=timer; PLYstate:=2;
	repeat
		updatePianoVis();
		PlayFlash();
		updateSong(true);
		updateSongStatus();

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
	until SONG_Tick>$80;
	SFX_Off(); keyClick:=true;
	reset_pianoVis();
	SONG_Tempo:=oldTempo;
	clearStatusBar();
end;
