procedure updateSFXView();
var
	scrOfs,i,v,ch:byte;
	pos,endPos:byte;

begin
	i:=0; pos:=cursorShift;
	v:=SFX_modulate[0] and $7f; // init "v" modulation (can't be MOD80, before first iteration)

	while (i<SFX_winWidth) do
	begin
		scrOfs:=40+winXStart+i;

	// volume
		v:=SFX_vol_dist[pos] and $0f;
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
		if (v>0) then
		begin
			if (v<10) then
				screen[scrOfs+80]:=$10+v
			else
				screen[scrOfs+80]:=$17+v;
		end
		else
			screen[scrOfs+80]:=byte('-'~);

		scrOfs:=scrOfs+100;

	// Distortion value
		if (SFX_vol_dist[pos]>0) then
		begin
			v:=SFX_vol_dist[pos] shr 4;
//				screen[scrOfs]:=byte(hexVal[v+1]);
			if (v<10) then
				screen[scrOfs]:=$10+v
			else
				screen[scrOfs]:=$17+v;
		end
		else
		  screen[scrOfs]:=byte('-'~);
		scrOfs:=scrOfs+20;

	// SFX_modulate
		if SFX_modulate[pos]>0 then
		begin
			v:=SFX_modulate[pos];
			ch:=(v shr 4);
			if (ch<10) then
				screen[scrOfs]:=$10+ch
			else
				screen[scrOfs]:=$17+ch;
			scrOfs:=scrOfs+20;
			ch:=(v and $0f);
			if (ch<10) then
				screen[scrOfs]:=$10+ch
			else
				screen[scrOfs]:=$21+ch-10;
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

	fillchar(@screen[SFXNameX+vadr[SFXNumberY]],16,$40);
	putASCIIText(SFXNameX,SFXNameY,SFXName,color_choice);

	fillchar(@screen[204],16,0);
	putValue(SFXCurPosX,SFXCurPosY,cursorShift+cursorPos,3,0);
	screen[SFXCurPosX+3+vadr[SFXCurPosY]]:=byte('/'~);
	putValue(SFXLenX,SFXLenY,sfxLen,3,0);

	i:=1+SFXModMode[currentSfx]*9;
	move(pointer(word(resptr[menu_sfx_mode])+i),@screen[204],7);
end;

procedure SFXScreen();
begin
	section:=0; cursorPos:=0; cursorShift:=0;
	fillchar(@screen[20],220,$00);
	VBar(0,1,width_menuBar,0);
	colorHLine(SFXNumberX-1,SFXNumberY,4,1);
	menuBar(resptr[menu_sfx],width_menuBar,1);
end;
