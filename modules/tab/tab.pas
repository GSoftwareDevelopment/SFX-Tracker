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
	section:=0; cursorPos:=0; cursorShift:=0; modified:=false;

	getTABData(currentTAB);
	TABDetermineLength();
	updateTABInfo();
	updateTABSFX();
	updateTAB(true);

	updateBar(resptr[menu_tabs],width_menuBar,section,color_choice,color_selected);

	screen2video();
	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode);
			controlSelectionKeys(key,key_Up,key_Down,section,0,4);
			case key of
				key_ESC: break;
				key_Left,key_Right: TABChangeMenuBarOpt(section);
				key_RETURN: TABSelectMenuBar(section);
			end;
			updateBar(resptr[menu_tabs],width_menuBar,section,color_choice,color_selected);
			screen2video();
			kbcode:=255;
		end;
	until false;
	kbcode:=255;
	updateBar(resptr[menu_tabs],width_menuBar,-1,0,0);
end;

procedure TABModule();
begin
	TABScreen();
	TABLoop();
end;
