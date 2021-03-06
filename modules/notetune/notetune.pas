var
   noteSetId,
   currentNoteTableOfs,
   oldSFXNoteTable:byte;
   definedNoteTune:array[0..0] of byte;

{$i modules/notetune/notetune_view.inc}
{$i modules/notetune/notetune_edit.inc}
{$i modules/notetune/notetune_options.inc}

procedure setNoteTune();
var
   opt:byte;

begin
   currentNoteTableOfs:=SFXNoteSetOfs[currentSFX];
   oldSFXNoteTable:=currentNoteTableOfs;
   noteSetId:=currentNoteTableOfs shr 6;
   opt:=0;
   set2General_Charset();
   repeat
      NoteTuneRedraw();
      if optionsList(menu_note_tune,width_menuBar,TUNEMenu,opt,key_Up,key_Down) then
      begin
         case opt of
            0: NoteTune_sets();
            1: NoteTuneLoop();
            2: NoteTune_options(TUNEOptions_BackToEdit);
         end;
      end
      else
         break;
   until false;
	set2SFX_Charset();
   SFXNoteSetOfs[currentSFX]:=oldSFXNoteTable;
end;
