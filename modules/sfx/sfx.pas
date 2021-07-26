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
{$i modules/sfx/sfx_edit.inc}
{$i modules/sfx/sfx_options.inc}
{$i modules/sfx/sfx_menubar.inc}

procedure SFXLoop();
   procedure update();
   begin
      updateBar(menu_sfx,width_menuBar,section,color_choice,color_selected);
   end;

begin
   getSFXData(currentSFX);
   SFXDetermineLength();
   updateSFXView();

   update();
   screen2video();
   modified:=false;
   repeat
      if keyPressed then
      begin
         controlSelectionKeys(key,key_Up,key_Down,section,0,7);
         if section=0 then SFXMenuBarChange();
         case key of
            key_ESC: break;
            key_RETURN: SFXSelectMenuBar(section);
         end;
         update();
         screen2video();
      end;
   until false;
end;

procedure SFXModule();
begin
   section:=0; cursorPos:=0; cursorShift:=0;
   SFXScreen();
   SFXLoop();
end;
