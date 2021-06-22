procedure SONGEditLoop();
var
	_pos,SONGOfs,SONGAdr,maxCols,x,y:byte;
	value:smallint;
	nTab:byte;

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
		SONGAdr:=SONGOfs+SONGChn;
	end;

	procedure storeInSONGTab(value:shortint);
	begin
		if SONGChn=0 then
		begin
			if (value<64) and (TABPtr[value]=$FFFF) then
				showError(msg_UnknownDefinition)
			else
			begin
				if (SONGData[SONGOfs] and chnOrder<>0) then
					fillchar(@SONGData[SONGOfs],4,chnBlank);
				SONGData[SONGAdr]:=value;
			end;
		end
		else
		begin
			if (SONGData[SONGOfs] and chnOrder<>0) then
				SONGData[SONGAdr]:=value
			else
				if (value<64) and (TABPtr[value]=$FFFF) then
					showError(msg_UnknownDefinition)
				else
					SONGData[SONGAdr]:=value;
		end;
	end;

begin
	updateSONG(false);
	// colorHLine(winSONGXStart+SONGChn*3,2+SONGPos,2,3);
	setCursor();
	screen2video(); kbcode:=255;
	reset_pianoVis();
	repeat
		if keyPressed then
		begin
			controlSelectionKeys(key,key_LEFT,key_RIGHT,SONGChn,0,maxCols);

			case key of
				key_ESC: break;
				key_SPACE: storeInSONGTab(chnBlank);
				key_RETURN: if SONGEntryEdit(SONGOfs) then break;
				KEY_CTRL_RETURN: begin
											nTab:=SONGData[SONGAdr];
											if nTab>=maxTABs then nTab:=0;
											nTAB:=dataList(TABPtr,nTab,maxTABs,TABNameLength);
											if (nTAB<maxTABs) then
												storeInSONGTab(nTab);
										end;
			end;

			controlEditKeys(key,SONG_winHeight,SONG_maxLength,SONGPos,SONGShift,key_Up,key_Down);

			value:=keyScan(key,keys_alphaNum,keysRange_decNum);
			if (value<>255) then
				if inputValue(x,y,2,value,0,maxTABs-1,0,0) then
					storeInSONGTab(value);

			//
			updateSONG(false);
			setCursor();
			screen2video();
		end;
		updatePianoVis();
	until false;
	reset_pianoVis();
	updateSONG(true);
end;
