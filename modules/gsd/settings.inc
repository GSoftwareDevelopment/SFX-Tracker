procedure SFXMM_Settings();
var
	opt:byte;

begin
	setWorkarea(); opt:=0;
	repeat
		if optionsList(menu_settings,width_menuTop,4,opt,key_Up,key_Down) then
		begin
			case opt of
				3:break;
			end;
		end;
	until false;
//	putMultiText(menu_settings,0);
   showAppSplash();

end;
