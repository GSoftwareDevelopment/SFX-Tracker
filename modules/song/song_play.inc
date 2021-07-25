procedure SONG_Play(startPos:byte);
var _SONGRow,oSONGOfs:byte;

begin
   SFX_PlaySONG(startPos);
   reset_pianoVis(); keyClick:=false;
   PLYtm:=timer; PLYstate:=2; oSongOfs:=255;
   repeat
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

      if keyPressed then
      begin
         case key of
            key_ESC: break;
         end;
      end;
      if (timer-ScreenTm)>0 then
      begin
         ScreenTm:=timer;
         move(@screen,@video,240);
      end;
   until SONG_Tick>$80;
   SFX_Off(); keyClick:=true; key:=TKeys($ff);
   reset_pianoVis();
   clearStatusBar();
end;
