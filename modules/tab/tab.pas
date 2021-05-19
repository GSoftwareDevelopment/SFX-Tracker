var
	TAB_notes:array[0..TAB_maxLength-1] of byte;
	TAB_fnSFX:array[0..TAB_maxLength-1] of byte;

	TABLen:byte;
	TABName:string[TABNameLength];

{$i modules/tab/tab_view.inc}
{$i modules/tab/tab_manage.inc}
{$i modules/tab/tab_edit.inc}
{$i modules/tab/tab_options.inc}

procedure TABLoop();
var
	opt,nTab:shortint;

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
								controlSelectionKeys(key,key_Left,key_Right,currentTAB,0,maxTABs);
								modified:=false;
								getTABData(currentTAB);
								TABDetermineLength();
								updateTABInfo();
								updateTAB(true);
							end;
						4: begin
							controlSelectionKeys(key,key_Left,key_Right,currentSFX,0,maxSFXs);
							updateTABSFX();
							end;
					end;
				end;
				key_RETURN: begin
					case section of
						0: begin
								move(@screen[20],@tmpbuf,200);
								box(0,2,20,9,$40);
								prepareTABsList();
								nTab:=listChoice(4,2,TABNameLength,9,currentTAB,listBuf,maxTABs,true);
								move(@tmpbuf,@screen[20],200);
								if (nTab<>currentTAB) then
								begin
									getTABData(nTab);
									TABDetermineLength();
									updateTAB(true);
									updateTABInfo();
									modified:=false;
								end;
							end;
						1: TABEditLoop();
						3: TAB_Options(4-byte(modified));
						4: begin
								move(@screen[20],@tmpbuf,200);
								box(0,2,20,9,$40);
								prepareSFXsList();
								currentSFX:=listChoice(1,2,SFXNameLength,9,currentSFX,listBuf,maxSFXs,true);
								move(@tmpbuf,@screen[20],200);
								updateTABSFX();
							end;
					end;
					if (section and $10=$10) then // test flag for pressed TAB in TABEditLoop...
					begin
						section:=section and $0f; // ...cleaning flag
						continue;
					end;
				end;
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
