procedure SONG_Play(startPos:byte);
begin
   SFX_PlaySONG(startPos);
   reset_pianoVis(); keyClick:=false;
   PLYtm:=timer; PLYstate:=2;
   repeat
      updatePianoVis();
      PlayFlash();
      updateSong(true);
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
