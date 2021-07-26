procedure updateSFXView();
var
   scrOfs,i,v,ch,fnCh:byte;
   MOD_Mode,pos,_v:byte;

   procedure putVal(v:byte); register;
   begin
      if (v<10) then
         screen[scrOfs]:=$10+v
      else
         screen[scrOfs]:=$17+v;
   end;

begin
   i:=0; pos:=cursorShift;
   MOD_Mode:=SFXModMode[currentSFX];
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

		fnCh:=$00;
		case MOD_Mode of
			0: begin // HFD
					if _v=$80 then
						fnCh:=$09 // stop sign
					else
						if _v<$80 then
							fnCh:=$1d // freq shift up
						else
							if _v>$80 then
								fnCh:=$20; // freq shift down
//					_v:=_v and $7f;
				end;
			1: begin // MFD
					if _v and $80=$80 then
						if _v=$80 then fnCh:=$09 else fnCh:=$1c
					else
						if _v>0 then
							if _v and $20=$20 then fnCh:=$1d else fnCh:=$20;	// freq

				end;
			2: begin // LFD/NLM
					if _v and $80=$80 then
						if _v=$80 then fnCh:=$09 else fnCh:=$1c
					else
						if _v>0 then
							if _v and $40=$40 then
								if _v>=$60 then fnCh:=$1b else fnCh:=$0c			// note
							else
								if _v>=$20 then fnCh:=$20 else fnCh:=$1d;			// freq
				end;
      end;

      if _v>0 then
      begin
			screen[scrOfs]:=fnCh;
         scrOfs:=scrOfs+20;

         ch:=(_v shr 4);   putVal(ch);
         scrOfs:=scrOfs+20;
         ch:=(_v and $0f); putVal(ch);
      end
      else
      begin
         screen[scrOfs]:=$00;	// -
         screen[scrOfs+20]:=$0d;	// -
         screen[scrOfs+40]:=$0d;	//
      end;

      if (pos>=SFXLen) then
         colorVLine(winXStart+i,2,9,2);
      //
      pos:=pos+1;
      i:=i+1;
   end;

	// show SFX number
   putValue(SFXNumberX,SFXNumberY,currentSFX,2,1);

	// show SFX name
   fillchar(@moduleBar[4],16,$40);
   putASCIIText(SFXNameX,SFXNameY,SFXName,color_choice);

	i:=SFXshortcutExist(255,currentSFX);
	if i<>255 then moduleBar[19]:=$50+i;

	// show edit status
	clearStatusBar();
	putValue(233,0,cursorShift+cursorPos,3,0);
	screen[236]:=byte('/'~);
	putValue(237,0,sfxLen,3,0);

   i:=1+(MOD_Mode*5);
   move(pointer(word(resptr[menu_sfx_mode])+i),@screen[220],3);

   v:=SFXNoteSetOfs[currentSFX] shr 6;
   screen[224]:=$21+v;

   updatePiano();
end;

procedure SFXScreen();
begin
   clearModule();
   VBar(0,1,width_menuBar,10,0);
   colorHLine(SFXNumberX-1,SFXNumberY,4,1);
   menuBar(menu_sfx,width_menuBar,1);
end;
