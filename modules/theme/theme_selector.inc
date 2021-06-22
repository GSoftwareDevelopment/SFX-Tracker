{$i modules/theme/theme_view.inc}
{$i modules/theme/theme_manage.inc}
{$i modules/theme/theme_options.inc}

procedure theme_selector();
var
	nTheme:shortint;

begin
	ThemesScreen(); DLIInit();

	repeat
		nTheme:=listChoice(7,2,12,8,currentTheme,themesNames,7,false);
		if nTheme=-1 then break;
		if nTheme<6 then
			getTheme(nTheme,PFCOLS)
		else
			ThemeMenuOption(nTheme-6);
	until false;

	exitThemeSelector(); DLIDone();
	clearModule();
end;
