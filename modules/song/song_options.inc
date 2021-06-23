procedure setSONGName();
begin
	if inputLongText(0,1,20,SONGNameLength,SONGTitle,0,0) then
	begin
		modified:=false;
	end;
end;

procedure setSONGTempo();
var
	newTempo:smallint;

begin
	putMultiText(msg_SetTempoPrompt,0);
	newTempo:=song_lpb;
	if inputValue(11,9,3,newTempo,0,255,0,0) then
		song_lpb:=byte(newTempo);
end;

procedure SONG_Options(defaultOpt:byte);
begin
	strVal2Mem(resptr[val_menu_tempo],song_lpb,3,0);
	if optionsList(menu_SONG_options,width_menuOptions,SONGMenuOptions,defaultOpt,key_Up,key_Down) then
		case defaultOpt of
			0: setSONGName();
			1: setSONGTempo();
		end;
	updateSONG(true);
end;
