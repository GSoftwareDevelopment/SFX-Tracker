{$i modules/io/io_view.inc}
{$i modules/io/io_dir.inc}
{$i modules/io/io_save.inc}
{$i modules/io/io_quit.inc}

procedure IOLoop();
var
	opt:byte;

begin
	opt:=1;
	updateBar(resptr[menu_IO],width_menuTop,opt,0,color_selected);
	screen2video();
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			controlSelectionKeys(key,key_Left,key_Right,opt,0,4);
			case key of
				key_ESC: break;
				key_RETURN:
					case opt of
						0: break;
						1: IODirectory();
						3: IOSave();
						4: IOQuit();
					end;
			end;
			updateBar(resptr[menu_IO],width_menuTop,opt,0,color_selected);
			screen2video();
		end;
	until false;
	move(@tmpbuf,@screen,240);
end;

procedure IOModule();
begin
	IOScreen();
	IOLoop();
end;
