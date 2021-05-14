var
	TAB_notes:array[0..TAB_maxLength-1] of byte;
	TAB_fnSFX:array[0..TAB_maxLength-1] of byte;

	TABLen:byte;
	TABName:string[TABNameLength];

{$i modules/tab/tabs_view.inc}
{$i modules/tab/tabs_manage.inc}
{$i modules/tab/tabs_edit.inc}

procedure TABLoop();
var
 opt,nTab:shortint;
 modified:boolean;

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
			case key of
				key_ESC: break;
				key_Up,key_SHIFT_TAB: if (section>0) then section:=section-1 else section:=4;
				key_Down,key_TAB: if (section<4) then section:=section+1 else section:=0;
				key_Left,key_Right: begin
					case section of
						0: begin
							case key of
								key_Left: if (currentTAB>0) then currentTAB:=currentTAB-1;
								key_Right: if (currentTAB<maxTABs) then currentTAB:=currentTAB+1;
							end;
							modified:=false;
							getTABData(currentTAB);
							TABDetermineLength();
							updateTABInfo();
							updateTAB(true);
						end;
						4: begin
							case key of
								key_Left: if (currentSFX>0) then currentSFX:=currentSFX-1;
								key_Right: if (currentSFX<maxSFXs-1) then currentSFX:=currentSFX+1;
							end;
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
						3: begin
								opt:=3;
								optionsList(resptr[menu_tabs_options],width_menuOptions,4,opt);
								case opt of
									3: begin
											TABDetermineLength();
											if (TABLen>0) then
											begin
												if (inputText(SFXNameX,SFXNumberY,TABNameLength,TABName,0,color_choice)) then
												begin
													storeTABData(currentTAB);
													modified:=false;
												end;
											end;
										end;
								end;
								box(4,2,16,9,0);
								updateTAB(true);
						end;
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

procedure TABScreen();
begin
	fillchar(@screen[20],200,$00);
	VBar(0,1,width_menuBar,0);
	screen[223]:=$06;
	screen[32]:=$07;

	fillchar(@TAB_notes,256,255);
	fillchar(@TAB_fnSFX,256,254);

	menuBar(resptr[menu_tabs],width_menuBar,1);

	TABLoop();
end;
