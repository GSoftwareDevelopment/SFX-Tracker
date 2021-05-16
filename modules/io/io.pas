{$i modules/io/io_dir.inc}

procedure IOScreen();
var
	opt:shortint;

begin
	move(@screen,@tmpbuf,240);
	fillchar(@screen[20],20,$00);
	menuBar(resptr[menu_IO],menu_Top,0);
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
					end;
			end;
			updateBar(resptr[menu_IO],width_menuTop,opt,0,color_selected);
			screen2video();
		end;
	until false;
	move(@tmpbuf,@screen,240);
end;
