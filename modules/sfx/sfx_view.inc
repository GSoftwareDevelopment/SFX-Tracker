procedure updateSFXView();
var
	scrOfs,i,v,ch:byte;
	pos,_v:byte;

	procedure putVal(v:byte); register;
	begin
		if (v<10) then
			screen[scrOfs]:=$10+v
		else
			screen[scrOfs]:=$17+v;
	end;

begin
	i:=0; pos:=cursorShift;
	while (i<SFX_winWidth) do
	begin
		scrOfs:=40+winXStart+i;
		_v:=SFX_vol_dist[pos];
	// volume
		v:=_v and $0f;
	// volume bar
		ch:=$00;
		if (v>12) then ch:=$3e-v+13;
		screen[scrOfs]:=ch;
		if (v>8) then
			if (v>12) then ch:=$3b else ch:=$3e-v+9;
		screen[scrOfs+20]:=ch;
		if (v>4) then
			if (v>8) then ch:=$3b else ch:=$3e-v+5;
		screen[scrOfs+40]:=ch;
		if (v>4) then ch:=$3b else ch:=$3f-v;
		screen[scrOfs+60]:=ch;

	// volume value
		inc(scrOfs,80);
		if (v>0) then
			putVal(v)
		else
			screen[scrOfs]:=byte('-'~);

		scrOfs:=scrOfs+20;

	// Distortion value
		if (_v>0) then
		begin
			v:=_v shr 4; putVal(v);
		end
		else
		  screen[scrOfs]:=byte('-'~);
		scrOfs:=scrOfs+20;

	// SFX_modulate
		_v:=SFX_modulate[pos];
		if _v>0 then
		begin
			ch:=(_v shr 4);	putVal(ch);
			scrOfs:=scrOfs+20;
			ch:=(_v and $0f); putVal(ch);
		end
		else
		begin
			ch:=byte('-'~);
			screen[scrOfs]:=ch;
			screen[scrOfs+20]:=ch;
		end;

		if (pos>=SFXLen) then
			colorVLine(winXStart+i,2,8,2);
		//
		pos:=pos+1;
		i:=i+1;
	end;

	putValue(SFXNumberX,SFXNumberY,currentSFX,2,1);

	fillchar(@moduleBar[4],16,$40);
	putASCIIText(SFXNameX,SFXNameY,SFXName,color_choice);

//	fillchar(@screen[204],16,0);
	putValue(SFXCurPosX,SFXCurPosY,cursorShift+cursorPos,3,0);
	screen[SFXCurPosX+3+vadr[SFXCurPosY]]:=byte('/'~);
	putValue(SFXLenX,SFXLenY,sfxLen,3,0);

	v:=SFXModMode[currentSFX];	i:=1+(v*5);
	move(pointer(word(resptr[menu_sfx_mode])+i),@screen[204],3);

	updatePiano();
end;

procedure SFXScreen();
begin
	clearModule(); //	fillchar(@moduleBar,220,$00);
	VBar(0,1,width_menuBar,10,0);
	colorHLine(SFXNumberX-1,SFXNumberY,4,1);
	menuBar(menu_sfx,width_menuBar,1);
end;
