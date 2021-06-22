function IO_Options():shortint;
var
	IOOpt:byte;

begin
	IOOpt:=0;
	if optionsList(menu_IO_options,width_menuIOOptions,IOOptions,IOOpt,key_Up,key_Down) then
		result:=IOOpt
	else
		result:=-1;
end;
