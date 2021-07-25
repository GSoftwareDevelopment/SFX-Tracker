{$i modules/gsd/gsd_view.inc}
{$i modules/theme/theme_selector.inc}
{$i modules/gsd/settings.inc}
{$i modules/gsd/memory_stats.inc}

procedure GSDLoop();
begin
   section:=1;
   repeat
      if optionsList(menu_GSD,width_menuTop,4,section,key_Left,key_Right) then
         case section of
            0: break;
            1: SFXMM_Settings();
            2: theme_selector();
            3: memory_stats();
         end
      else
         break;
   until false;
end;

procedure GSDModule();
begin
   GSDScreen();
   GSDLoop();
end;
