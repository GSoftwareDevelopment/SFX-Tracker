{$i modules/gsd/dli_interrupt.inc}
{$i modules/gsd/gsd_view.inc}
{$i modules/theme/theme_selector.inc}


procedure GSDLoop();
begin
	section:=1;
	repeat
		if optionsList(resptr[menu_GSD],width_menuTop,2,section,key_Left,key_Right) then
			case section of
				0: break;
				1: theme_selector();
			end
		else
			break;
	until false;
	move(@tmpbuf,@screen,240);
end;

procedure GSDModule();
begin
	GSDScreen();
	GSDLoop();
end;
