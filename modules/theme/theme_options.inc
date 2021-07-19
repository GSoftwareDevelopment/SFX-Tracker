{$i modules/theme/theme_io.inc}

procedure ThemeMenuOption(opt:byte);
begin
   exitThemeSelector(); DLIDone();

   case opt of
      0: begin
            fillchar(@moduleBar,200,$40);
            if msgYesNo(msg_Theme_Overwrite)=1 then IOSaveTheme(defaultThemeFile);
         end;
      1: begin
            Theme_IO_Load();
            if IOErr=IO_OK then
               getTheme(currentTheme,PFCOLS);
         end;
   end;

   ThemesScreen(); DLIInit();
end;
