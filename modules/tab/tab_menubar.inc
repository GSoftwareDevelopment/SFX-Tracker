function TABList():boolean;
var
	nTAB:shortint;

begin
	move(@screen[20],@tmpbuf,200);
	box(0,2,20,9,$40);
	prepareTABsList();
	nTAB:=listChoice(4,2,TABNameLength,9,currentTAB,listBuf,maxTABs,true);
	move(@tmpbuf,@screen[20],200);
	if (nTAB<>-1) and (nTAB<>currentTAB) then
	begin
		getTABData(nTAB);
		TABDetermineLength();
		result:=true;
	end
	else
		result:=false;
end;

procedure TABMenuBar(section:byte);
begin
	case section of
		0: if TABList then
			begin
				updateTAB(true);
				updateTABInfo();
				modified:=false;
			end;
		1: TABEditLoop();
		3: TAB_Options(4-byte(modified));
		4: begin
				SFXList();
				updateTABSFX();
			end;
	end;

end;
