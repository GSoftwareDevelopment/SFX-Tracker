const
	octShift:array[0..5] of byte = (0,12,24,36,50,62);

var
	vis_xpos:array[0..28] of byte absolute VIS_TABLE_ADDR;
	vis_ypos:array[0..28] of byte absolute VIS_TABLE_ADDR+29;

	pianoScreen:array[0..0] of byte absolute KEYBOARD_ADDR;
	old_pianoY:array[0..3] of byte;
	pianoScopeOfs:byte;
	pianoTm:byte;

procedure reset_pianoVis();
var i:byte;

begin
	PMGClear();
	for i:=0 to 3 do
	begin
		playersHPos[i]:=0;
		playersColor[i]:=15;
		playersSize[i]:=PSizeByOne;
		old_pianoY[i]:=0;
	end;
	pianoScopeOfs:=220;
end;

procedure updatePianoVis();
var
	chn,chnOfs,note,minNote,vol:byte;
//	gvol,activeChns:byte;
	pOfs:word;

begin
	if (timer=pianoTm) then exit;
	pianoTm:=timer;
	chn:=0; chnOfs:=0; minNote:=octShift[currentOct];
//	gvol:=0; activeChns:=1;
	repeat
		vol:=channels[chnOfs+7] and $0f;
//		gvol:=gvol+vol;
		if vol>0 then
		begin
			pOfs:=$100*chn;
			note:=channels[chnOfs+3]-minNote;
//			activeChns:=activeChns+1;
			if note<=28 then
			begin
				fillchar(@playersData[pOfs+old_pianoY[chn]],15,0);
				playersHPos[chn]:=vis_xpos[note];
				pOfs:=pOfs+vis_ypos[note]+15-vol;
				fillchar(@playersData[pOfs],vol,%11111000);
				old_pianoY[chn]:=pOfs;
			end
			else
				playersHPos[chn]:=0;
		end
		else
		begin
			playersHPos[chn]:=0;
		end;
		chnOfs:=chnOfs+16;
		chn:=chn+1;
	until chnOfs=64;
//	if (pianoScopeOfs<239) then pianoScopeOfs:=pianoScopeOfs+1 else pianoScopeOfs:=220;
//	video[pianoScopeOfs]:=$3f-(gvol shr activeChns);
end;
