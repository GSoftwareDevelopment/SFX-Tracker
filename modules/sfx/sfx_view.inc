procedure updateSFXView();
var
   i,v,ch,fnCh:byte;
   MOD_Mode,pos,_v:byte;

	procedure _putChar(ch:byte); register;
	begin
		screen[__scrOfs]:=ch; inc(__scrOfs,20);
	end;

   procedure putVal(v:byte); register;
   begin
      if (v<10) then
         v:=v+$10
      else
         v:=v+$17;
      _putChar(v);
   end;

begin
   i:=0; pos:=cursorShift;
   MOD_Mode:=SFXModMode[currentSFX];
   while (i<SFX_winWidth) do
   begin
      __scrOfs:=40+winXStart+i;
      _v:=SFX_vol_dist[pos];
   // volume
      v:=_v and $0f;
   // volume bar
      ch:=$00;
      if (v>12) then ch:=$3e-v+13;
      _putChar(ch);
      if (v>8) then
         if (v>12) then ch:=$3b else ch:=$3e-v+9;
      _putChar(ch);
      if (v>4) then
         if (v>8) then ch:=$3b else ch:=$3e-v+5;
      _putChar(ch);
      if (v>4) then ch:=$3b else ch:=$3f-v;
      _putChar(ch);

   // volume value
      if (v>0) then putVal(v) else _putChar($0d); // -

   // Distortion value
      if (_v>0) then
      begin
         v:=_v shr 4; putVal(v);
      end
      else
        _putChar($0d); // -

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
			_putChar(fnCh);

         ch:=(_v shr 4);   putVal(ch);
         ch:=(_v and $0f); putVal(ch);
      end
      else
      begin
			_putChar($00);
			_putChar($0d);
			_putChar($0d);
      end;

      if (pos>=SFXLen) then
      begin
         __scrOfs:=40+winXStart+i; color:=$80; colorVLine(9);
      end;
      //
      pos:=pos+1;
      i:=i+1;
   end;

	// show SFX number
	color:=$00; strVal2Mem(pointer(word(resptr[menu_sfx])+2),currentSFX,2);
   __scrOfs:=21; color:=$40; putValue(currentSFX,2);

	// show SFX name
   fillchar(@moduleBar[4],16,$40);
   __scrOfs:=24; putASCIIText(SFXName);

	i:=SFXshortcutExist(255,currentSFX);
	if i<>255 then moduleBar[19]:=$50+i;

	// show edit status
	clearStatusBar();
	__scrOfs:=233; color:=0; putValue(cursorShift+cursorPos,3);
	putChar($0f); // /
	putValue(sfxLen,3);

   i:=1+(MOD_Mode*5);
   move(pointer(word(resptr[menu_sfx_mode])+i),@screen[220],3);

   v:=SFXNoteSetOfs[currentSFX] shr 6;
   __scrOfs:=224; putChar($21+v);

   updatePiano();
end;

procedure SFXScreen();
begin
   clearModule();
   VBar(0,1,width_menuBar,10,0);
   menuBar(menu_sfx,width_menuBar,section,1,2);

   getSFXData(currentSFX);
   SFXDetermineLength();
   updateSFXView();

end;
