{$i modules/gsd_options/dli_interrupt.inc}
{$i modules/gsd_options/credits.inc}
{$i modules/gsd_options/theme_selector.inc}
{$i modules/gsd_options/memory_stats.inc}

procedure updateFreeMem();
var
	s,o:string[3];
	free:word;
	submenu:array[0..0] of byte;

begin
	submenu:=resptr[menu_GSD];
	free:=trunc((HEAP_FreeMem/HEAP_MEMORY_SIZE)*100);
	str(free,s); o:=concat(StringOfChar('0',3-length(s)),s);
	conv2Internal(o);
	move(@o[1],@submenu[17],3);
end;

procedure GSDOptions();
var
	opt:shortint;

begin
	updateFreeMem();
	move(@screen[20],@tmpbuf,200);
	fillchar(@screen[20],20,$00);
	menuBar(resptr[menu_GSD],menu_Top,0);
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
