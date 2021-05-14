var
	SFX_vol_dist:array[0..SFX_maxLength-1] of byte;
	SFX_modulate:array[0..SFX_maxLength-1] of byte;

	sfxLen:byte;
	SFXName:string[SFXNameLength];

//
//
//

{$i modules/sfx/sfx_view.inc}
{$i modules/sfx/sfx_manage.inc}
{$i modules/sfx/sfx_edit.inc}


procedure SFXLoop();
var
	nSFX,opt:shortint;
	modified,modState:boolean;
	modTm:longint;

begin
	kbcode:=255; section:=0;
	getSFXData(currentSFX);
	SFXDetermineLength();
	updateSFXView();

	updateBar(resptr[menu_sfx],width_menuBar,section,color_choice,color_selected);
	screen2video();
	modified:=false; modState:=false;
	repeat
		if (getTime-modTm>25) then
		begin
			if (modified) then
			begin
				modState:=not modState;
				if modState then
					screen[39]:=$49
				else
					screen[39]:=$41;
			end
			else
				screen[393]:=$40;
			screen2video();
			modTm:=getTime;
		end;
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			case key of
				key_TAB,key_Down: begin
					section:=section+1; if section>6 then section:=0;
				end;
				key_SHIFT_TAB,key_Up: begin
					section:=section-1; if section<0 then section:=6;
				end;
				key_Left,key_Right: if (section=0) then
				begin
					case key of
						key_Right: if (currentSFX<maxSFXs-1) then currentSFX:=currentSFX+1;
						key_Left: if (currentSFX>0) then currentSFX:=currentSFX-1;
					end;
					getSFXData(currentSFX);
					updateSFXView();
					modified:=false;
				end;
				key_ESC: break;
				key_RETURN: begin
					case section of
						0: begin
							move(@screen[20],@tmpbuf,200);
							box(0,2,20,9,$40);
							prepareSFXsList();
							nSFX:=listChoice(1,2,SFXNameLength,9,currentSFX,listBuf,maxSFXs,true);
							move(@tmpbuf,@screen[20],200);
//							colorHLine(0,1,20,0);
//							nSFX:=instrListChoice;
							if (nSFX<>currentSFX) then
							begin
								getSFXData(nSFX);
								SFXDetermineLength();
								updateSFXView();
								modified:=false;
							end;
						end;
						1: section:=2;
						6: begin
								opt:=4;
								optionsList(resptr[menu_sfx_options],width_menuOptions,5,opt);
								case opt of
									4: begin
											SFXDetermineLength();
											if (sfxLen>0) then
											begin
												if (inputText(SFXNameX,SFXNumberY,SFXNameLength,SFXName,0,color_choice)) then
												begin
													storeSFXData(currentSFX);
													modified:=false;
												end;
											end;
									end;
								end;
							end;
					end;

					if (section>=2) and (section<=5) then
					begin
						colorVLine(winXStart+cursorPos,winYStart,8,3);
						repeat
							section:=section and $f;
							updateBar(resptr[menu_sfx],width_menuBar,section,color_choice,color_selected);
							case section of
								2: SFXEditLoop(SFX_vol_dist,0,modified);
								3: SFXEditLoop(SFX_vol_dist,1,modified);
								4: SFXEditLoop(SFX_modulate,1,modified);
								5: SFXEditLoop(SFX_modulate,0,modified);
							end;
						until section and $10=0;
						colorVLine(winXStart+cursorPos,winYStart,8,0);
					end;

					SFXDetermineLength();
					updateSFXView();
				end;
			end;
			updateBar(resptr[menu_sfx],width_menuBar,section,color_choice,color_selected);
			screen2video();
		end;
	until false;
	updateBar(resptr[menu_sfx],4,-1,0,0);
end;


procedure SFXScreen();
begin
	section:=0; cursorPos:=0; cursorShift:=0;
	fillchar(@screen[20],180,$00);
	VBar(0,1,width_menuBar,0);
	colorHLine(SFXNumberX-1,SFXNumberY,4,1);
	menuBar(resptr[menu_sfx],width_menuBar,1);
	SFXLoop();
end;
