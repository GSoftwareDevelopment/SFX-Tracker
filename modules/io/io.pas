{$i modules/io/io_load.inc}
{$i modules/io/io_save.inc}
{$i modules/io/io_quit.inc}

procedure IOModule();
begin
	section:=1;
	clearTopMenu();
	if optionsList(menu_IO,width_menuTop,5,section,key_Left,key_Right) then
	begin
		case section of
			0: exit;
			1: IOLoad();
			2: IOSave();
			4: IOQuit();
		end;
		clearModule();
	end;
end;
