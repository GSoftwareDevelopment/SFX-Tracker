procedure TABList();
var
   nTAB:shortint;

begin
   nTAB:=dataList(TABPtr,currentTAB,maxTABs,TABNameLength);
   if (nTAB=-1) then nTAB:=currentTAB;

	getTABData(nTAB);
	TABDetermineLength();
//   modified:=false;
end;

procedure TABChangeMenuBarOpt(section:byte);
begin
   case section of
      0: begin
            controlSelectionKeys(key_Left,key_Right,currentTAB,0,maxTABs-1);
            TABScreen();
         end;
   end;
end;

procedure TABSelectMenuBar(section:byte);
begin
   case section of
      0: TABList();
      1: TABEditLoop();
      2: TAB_Play(0);
      3: TAB_Options(TABMenuOption_Back);
   end;
	TABScreen();
end;
