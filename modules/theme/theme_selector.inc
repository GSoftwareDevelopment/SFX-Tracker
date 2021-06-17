{$i modules/theme/theme_view.inc}
{$i modules/theme/theme_manage.inc}
{$i modules/theme/theme_options.inc}

procedure theme_selector();
var
	nTheme:byte;

begin
	nTheme:=currentTheme;
	ThemesScreen();

	colorHLine(6,2+nTheme,14,3);
	screen2video();

	repeat
		if keyPressed then
		begin
			if nTheme<6 then
				colorHLine(6,2+nTheme,14,1)
			else
				colorHLine(0,2+nTheme,20,1);
			repeat
				controlSelectionKeys(key,key_Up,key_Down,nTheme,0,8);
			until nTheme<>6;
			case key of
				key_ESC: break;
				key_RETURN:
					if nTheme<6 then
						getTheme(nTheme,PFCOLS)
					else
					begin
						exitThemeSelector();
						ThemeMenuOption(nTheme-7);
						ThemesScreen();
					end;
			end;
			if nTheme<6 then
				colorHLine(6,2+nTheme,14,3)
			else
				colorHLine(0,2+nTheme,20,3);
			screen2video();
		end;
	until false;

	exitThemeSelector();
	move(@tmpbuf[20],@screen[20],200);
end;
