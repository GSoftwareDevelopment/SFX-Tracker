
{$i modules/song/song_view.inc}
{$i modules/song/song_edit.inc}
{$i modules/song/song_options.inc}
{$i modules/song/song_menubar.inc}

procedure SONGLoop();
begin
	section:=0;
	repeat
		if optionsList(resptr[menu_song],width_menuBar,3,section,key_Up,key_Down) then
			SONGSelectMenuBar(section)
		else
			break
	until false;
end;

procedure SONGModule();
begin
	SONGScreen();
	SONGLoop();
end;
