var
	pianoScreen:array[0..0] of byte absolute KEYBOARD_ADDR;

procedure updatePianoOctave();
begin
	pianoScreen[20]:=$10+currentOct;
	pianoScreen[39]:=$12+currentOct;
end;

procedure sfx_piano();
var ofs,i:byte;
	SFXAddr:word;
	SFXMode:byte;
	Note:byte;

begin
	updatePianoOctave();
	SFX_Start;
	ofs:=220;
	repeat
		screen[ofs]:=$00; if (ofs<240) then ofs:=ofs+1 else ofs:=220;
		screen[ofs]:=$0d;
		screen2video();
		if kbcode<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			if controlSelectionKeys(key,key_SHIFT_TAB,key_TAB,currentOct,0,3) then
				updatePianoOctave();
			case key of
				key_ESC: break;
			end;
			i:=keyScan(key,keys_notes,34);
			if i<>255 then
			begin
				// SFXAddr:=HEAP_MEMORY_ADDR+SFXPtr[currentSFX]+SFXNameLength;
				SFXAddr:=SFXPtr[currentSFX];
				SFXAddr:=SFXAddr+HEAP_MEMORY_ADDR+SFXNameLength;
				SFXMode:=SFXModMode[currentSFX];
				if i>16 then Note:=i-5 else Note:=i;
				Note:=Note+(currentOct*12);
				SFX_Note(0,Note,SFXMode,SFXAddr);
			end;
		end;
	until false;
	SFX_End();
end;
