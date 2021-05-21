var
	TAB_notes:array[0..TAB_maxLength-1] of byte;
	TAB_fnSFX:array[0..TAB_maxLength-1] of byte;

	TABLen:byte;
	TABName:string[TABNameLength];

{$i modules/tab/tab_view.inc}
{$i modules/tab/tab_manage.inc}
{$i modules/tab/tab_edit.inc}
{$i modules/tab/tab_options.inc}
{$i modules/tab/tab_menubar.inc}

procedure TABLoop();
var
	opt,nTAB:shortint;

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
		updateModified();
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode);
			controlSelectionKeys(key,key_Up,key_Down,section,0,4);
			case key of
				key_ESC: break;
				key_Left,key_Right: begin
					case section of
						0: begin
								controlSelectionKeys(key,key_Left,key_Right,currentTAB,0,maxTABs-1);
								modified:=false;
								getTABData(currentTAB);
								TABDetermineLength();
								updateTABInfo();
								updateTAB(true);
							end;
						4: begin
								controlSelectionKeys(key,key_Left,key_Right,currentSFX,0,maxSFXs-1);
								updateTABSFX();
							end;
					end;
				end;
				key_RETURN: TABMenuBar(section);
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
