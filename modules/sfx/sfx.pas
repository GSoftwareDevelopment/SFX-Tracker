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
{$i modules/sfx/sfx_options.inc}

procedure SFXLoop();
var
	nSFX,opt:shortint;

begin
	kbcode:=255; section:=0;
	getSFXData(currentSFX);
	SFXDetermineLength();
	updateSFXView();

	updateBar(resptr[menu_sfx],width_menuBar,section,color_choice,color_selected);
	screen2video();
	modified:=false;
	repeat
		updateModified();
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			controlSelectionKeys(key,key_Up,key_Down,section,0,7);
			case key of
				key_Left,key_Right:
					if (section=0) then
					begin
						controlSelectionKeys(key,key_Left,Key_Right,currentSFX,0,maxSFXs-1);
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
							if (nSFX<>currentSFX) then
							begin
								getSFXData(nSFX);
								SFXDetermineLength();
								updateSFXView();
								modified:=false;
							end;
						end;
						1: section:=3;
						7: SFX_Options(5-byte(modified));
					end;

					if (section>=2) and (section<=5) then
					begin
						colorVLine(winXStart+cursorPos,winYStart,8,3);
						repeat
							section:=section and $f;
							updateBar(resptr[menu_sfx],width_menuBar,section,color_choice,color_selected);
							case section of
								3: SFXEditLoop(SFX_vol_dist,0,modified);
								4: SFXEditLoop(SFX_vol_dist,1,modified);
								5: SFXEditLoop(SFX_modulate,1,modified);
								6: SFXEditLoop(SFX_modulate,0,modified);
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

procedure SFXModule();
begin
	SFXScreen();
	SFXLoop();
end;
