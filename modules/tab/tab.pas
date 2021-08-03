var
   TAB_notes:array[0..TAB_maxLength-1] of byte absolute EDIT_BUF1_ADDR;
   TAB_fnSFX:array[0..TAB_maxLength-1] of byte absolute EDIT_BUF2_ADDR;
   TAB_RealPos:array[0..TAB_maxLength-1] of word absolute EDIT_BUF3_ADDR; // this table is used only in TAB view and its build by 'determineTABLen' procedure

   TABLen:byte;
   TABRealLen:word;

   TABName:string[TABNameLength];

{$i modules/tab/tab_manage.inc}
{$i modules/tab/tab_view.inc}
{$i modules/tab/tab_play.inc}
{$i modules/tab/tab_options.inc}
{$i modules/tab/tab_edit.inc}
{$i modules/tab/tab_menubar.inc}

procedure TABLoop();
begin
   repeat
      if keyPressed then
      begin
         controlSelectionKeys(key_Up,key_Down,section,0,3);
         case key of
            key_Left,key_Right: TABChangeMenuBarOpt(section);
            key_RETURN: TABSelectMenuBar(section);
         end;
         updateTABBar();
         screen2video();
      end;
   until checkEscape;
end;

procedure TABModule();
begin
   section:=0; cursorPos:=0; cursorShift:=0; modified:=false;
   TABScreen();
	screen2video();
   TABLoop();
end;
