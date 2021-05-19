{$i modules/gsd/dli_interrupt.inc}
{$i modules/gsd/gsd_view.inc}
{$i modules/gsd/credits.inc}
{$i modules/theme/theme_selector.inc}
{$i modules/gsd/memory_stats.inc}


procedure GSDLoop();
var
	opt:byte;

begin
	opt:=1;
	updateBar(resptr[menu_GSD],width_menuTop,opt,0,color_selected);
	screen2video();
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			controlSelectionKeys(key,key_Left,key_Right,opt,0,2);
			case key of
				key_ESC: break;
				key_RETURN: case opt of
					0: break;
					1: credits();
					2: theme_selector();
					3: memory_stats();
				end;
			end;
			updateBar(resptr[menu_GSD],width_menuTop,opt,0,color_selected);
			screen2video();
		end;
	until false;
	move(@tmpbuf,@screen,240);
end;

procedure GSDModule();
begin
	GSDScreen();
	GSDLoop();
end;
