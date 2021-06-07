procedure SONGSelectMenuBar(section:byte);
begin
	case section of
		0: SONGEditLoop();
		1: exit;
		2: SONG_Options(SONGMenuOption_Back);
	end;
end;
