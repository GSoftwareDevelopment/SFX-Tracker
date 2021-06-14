var
	noteSetId:byte;
	definedNoteTune:array[0..0] of byte;

{$i modules/sfx/notetune_view.inc}
{$i modules/sfx/notetune_edit.inc}
{$i modules/sfx/notetune_options.inc}

procedure setNoteTune();
var
	opt:byte;

begin
	currentNoteTableOfs:=SFXNoteSetOfs[currentSFX];
	NoteTuneScreen();

	opt:=0;
	repeat
		NoteTuneRedraw();
		if optionsList(resptr[menu_note_tune],width_menuBar,TUNEMenu,opt,key_Up,key_Down) then
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
	currentNoteTableOfs:=$FF;
	SFXScreen();
end;
