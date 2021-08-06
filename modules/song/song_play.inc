procedure SONG_Play(startPos:byte);
var _SONGRow,oSONGOfs,__chnOfs,i,v:byte;

	procedure _putChar(ch:byte); register;
	begin
		screen[__scrOfs]:=ch; inc(__scrOfs,20);
	end;

	procedure VolumeBar();
	var ch:byte;
	begin
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
	end;

	procedure updateVolumeBars();
	begin
		__chnOfs:=0;
		for i:=0 to 3 do
		begin
			__scrOfs:=150+i*3;
			v:=channels[__chnOfs+_chnCtrl] and $0f;
			VolumeBar();
			inc(__chnOfs,$10);
		end;
	end;

begin
   SFX_PlaySONG(startPos);
   clearStatusBar();
   reset_pianoVis(); keyClick:=false;
   PLYtm:=timer; PLYstate:=2; oSongOfs:=255;
   repeat
      if keyPressed then
      begin
			controlSelectionKeys(key_CTRL_LEFT,key_CTRL_RIGHT,SONGChn,0,3);
         if controlSelectionKeys(key_LEFT,key_RIGHT,currentSFX,0,maxSFXs-1) then
            updateTABSFX();					// change the current SFX
         pianoKeyScan(SONGChn);
      end;

      if (timer-ScreenTm)>0 then
      begin
			updateVolumeBars();
			updatePianoVis();
			PlayFlash();
			if oSONGOfs<>SONG_Ofs then
			begin
				oSONGOfs:=SONG_Ofs;
				_SONGRow:=SONG_Ofs shr 2;
				SONGShift:=(_SONGRow div 9)*9;
				updateSong(true);
			end;
			updateSongStatus();
         ScreenTm:=timer;
         move(@screen,@video,240);
      end;
   until (SONG_Tick>$80) or checkEscape;
   SFX_Off(); keyClick:=true; key:=TKeys($ff);
   reset_pianoVis();
	updateSong(true);
   clearStatusBar();
end;
