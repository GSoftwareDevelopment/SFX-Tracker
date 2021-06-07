
{$i modules/song/song_view.inc}
{$i modules/song/song_edit.inc}
{$i modules/song/song_options.inc}
{$i modules/song/song_menubar.inc}

procedure SONGLoop();
begin
	section:=0;
	updateBar(resptr[menu_song],width_menuBar,section,color_choice,color_selected);
	screen2video();
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode);
			controlSelectionKeys(key,key_Up,key_Down,section,0,2);
			case key of
				key_ESC: break;
				key_RETURN: SONGSelectMenuBar(section);
			end;
			updateBar(resptr[menu_song],width_menuBar,section,color_choice,color_selected);
			screen2video();
			kbcode:=255;
		end;
	until false;
	kbcode:=255;
	updateBar(resptr[menu_tabs],width_menuBar,-1,0,0);
end;

procedure SONGModule();
begin
	SONGScreen();
	SONGLoop();
end;
