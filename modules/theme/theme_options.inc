{$i modules/theme/theme_io.inc}

procedure ThemeMenuOption(opt:byte);
var
	IORes:Byte;

begin
	box(0,1,20,11,$40);
	case opt of
		0: begin
				if messageBox(resptr[msg_Theme_Overwrite],color_choice,resptr[but_Theme_overwrite],12,2,1,key_Up,key_Down)=0 then
				begin
					IORes:=IOSaveTheme(defaultThemeFile);
					if IORes<>IO_OK then IOError(IORes);
				end;
			end;
		1: Theme_IO_Load();
	end;
end;
