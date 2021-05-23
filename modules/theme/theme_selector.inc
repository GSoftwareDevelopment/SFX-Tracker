var
	themesDef:array[0..0] of byte;
	currentTheme:byte;

{$i modules/theme/theme_view.inc}
{$i modules/theme/theme_manage.inc}
{$i modules/theme/theme_edit.inc}
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
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			if nTheme<6 then
				colorHLine(6,2+nTheme,14,1)
			else
				colorHLine(0,2+nTheme,20,1);
			controlSelectionKeys(key,key_Up,key_Down,nTheme,0,8);
			case key of
				key_ESC: break;
				key_RETURN:
					if nTheme<6 then
						setTheme(nTheme)
					else
					begin
						exitThemeSelector();
						ThemeMenuOption(nTheme-6);
						initThemeSelector();
					end;
				key_SPACE: begin
					editTheme(nTheme);
					exitThemeSelector();
					initThemeSelector();
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

procedure initThemes(themesPtr:pointer);
begin
	themesDef:=themesPtr;
	setTheme(0);
end;
