procedure updateTheme(var themeColors:array[0..4] of byte);
var i:byte;

begin
	for i:=0 to 4 do
	begin
		poke(colTabAdr+i*5,themeColors[i]);
		putHexValue(17,2+i,themeColors[i],0);
	end;
end;

procedure editColor(nCol:shortint; var themeColors:array[0..4] of byte);
var
	nibbel:byte;
	i,value:byte;

	procedure setValue(value:byte);
	begin
		if nibbel=0 then
		begin
			themeColors[nCol]:=(value shl 4)+(themeColors[nCol] and $0f);
		end
		else
			themeColors[nCol]:=(themeColors[nCol] and $f0)+(value);
	end;

begin
	nibbel:=0;
	colorHLine(width_themeEdit,2+nCol,width_colorEdit,3);
	colorHLine(width_themeEdit+6,2+nCol,1,0);
	screen2video();
	repeat
		if (kbcode<>255) then
		begin
			colorHLine(width_themeEdit,2+nCol,width_colorEdit,0);
			key:=TKeys(kbcode);
			if nibbel=0 then
				value:=themeColors[nCol] shr 4
			else
				value:=themeColors[nCol] and $0f;

			case key of
				key_ESC: begin kbcode:=255; break; end;
				key_RETURN, key_TAB, key_SHIFT_TAB: break;
				key_Up: begin
					if value<15 then value:=value+1 else value:=0;
					setValue(value);
				end;
				key_Down: begin
					if value>0 then value:=value-1 else value:=15;
					setValue(value);
				end;
				key_Left: nibbel:=0;
				key_Right: nibbel:=1;
			end;

			i:=keyScan(key,keys_alphaNum,keysRange_hexNum);
			if (i<>255) then
			begin
				setValue(i);
				nibbel:=1-nibbel;
			end;

			updateTheme(themeColors);
			colorHLine(width_themeEdit,2+nCol,width_colorEdit,3);
			colorHLine(width_themeEdit+6+nibbel,2+nCol,1,0);
			screen2video();
			kbcode:=255;
		end;
	until false;
end;

procedure editTheme(nTheme:shortint);
var i:byte;
	opt:shortint;
	themeColors:array[0..4] of byte;

begin
	getTheme(nTheme,themeColors);
	fillchar(@playersData,$500,0);
	for i:=0 to 4 do fillchar(@playersData[$100+50+i*16],12,255);

	playersHPos[0]:=144;
	playersSize[0]:=PSizeByFour;

	opt:=6;
	box(0,2,20,9,0);
	VBar(0,2,width_themeEdit,0);
	menuBar(resptr[menu_themeEdit],width_themeEdit,1);
	updateBar(resptr[menu_themeEdit],width_themeEdit,opt,1,3);
	updateTheme(themeColors);

	screen2video();

	repeat
		if (kbcode<>255) then
		begin
			key:=TKeys(kbcode); kbcode:=255;
			case key of
				key_ESC: break;
				key_Up: if opt>0 then opt:=opt-1 else opt:=6;
				key_Down: if opt<6 then opt:=opt+1 else opt:=0;
				key_RETURN, key_TAB, key_SHIFT_TAB:
					case opt of
						0..4: editColor(opt,themeColors);
						5: begin
							storeTheme(nTheme,themeColors);
							break;
						end;
						6: break;
					end;
			end;

			if (opt>=0) and (opt<5) and (kbcode<>255) then
			begin
				if (kbcode=key_RETURN) and (opt=4) then
				begin
					opt:=5; kbcode:=255;
				end
				else
				begin
					if (kbcode=key_SHIFT_TAB) then opt:=opt-1 else opt:=opt+1;
					if opt>4 then opt:=0;
					if opt<0 then opt:=4;
				end;
			end;
			updateBar(resptr[menu_themeEdit],width_themeEdit,opt,1,3);
			screen2video();
		end;
	until false;
end;
