{$i modules/io/io_view.inc}
{$i modules/io/io_dir.inc}
{$i modules/io/io_save.inc}

procedure IOLoop();
var
	opt:shortint;

begin
	opt:=0;
	updateBar(resptr[menu_IO],width_menuTop,opt,0,color_selected);
	screen2video();
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			case key of
				key_ESC: break;
				key_Left: if (opt>0) then opt:=opt-1;
				key_Right: if (opt<3) then opt:=opt+1;
				key_RETURN:
					case opt of
						0: IODirectory();
						2: IOSave();
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
