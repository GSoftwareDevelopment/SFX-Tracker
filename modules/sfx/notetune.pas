{$i modules/sfx/notetune_view.inc}
{$i modules/sfx/notetune_edit.inc}

procedure setNoteTune();
var
	opt:byte;

begin
	NoteTuneScreen();
	updateNoteTune(currentOct*12);
	opt:=0;
	repeat
		if optionsList(resptr[menu_note_tune],width_menuBar,TUNEMenu,opt,key_Up,key_Down) then
		begin
			case opt of
				0: NoteTuneLoop();
			end;
		end
		else
			break;
	until false;
	move(@tmpbuf,@screen,240);
end;
