procedure setTABName();
begin
   if (inputText(SFXNameX,SFXNumberY,TABNameLength,TABName,0,color_choice)) then
   begin
		TABDetermineLength();
      storeTABData(currentTAB);
      modified:=false;
   end;
end;

procedure setBeatStep();
var
   newBeat:smallint;

begin
   putMultiText(msg_BeatStepPrompt,0);
   newBeat:=song_beat;
   if inputValue(15,9,2,newBeat,1,32,0,0) then
      song_beat:=byte(newBeat);
   clearStatusBar();
end;

procedure TAB_Options(defaultOpt:byte);
begin
   color:=0; strVal2Mem(resptr[val_menu_beatStep],song_beat,2);
   if optionsList(menu_tabs_options,width_menuOptions,TABMenuOptions,defaultOpt,key_Up,key_Down) then
      case defaultOpt of
         0: setTABName();
         1: setBeatStep();
      end;
   updateTAB(true);
end;
