var
	TAB_notes:array[0..TAB_maxLength-1] of byte absolute EDIT_BUF1_ADDR;
	TAB_fnSFX:array[0..TAB_maxLength-1] of byte absolute EDIT_BUF2_ADDR;
	TAB_RealPos:array[0..TAB_maxLength-1] of word absolute EDIT_BUF3_ADDR; // this table is used only in TAB view and its build by 'determineTABLen' procedure

	TABLen:byte;
	TABRealLen:word;

	TABName:string[TABNameLength];

{$i modules/tab/tab_view.inc}
{$i modules/tab/tab_manage.inc}
{$i modules/tab/tab_play.inc}
{$i modules/tab/tab_edit.inc}
{$i modules/tab/tab_options.inc}
{$i modules/tab/tab_menubar.inc}

procedure TABLoop();
	procedure update();
	begin
		updateBar(menu_tabs,width_menuBar,section,color_choice,color_selected);
	end;

begin
	getTABData(currentTAB);
	TABDetermineLength();
	updateTABInfo();
	updateTAB(true);
	update();
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
			update();
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
