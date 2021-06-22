{$i modules/song/song_play.inc}

procedure SONGSelectMenuBar(section:byte);
begin
	case section of
		0: SONGEditLoop();
		1: SONG_Play();
		2: SONG_Options(SONGMenuOption_Back);
	end;
end;
