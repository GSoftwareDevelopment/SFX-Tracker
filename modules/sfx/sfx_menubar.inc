procedure SFXList();
var
   nSFX:shortint;

begin
   nSFX:=dataList(SFXPtr,currentSFX,maxSFXs,SFXNameLength);
   if (nSFX=-1) then nSFX:=currentSFX;
	getSFXData(nSFX);
	SFXScreen();
end;

procedure SFXMenuBarChange();
begin
   if controlSelectionKeys(key,key_Left,Key_Right,currentSFX,0,maxSFXs-1) then
   begin
      getSFXData(currentSFX);
      SFXDetermineLength();
      updateSFXView();
      modified:=false;
   end;
end;

{$i modules/sfx/sfx_piano.inc}

procedure SFXSelectMenuBar(var section:byte);
begin
   case section of
      0: SFXList();
      1: section:=3;
      2: SFX_Piano();
      7: SFX_Options(SFXMenuOption_Back);
   end;

   if (section>=3) and (section<=6) then
   begin
      repeat
         section:=section and $f;
			menuBar(menu_sfx,width_menuBar,section,color_choice,color_selected);
         case section of
            3: SFXEditLoop(SFX_vol_dist,0);
            4: SFXEditLoop(SFX_vol_dist,1);
            5: SFXEditLoop(SFX_modulate,1);
            6: SFXEditLoop(SFX_modulate,0);
         end;
      until section and $10=0;
   end;

   if modified then
   begin
      // store SFX set after edit leave, if its modified
      storeSFXData(currentSFX);
      modified:=false;
   end;

   SFXDetermineLength();
   updateSFXView();
end;
