{$i modules/gsd/dli_interrupt.inc}
{$i modules/gsd/gsd_view.inc}
{$i modules/gsd/credits.inc}
{$i modules/theme/theme_selector.inc}
{$i modules/gsd/memory_stats.inc}


procedure GSDLoop();
var
	opt:shortint;

begin
	opt:=0;
	updateBar(resptr[menu_GSD],width_menuTop,opt,0,color_selected);
	screen2video();
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			case key of
				key_ESC: break;
				key_Left: if (opt>0) then opt:=opt-1;
				key_Right: if (opt<2) then opt:=opt+1;
				key_RETURN: case opt of
					0: credits();
					1: theme_selector();
					2: memory_stats();
				end;
			end;
			updateBar(resptr[menu_GSD],width_menuTop,opt,0,color_selected);
			screen2video();
		end;
	until false;
	move(@tmpbuf,@screen[20],200);
end;

procedure GSDModule();
begin
	GSDScreen();
	GSDLoop();
end;
