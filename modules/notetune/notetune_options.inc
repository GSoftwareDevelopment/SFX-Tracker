{$i modules/notetune/notetune_io.inc}

procedure NoteTune_setName();
begin
   FName[0]:=char(NOTETABnameLength);
   move(pointer(word(resptr[menu_NoteDef])+4+noteSetId*16),@FName[1],NOTETABnameLength);
   conv2ASCII(FName);
   if inputText(4,1,11,FName,1,1) then
   begin
      conv2Internal(FName);
      move(@FName[1],pointer(word(resptr[menu_NoteDef])+4+noteSetId*16),NOTETABnameLength);
   end;
end;

procedure NoteTune_sets();
begin
   noteSetId:=currentNoteTableOfs shr 6;
   if optionsList(menu_NoteDef,width_menuOptions,NOTEDEFMenu,noteSetId,key_Up,key_Down) then
      currentNoteTableOfs:=NOTE_TABLE_ADDR+noteSetId*64
   else
      currentNoteTableOfs:=SFXNoteSetOfs[currentSFX];
   SFXNoteSetOfs[currentSFX]:=currentNoteTableOfs;
   noteSetId:=currentNoteTableOfs shr 6;
end;

procedure NoteTune_Options(defaultOpt:byte);
begin
   if optionsList(menu_tune_options,width_menuOptions,TUNEMenuOptions,defaultOpt,key_Up,key_Down) then
      case defaultOpt of
         0:NoteTune_setName();
         2:NoteTune_IO_Options();
      end;

   currentNoteTableOfs:=noteSetId*64;
   NoteTuneRedraw();
end;
