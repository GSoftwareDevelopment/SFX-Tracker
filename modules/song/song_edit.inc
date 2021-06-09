procedure SONGEditLoop();
var
	_pos,SONGOfs,x,y:byte;
	i:smallint;

begin
	updateSONG(false);
	colorHLine(winSONGXStart+SONGChn*3,2+SONGPos,2,3);
	screen2video(); kbcode:=255;
	reset_pianoVis();
	repeat
		if (kbcode)<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			x:=winSONGXStart+SONGChn*3;
			y:=2+SONGPos;
			colorHLine(x,y,2,0);
			_pos:=SONGShift+SONGPos;
			SONGOfs:=_pos*4;

			controlSelectionKeys(key,key_LEFT,key_RIGHT,SONGChn,0,3);

			case key of
				key_ESC: break;
				key_SPACE: begin
					if (SONGData[SONGOfs] and chnOrder<>0) then
						fillchar(@SONGData[SONGOfs],4,chnBlank)
					else
						SONGData[SONGOfs+SONGChn]:=chnBlank;
				end;
				key_RETURN: if SONGEntryEdit(SONGOfs) then break;
			end;

			controlEditKeys(key,SONG_winHeight,SONG_maxLength,SONGPos,SONGShift,key_Up,key_Down);

			i:=keyScan(key,keys_alphaNum,keysRange_decNum);
			if (i<>255) then
			begin
				if inputValue(x,y,2,i,0,maxTABs-1,0,0) then
				begin
					if TABPtr[i]=$FFFF then
						showError(resptr[msg_UnknownDefinition])
					else
					begin
						if (SONGChn=0) and (SONGData[SONGOfs] and chnOrder<>0) then
							fillchar(@SONGData[SONGOfs],4,chnBlank);
						SONGData[SONGOfs+SONGChn]:=i;
					end;
				end;
			end;
			//
			updateSONG(false);
			colorHLine(winSONGXStart+SONGChn*3,2+SONGPos,2,3);
			screen2video();
		end;
		updatePianoVis();
	until false;
	reset_pianoVis();
	kbcode:=255;
	updateSONG(true);
end;
