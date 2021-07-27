procedure NoteTunePiano();
begin
   move(resptr[pianoTuneOdd],@screen[tunePianoPos],12);
   move(resptr[pianoTuneEven],@screen[tunePianoPos+20],12);
end;

procedure updateNoteTune(baseOct:byte);
var
   indChar:array[0..0] of byte;
   noteShift,v,i,__scrOfs,_pos:byte;

	procedure _putChar(ch:byte);
	begin
		screen[__scrOfs]:=ch;
		inc(__scrOfs,20);
	end;

begin
   definedNoteTune:=pointer(NOTE_TABLE_ADDR+currentNoteTableOfs);
   indChar:=resptr[tuneIndChars];
   putMultiText(msg_pianoTuneInfo,0);
   screen[54+baseOct]:=screen[54+baseOct] xor $c0;
   noteShift:=octShift[baseOct];
   for i:=0 to 11 do
   begin
      __scrOfs:=tuneBarsPos+i;
      _pos:=noteShift+i;
      if (_pos<64) then
      begin
         v:=(definedNoteTune[_pos] and $0f) shl 1;
         // tune bar
         if v>15 then
         begin
				_putChar($00);
				_putChar($3f);
				_putChar(indChar[v]);
				_putChar(indChar[v+1]);
         end
         else
         begin
				_putChar(indChar[v]);
				_putChar(indChar[v+1]);
				_putChar($00);
				_putChar($00);
         end;
         // high nibbel of note frequency
         v:=definedNoteTune[_pos] shr 4;
         if (v<10) then
            _putChar($10+v)
         else
            _putChar($17+v);
      end
      else
      begin
			_putChar($00);
			_putChar($3f);
			_putChar($00);
			_putChar($00);
			_putChar($00);
      end;
   end;
end;

procedure NoteTuneRedraw();
begin
   clearWorkarea();
   VBar(0,2,width_menuBar,9,0);
   menuBar(menu_note_tune,width_menuBar,-1,1,0);
   fillchar(@moduleBar[4],16,$40);
   noteSetId:=currentNoteTableOfs shr 6;
   __scrOfs:=44; putNText(pointer(word(resptr[menu_NoteDef])+4+noteSetId*16),1);
   updateNoteTune(currentOct);
   NoteTunePiano();
end;
