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
   newTempo:=SONG_Tempo;
   if inputValue(11,9,3,newTempo,0,255,0,0) then
      SONG_Tempo:=byte(newTempo);
end;

procedure SONG_Options(defaultOpt:byte);
begin
   color:=0; strVal2Mem(resptr[val_menu_tempo],SONG_Tempo,3);
   if optionsList(menu_SONG_options,width_menuOptions,SONGMenuOptions,defaultOpt,key_Up,key_Down) then
      case defaultOpt of
         0: setSONGName();
         1: setSONGTempo();
      end;
   updateSONG(true);
end;
