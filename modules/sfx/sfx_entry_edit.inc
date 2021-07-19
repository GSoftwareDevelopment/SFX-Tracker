function getMODValue(var v:smallint; min,max:smallint; pos,fnCode,minusBase:byte):boolean;
var
   a:byte;

begin
   putMultiText(msg_SFX_ValuePrompt,0);
   putValue(13,11,abs(min),3,0);
   putValue(17,11,max,3,0);
   if (min<0) then screen[232]:=$0d else screen[232]:=$00;
   if inputValue(4,11,4,v,min,max,0,0) then
   begin
      if v<0 then
         a:=minusBase-abs(v)
      else
         a:=v;
      SFX_Modulate[pos]:=a or fnCode;
      modified:=true;
   end;
   fillchar(@screen[220],20,0);
end;

function SFXEntryEdit():boolean;
var
   _pos,_mod,_SFXMode:byte;
   opt:byte;
   v:smallint;

begin
   _pos:=cursorShift+cursorPos;
   _mod:=SFX_modulate[_pos];
   _SFXMode:=SFXModMode[currentSFX];

   case _SFXMode of
      0: begin // High Frequency Modulation
(*
* _mod      opt
* %00000000  2  - no modulation
* %0xxxxxxx  1  - incrace freq. value by x
* %1xxxxxxx  1  - decrace freq. value by x
* %10000000  0  - end of SFX definition
*)

            opt:=1;
            if optionsList(menu_HFD_mode,width_menuSFXFunc,5,opt,key_Up,key_Down) then
               case opt of
                  0: SFX_modulate[_pos]:=$80;
                  1: getMODValue(v,-127,127,_pos,$00,$00);
                  2: SFX_modulate[_pos]:=$00;
                  3: exit(false);
                  4: exit(true);
               end;
         end;
      1: begin // Middle Frequency Modulation
(*
* _mod      opt
* %00000000  3  - no modulation
* %00xxxxxx  2  - incrase freq. value by x
* %01xxxxxx  2  - decrase freq. value by x
* %1xxxxxxx  1  - jump to position x in SFX definition
* %10000000  0  - end of sfx definition
*)
            opt:=2;
            if optionsList(menu_MFD_mode,width_menuSFXFunc,6,opt,key_Up,key_Down) then
               case opt of
                  0: SFX_modulate[_pos]:=$80;
                  1: getMODValue(v, 0,127,_pos,$80,$00);
                  2: getMODValue(v,-63,63,_pos,$00,$80);
                  3: SFX_modulate[_pos]:=$00;
                  4: exit(false);
                  5: exit(true);
               end;
         end;
      2: begin // Low Frequency Modulation/Note Level Modulation
(*
* _mod      opt
* %00000000  4  - no modulation
* %000xxxxx  3  - incrase freq. value by x
* %001xxxxx  3  - decrase freq. value by x
* %010xxxxx  2  - incrase note by x halftones
* %011xxxxx  2  - decrase note by x halftones
* %1xxxxxxx  1  - jump to position x in SFX definition
* %10000000  0  - end of sfx definition
*)
            opt:=3;
            if optionsList(menu_LFD_NLM_mode,width_menuSFXFunc,7,opt,key_Up,key_Down) then
               case opt of
                  0: SFX_modulate[_pos]:=$80;
                  1: getMODValue(v, 0,127,_pos,$80,$00);
                  2: getMODValue(v,-31,31,_pos,$00,$40);
                  3: getMODValue(v,-31,31,_pos,$40,$40);
                  4: SFX_modulate[_pos]:=$00;
                  5: exit(false);
                  6: exit(true);
               end;
         end;
      3: begin // Direct Set Divider
(*
* %xxxxxxxx - direct value to freq. divider register
*)
            opt:=1;
            if optionsList(menu_DFD_mode,width_menuSFXFunc,3,opt,key_Up,key_Down) then
               case opt of
                  0:getMODValue(v,0,255,_pos,$00,$00);
                  1:exit(false);
                  2:exit(true);
               end;
         end;
   end;
   result:=false;
end;
