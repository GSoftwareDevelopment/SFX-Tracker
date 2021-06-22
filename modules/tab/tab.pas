var
	TAB_notes:array[0..TAB_maxLength-1] of byte absolute EDIT_BUF1_ADDR;
	TAB_fnSFX:array[0..TAB_maxLength-1] of byte absolute EDIT_BUF2_ADDR;

	TABLen:byte;
	TABName:string[TABNameLength];

{$i modules/tab/tab_view.inc}
{$i modules/tab/tab_manage.inc}
{$i modules/tab/tab_play.inc}
{$i modules/tab/tab_edit.inc}
{$i modules/tab/tab_options.inc}
{$i modules/tab/tab_menubar.inc}

procedure TABLoop();
begin
	getTABData(currentTAB);
	TABDetermineLength();
	updateTABInfo();
	updateTAB(true);

	updateBar(menu_tabs,width_menuBar,section,color_choice,color_selected);
	screen2video();
	modified:=false;
	repeat
		if keyPressed then
		begin
			controlSelectionKeys(key,key_Up,key_Down,section,0,3);
			case key of
				key_ESC: break;
				key_Left,key_Right: TABChangeMenuBarOpt(section);
				key_RETURN: TABSelectMenuBar(section);
			end;
			updateBar(menu_tabs,width_menuBar,section,color_choice,color_selected);
			screen2video();
		end;
	until false;
end;

procedure TABModule();
begin
	section:=0; cursorPos:=0; cursorShift:=0;
	TABScreen();
	TABLoop();
end;
