function SONGEntryEdit(SONGOfs:byte):boolean;
var
   opt:byte;

   procedure setTempo();
   begin
      SONGData[SONGOfs]:=chnOrder_Tempo;
      SONGData[SONGOfs+1]:=SONG_Tempo;
      SONGChn:=1;
   end;

   procedure setJumpTo();
   begin
      SONGData[SONGOfs]:=chnOrder_JumpTo;
      SONGData[SONGOfs+1]:=0;
      SONGChn:=1;
   end;

   procedure setRepeat();
   begin
      SONGData[SONGOfs]:=chnOrder_Repeat;
      SONGData[SONGOfs+1]:=0;
      SONGData[SONGOfs+2]:=0;
      SONGChn:=1;
   end;

begin
   opt:=SONGEditOption_BackToEdit;
   if optionsList(menu_song_edit,width_menuSONGFunc,SONGEditOptions,opt,key_Up,key_Down) then
   begin
      if opt>=SONGEditOption_BackToEdit then exit(opt=SONGEditOption_BackToMenuBar);

      if (SONGData[SONGOfs] and chnOrder<>0) then
         fillchar(@SONGData[SONGOfs],4,chnBlank);

      case opt of
         0: SONGData[SONGOfs]:=chnOrder_EndSong;
         4: SONGData[SONGOfs+SONGChn]:=chnOff;
         5: SONGData[SONGOfs+SONGChn]:=chnBlank;
      end;

      clearWorkarea();
      updateSONG(false);

      case opt of
         1: setTempo();
         2: setJumpTo();
         3: setRepeat();
      end;

      clearStatusBar();
      result:=false;
   end
   else
      updateSONG(false);
end;
