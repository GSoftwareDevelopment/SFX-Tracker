{$i modules/song/song_play.inc}
{$i modules/song/song_edit.inc}

procedure SONGSelectMenuBar(section:byte);
begin
   case section of
      0: SONGEditLoop();
      1: SONG_Play(0);
      2: SONG_Options(SONGMenuOption_Back);
   end;
end;
