{$i modules/theme/theme_io.inc}

procedure ThemeMenuOption(opt:byte);
begin
	exitThemeSelector(); DLIDone();

	case opt of
		0: begin
				clearModule(); // box(0,1,20,11,$40);
				if messageBox(msg_Theme_Overwrite,color_choice,but_Theme_overwrite,12,2,1,key_Up,key_Down)=0 then
				begin
					IOSaveTheme(defaultThemeFile);
				end;
			end;
		1: begin
				Theme_IO_Load();
				if IOErr=IO_OK then
					getTheme(currentTheme,PFCOLS);
			end;
	end;

	ThemesScreen(); DLIInit();
end;
