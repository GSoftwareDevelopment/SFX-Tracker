procedure SONGEditLoop();
var
   SONGRow,SONGAdr,maxCols,x,y:byte;
   value:smallint;
   nTab:byte;

   procedure calcPos();
   var _pos:byte;
   begin
      _pos:=SONGShift+SONGPos;
      SONGRow:=_pos*4;
      x:=winSONGXStart; y:=2+SONGPos;
      SONGAdr:=SONGRow+SONGChn;
   end;

   function isRowOnOrder():boolean;
   begin
      result:=(SONGData[SONGRow] and chnOrder<>0);
   end;

   procedure drawCursor();
   var
     w,order:byte;

   begin
      order:=SONGData[SONGRow];
      if order=chnOrder_EndSong then
      begin
         SONGChn:=0; maxCols:=0;
         w:=8; // 8 is length of 'END-SONG'
      end
      else
      begin
         if order and chnOrder<>0 then
            case order of
               chnOrder_Tempo,
               chnOrder_JumpTo: maxCols:=1;
               chnOrder_Repeat: maxCols:=2;
            end
         else
            maxCols:=3;
         if SONGChn>maxCols then SONGChn:=maxCols;
         x:=x+SONGChn*3;
         w:=2;
      end;
      colorHLine(x,y,w,3);
      SONGAdr:=SONGRow+SONGChn;

      nTab:=SONGData[SONGAdr];
      clearStatusBar();
      if not isRowOnOrder then
         showTABInfo(nTAB)
   end;

   procedure clearRow();
   begin
      fillchar(@SONGData[SONGRow],4,chnBlank);
   end;

   procedure storeInSONGTab(value:shortint);
   begin
      if SONGChn=0 then
      begin
         if (value<64) and (TABPtr[value]=$FFFF) then
         begin showError(msg_UnknownDefinition); exit; end
         else
            if isRowOnOrder then clearRow();
      end
      else
      begin
         if not isRowOnOrder then
            if (value<64) and (TABPtr[value]=$FFFF) then
            begin showError(msg_UnknownDefinition); exit; end;
      end;
      SONGData[SONGAdr]:=value
   end;

begin
   updateSONG(false);
   calcPos();
   drawCursor();
   screen2video();
   reset_pianoVis();
   repeat
      if keyPressed then
      begin
         if controlSelectionKeys(key,key_LEFT,key_RIGHT,SONGChn,0,maxCols) or
            controlEditKeys(key,SONG_winHeight,SONG_maxLength,SONGPos,SONGShift,key_Up,key_Down) then
         begin
            calcPos();
         end;

         if key=138 (* CTRL+P *) then SONG_Play(SONGRow);

         case key of
            key_ESC: break;
            key_SPACE: begin
               if isRowOnOrder then
                  clearRow()
               else
                  storeInSONGTab(chnBlank);
            end;
            key_RETURN: if SONGEntryEdit(SONGRow) then break;
            key_CTRL_RETURN: if not isRowOnOrder then
                              begin
                                 if nTAB>=maxTABs then nTAB:=0;
                                 nTAB:=dataList(TABPtr,nTAB,maxTABs,TABNameLength);
                                 SONGScreen();
                                 if (nTAB<maxTABs) then
                                    storeInSONGTab(nTAB);
                              end;
            key_CTRL_INSERT: begin
                                 move(@SONGData[SONGRow],@SONGData[SONGRow+4],252-SONGRow);
                                 clearRow();
                              end;
            key_CTRL_DELETE: begin
                                 move(@SONGData[SONGRow+4],@SONGData[SONGRow],252-SONGRow);
                                 value:=SONGRow; SONGRow:=252;
                                 clearRow();
                                 SONGRow:=value;
                              end;
            key_SHIFT_RETURN: begin
                                 TABScreen();
                                 TABEditLoop();
                                 SONGScreen();
                              end;
         end;

         value:=keyScan(key,keys_alphaNum,keysRange_decNum);
         if (value<>255) then
            if inputValue(x,y,2,value,0,maxTABs-1,0,0) then
               storeInSONGTab(value);

         //
         updateSONG(false);
         calcPos();
         drawCursor();
         screen2video();
      end;
   until false;
   reset_pianoVis();
   updateSONG(true);
end;
