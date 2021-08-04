var
   vis_xpos:array[0..28] of byte absolute VIS_TABLE_ADDR;
   vis_ypos:array[0..28] of byte absolute VIS_TABLE_ADDR+29;

   pianoScreen:array[0..0] of byte absolute VIDEO_PIANO_ADDR;
   old_pianoY:array[0..3] of byte;
   pianoTm:byte;

   multiChnMode:boolean = false;
   keys_notes:byteArray;

procedure updatePiano();
var v:byte;
begin
   if multiChnMode then
      v:=byte('M'~)     // multi channel
   else
      v:=byte('S'~); // single channel
   pianoScreen[0]:=v;
   pianoScreen[20]:=$10+currentOct; // base octave
   pianoScreen[39]:=$12+currentOct; // finish octave

   v:=SFXNoteSetOfs[currentSFX] shr 6;
   pianoScreen[19]:=$21+v;          // note table
end;

function pianoKeyScan(playChn:byte):byte;
var
	okey:TKeys;
	__chn:byte;

begin
   if controlSelectionKeys(key_SHIFT_TAB,key_TAB,currentOct,0,3) then
      updatePiano();
//   okey:=key; key:=key and $3f;
   result:=keyScan(keys_notes,34);
//   key:=okey;
   if result<>255 then
   begin
      if SFXPtr[currentSFX]=$FFFF then
      begin
         showError(msg_UnknownDefinition);
         exit(255);
      end;

      if result>16 then result:=result-5;
      result:=result+octShift[currentOct];
      if (result<64) and (playChn<>255) then
      begin
			__chn:=playChn*16;
			channels[__chn+_tabOfs]:=$ff;
			channels[__chn+_tabRep]:=$00;
         SFX_Note(playChn,result,currentSFX);
      end
      else
         exit(255);
   end;
end;

procedure reset_pianoVis();
var i,c:byte;

begin
   PMGClear(); c:=$28;
   for i:=0 to 3 do
   begin
      playersHPos[i]:=0;
      playersColor[i]:=c; inc(c,$30);
      playersSize[i]:=PSizeByOne;
      old_pianoY[i]:=0;
   end;
end;

procedure updatePianoVis();
var
   chn,chnOfs,note,minNote,vol,x,y:byte;
   pOfs:word;

begin
   if (timer=pianoTm) then exit;
   pianoTm:=timer;
   chn:=0; chnOfs:=0; minNote:=octShift[currentOct];
   repeat
      vol:=channels[chnOfs+_chnCtrl] and $0f; x:=0;
      if vol>0 then
      begin
         pOfs:=$100*chn;
         note:=channels[chnOfs+_chnNote]-minNote;
         if note<=28 then
         begin
            y:=old_pianoY[chn];
            fillchar(@playersData[pOfs+y],14,0);
            x:=vis_xpos[note];
            y:=vis_ypos[note]-vol;
            fillchar(@playersData[pOfs+y],vol,%11111000);
            old_pianoY[chn]:=y;
         end;
      end;
      playersHPos[chn]:=x;
      chn:=chn+1;
      chnOfs:=chnOfs+$10;
   until chnOfs=$40;
end;
