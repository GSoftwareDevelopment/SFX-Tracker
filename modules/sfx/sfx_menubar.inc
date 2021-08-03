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
   if controlSelectionKeys(key_Left,Key_Right,currentSFX,0,maxSFXs-1) then
   begin
      getSFXData(currentSFX);
      SFXDetermineLength();
      updateSFXView();
      modified:=false;
   end;
end;

{$i modules/sfx/sfx_piano.inc}

procedure SFXSelectMenuBar();
begin
   case section of
      0: SFXList();
      1: begin section:=3; SFXEditLoop(); end;
      2: SFX_Piano();
      3..6: SFXEditLoop();
      7: SFX_Options(SFXMenuOption_Back);
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
