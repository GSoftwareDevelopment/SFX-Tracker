{$i modules/io/io_view.inc}
{$i modules/io/io_dir.inc}
{$i modules/io/io_load.inc}
{$i modules/io/io_save.inc}
{$i modules/io/io_quit.inc}

procedure IOLoop();
begin
	section:=1;
	repeat
		if optionsList(resptr[menu_IO],width_menuTop,5,section,key_Left,key_Right) then
			case section of
				0: break;
				1: IODirectory();
				2: IOLoad();
				3: IOSave();
				4: IOQuit();
			end
		else
			break;
	until false;
	move(@tmpbuf,@screen,240);
end;

procedure IOModule();
begin
	IOScreen();
	IOLoop();
end;
