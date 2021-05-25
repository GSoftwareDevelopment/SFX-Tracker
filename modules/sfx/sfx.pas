var
	SFX_vol_dist:array[0..SFX_maxLength-1] of byte absolute EDIT_BUF1_ADDR;
	SFX_modulate:array[0..SFX_maxLength-1] of byte absolute EDIT_BUF2_ADDR;

	sfxLen:byte;
	SFXName:string[SFXNameLength];

//
//
//

{$i modules/sfx/sfx_view.inc}
{$i modules/sfx/sfx_manage.inc}
{$i modules/sfx/sfx_edit.inc}
{$i modules/sfx/sfx_options.inc}
{$i modules/sfx/sfx_menubar.inc}

procedure SFXLoop();
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
				key_Left,key_Right: SFXChangeMenuBarOpt(section);
				key_ESC: break;
				key_RETURN: SFXSelectMenuBar(section);
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
