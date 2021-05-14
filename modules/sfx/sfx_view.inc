procedure updateSFXView();
var
	ofs,i,v,ch:byte;
	pos:smallint;

begin
	i:=0; pos:=cursorShift;
	v:=SFX_modulate[0] and $7f; // init "v" modulation (can't be MOD80, before first iteration)

	while (i<SFX_winWidth) do
	begin
		ofs:=40+winXStart+i;

		if (v and $80<>$80) then
		begin
		// volume
			v:=SFX_vol_dist[pos] and $0f;
		// volume bar
			ch:=$00;
			if (v>12) then ch:=$3c+v-13;
			screen[ofs]:=ch;
			if (v>8) then
				if (v>12) then ch:=$3f else ch:=$3c+v-9;
			screen[ofs+20]:=ch;
			if (v>4) then
				if (v>8) then ch:=$3f else ch:=$3c+v-5;
			screen[ofs+40]:=ch;
			if (v>0) then
				if (v>4) then ch:=$3f else ch:=$3b+v;
			if (v=0) then ch:=$3b;
			screen[ofs+60]:=ch;
		// volume value
			if (v>0) then
			begin
				if (v<10) then
					screen[ofs+80]:=$10+v
				else
					screen[ofs+80]:=$17+v;
			end
			else
				screen[ofs+80]:=byte('-'~);

			ofs:=ofs+100;

		// Distortion value
			if (SFX_vol_dist[pos]>0) then
			begin
				v:=SFX_vol_dist[pos] shr 4;
//				screen[ofs]:=byte(hexVal[v+1]);
				if (v<10) then
					screen[ofs]:=$10+v
				else
					screen[ofs]:=$17+v;
			end
			else
			  screen[ofs]:=byte('-'~);
			ofs:=ofs+20;

		// SFX_modulate
			if SFX_modulate[pos]>0 then
			begin
				v:=SFX_modulate[pos];
				ch:=(v shr 4);
				if (ch<10) then
					screen[ofs]:=$10+ch
				else
					screen[ofs]:=$17+ch;
				ofs:=ofs+20;
				ch:=(v and $0f);
				if (ch<10) then
					screen[ofs]:=$10+ch
				else
					screen[ofs]:=$21+ch-10;
			end
			else
			begin
				ch:=byte('-'~);
				(* here is a bug - is reported on GitHub Repo #39
				screen[ofs]:=ch; ofs:=ofs+20;
				screen[ofs]:=ch;
				*)
				screen[ofs]:=ch;
				screen[ofs+20]:=ch;
			end;

			pos:=pos+1;
		end
		else
		begin
			for ch:=0 to 7 do
			begin
				screen[ofs]:=0; ofs:=ofs+20;
			end;
		end;

		//
		i:=i+1;
	end;

	putValue(SFXNumberX,SFXNumberY,currentSFX,2,1);

	fillchar(@screen[SFXNameX+vadr[SFXNumberY]],16,$40);
	putText(SFXNameX,SFXNameY,SFXName,color_choice);

	fillchar(@screen[204],16,0);
	putValue(SFXCurPosX,SFXCurPosY,cursorShift+cursorPos,3,0);
	screen[SFXCurPosX+3+vadr[SFXCurPosY]]:=byte('/'~);
	putValue(SFXLenX,SFXLenY,sfxLen,3,0);
end;
