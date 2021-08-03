var
   SFX_vol_dist:array[0..SFX_maxLength-1] of byte absolute EDIT_BUF1_ADDR;
   SFX_modulate:array[0..SFX_maxLength-1] of byte absolute EDIT_BUF2_ADDR;

   sfxLen:byte;
   SFXName:string[SFXNameLength];

//
//
//

{$i modules/sfx/sfx_manage.inc}
{$i modules/sfx/sfx_view.inc}
{$i modules/sfx/sfx_options.inc}
{$i modules/sfx/sfx_edit.inc}
{$i modules/sfx/sfx_menubar.inc}

procedure SFXLoop();
var
	i:byte;

   procedure update();
   begin
		menuBar(menu_sfx,width_menuBar,section,color_choice,color_selected);
		screen2video();
   end;

begin
   getSFXData(currentSFX);
   SFXDetermineLength();
   updateSFXView();

   update();
   modified:=false;
   repeat
      if keyPressed then
      begin
         controlSelectionKeys(key_Up,key_Down,section,0,7);

			i:=controlSFXShortcutKeys;
			if i<>255 then
			begin
				setShortcut2currentSFX(i);
// TIP: uncomment below lines to active status bar info about assigned fast key
//				updateSFXView();
//				SFXshortcutMessage(i);
			end;

         if section=0 then SFXMenuBarChange();

         case key of
            key_RETURN: SFXSelectMenuBar(section);
         end;

         update();
      end;
   until checkEscape;
end;

procedure SFXModule();
begin
   section:=0; cursorPos:=0; cursorShift:=0;
   SFXScreen();
   SFXLoop();
end;
