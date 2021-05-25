function getRangeValue(var v:smallint; min,max:smallint):boolean;
begin
	putMultiText(resptr[msg_SFX_ValuePrompt],0);
	putValue(13,11,abs(min),3,0);
	putValue(17,11,max,3,0);
	if (min<0) then screen[232]:=$0d else screen[232]:=$00;
	result:=inputValue(4,11,4,v,min,max,0,0);
	fillchar(@screen[220],20,0);
end;

function SFXEntryEdit():boolean;
var
	_pos,_mod,_SFXMode:byte;
	opt:shortint;
	v:smallint;

begin
	_pos:=cursorShift+cursorPos;
	_mod:=SFX_modulate[_pos];
	_SFXMode:=SFXModMode[currentSFX];

	case _SFXMode of
		0: begin // High Frequency Modulation
(*
* _mod
* %00000000 - no modulation
* %0xxxxxxx - incrace freq. value by x
* %1xxxxxxx - decrace freq. value by x
* %10000000 - end of SFX definition
*)

				opt:=1;
				optionsList(resptr[menu_HFD_mode],width_menuSFXFunc,5,opt,key_Up,key_Down);
				if opt=34then exit(true);
				case opt of
					0:SFX_modulate[_pos]:=$80;
					1:getRangeValue(v,-127,127);
					2:SFX_modulate[_pos]:=$00;
					3:exit(false);
					4:exit(true);
				end;
			end;
		1: begin // Middle Frequency Modulation
(*
* _mod
* %00000000 - no modulation
* %00xxxxxx - incrase freq. value by x
* %01xxxxxx - decrase freq. value by x
* %1xxxxxxx - jump to position x in SFX definition
* %10000000 - end of sfx definition
*)
				opt:=2;
				optionsList(resptr[menu_MFD_mode],width_menuSFXFunc,6,opt,key_Up,key_Down);
				case opt of
					0:SFX_modulate[_pos]:=$80;
					1:getRangeValue(v,-63,63);
					2:getRangeValue(v,0,127);
					3:SFX_modulate[_pos]:=$00;
					4:exit(false);
					5:exit(true);
				end;
			end;
		2: begin // Low Frequency Modulation/Note Level Modulation
(*
* _mod
* %00000000 - no modulation
* %000xxxxx - incrase freq. value by x
* %001xxxxx - decrase freq. value by x
* %010xxxxx - incrase note by x halftones
* %011xxxxx - decrase note by x halftones
* %1xxxxxxx - jump to position x in SFX definition
* %10000000 - end of sfx definition
*)
				opt:=3;
				optionsList(resptr[menu_LFD_NLM_mode],width_menuSFXFunc,7,opt,key_Up,key_Down);
				case opt of
					0:SFX_modulate[_pos]:=$80;
					1:getRangeValue(v,-31,31);
					2:getRangeValue(v,-31,31);
					3:getRangeValue(v,0,127);
					4:SFX_modulate[_pos]:=$00;
					5:exit(false);
					6:exit(true);
				end;
			end;
		3: begin	// Direct Set Divider
(*
* %xxxxxxxx - direct value to freq. divider register
*)
				opt:=1;
				optionsList(resptr[menu_DFD_mode],width_menuSFXFunc,3,opt,key_Up,key_Down);
				case opt of
					0:getRangeValue(v,0,255);
					1:exit(false);
					2:exit(true);
				end;
			end;
	end;
	result:=false;
end;
