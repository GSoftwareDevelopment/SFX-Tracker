procedure SONGEditLoop();
var
	_pos,SONGOfs,maxCols,x,y:byte;
	value:smallint;

	procedure setCursor();
	var
	  order:byte;

	begin
		_pos:=SONGShift+SONGPos;
		SONGOfs:=_pos*4;
		x:=winSONGXStart; y:=2+SONGPos;
		order:=SONGData[SONGOfs];
		if order=chnOrder_EndSong then
		begin
			SONGChn:=0; maxCols:=0;
			colorHLine(x,y,8,3); // 8-hardcoded length of 'END-SONG'
		end
		else
		begin
			if order and chnOrder<>0 then
				case order of
					chnOrder_Tempo,
					chnOrder_JumpTo: maxCols:=1;
					chnOrder_Repeat: maxCols:=2;
				end
			else
				maxCols:=3;
			if SONGChn>maxCols then SONGChn:=maxCols;
			x:=x+SONGChn*3;
			colorHLine(x,y,2,3); // 8-hardcoded length of 'END-SONG'
		end;
	end;

begin
	updateSONG(false);
	// colorHLine(winSONGXStart+SONGChn*3,2+SONGPos,2,3);
	setCursor();
	screen2video(); kbcode:=255;
	reset_pianoVis();
	repeat
		if (kbcode)<>255 then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			controlSelectionKeys(key,key_LEFT,key_RIGHT,SONGChn,0,maxCols);

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

			value:=keyScan(key,keys_alphaNum,keysRange_decNum);
			if (value<>255) then
			begin
				if inputValue(x,y,2,value,0,maxTABs-1,0,0) then
				begin
					if SONGChn=0 then
					begin
						if TABPtr[value]=$FFFF then
							showError(resptr[msg_UnknownDefinition])
						else
						begin
							if (SONGData[SONGOfs] and chnOrder<>0) then
								fillchar(@SONGData[SONGOfs],4,chnBlank);
							SONGData[SONGOfs+SONGChn]:=value;
						end;
					end
					else
					begin
						if (SONGData[SONGOfs] and chnOrder<>0) then
							SONGData[SONGOfs+SONGChn]:=value
						else
							if TABPtr[value]=$FFFF then
								showError(resptr[msg_UnknownDefinition])
							else
								SONGData[SONGOfs+SONGChn]:=value;
					end;

				end;
			end;
			//
			updateSONG(false);
			setCursor();
			screen2video();
		end;
		updatePianoVis();
	until false;
	reset_pianoVis();
	kbcode:=255;
	updateSONG(true);
end;
