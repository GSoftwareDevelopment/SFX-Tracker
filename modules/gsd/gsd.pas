{$i modules/gsd/dli_interrupt.inc}
{$i modules/gsd/gsd_view.inc}
{$i modules/theme/theme_selector.inc}


procedure GSDLoop();
var
	opt:shortint;

begin
	opt:=1;
	repeat
		if optionsList(resptr[menu_GSD],width_menuTop,2,opt,key_Left,key_Right) then
			case opt of
				0: break;
				1: theme_selector();
			end;
	until false;
	move(@tmpbuf,@screen,240);
end;

procedure GSDModule();
begin
	GSDScreen();
	GSDLoop();
end;
