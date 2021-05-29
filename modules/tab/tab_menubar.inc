function TABList():boolean;
var
	nTAB:shortint;

begin
	nTAB:=dataList(TABPtr,currentTAB,maxTABs,TABNameLength);
	if (nTAB<>-1) and (nTAB<>currentTAB) then
	begin
		getTABData(nTAB);
		TABDetermineLength();
		result:=true;
	end
	else
		result:=false;
end;

procedure TABChangeMenuBarOpt(section:byte);
begin
	case section of
		0: begin
				controlSelectionKeys(key,key_Left,key_Right,currentTAB,0,maxTABs-1);
				modified:=false;
				getTABData(currentTAB);
				TABDetermineLength();
				updateTABInfo();
				updateTAB(true);
			end;
		4: begin
				controlSelectionKeys(key,key_Left,key_Right,currentSFX,0,maxSFXs-1);
				updateTABSFX();
			end;
	end;
end;

procedure TABSelectMenuBar(section:byte);
begin
	case section of
		0: if TABList then
			begin
				updateTAB(true);
				updateTABInfo();
				modified:=false;
			end;
		1: begin
				TABEditLoop();
				if modified then
				begin
					TABDetermineLength();
					storeTABData(currentTAB);
					modified:=false;
				end;
			end;
		3: TAB_Options(TABMenuOption_Back);
		4: begin
				SFXList();
				updateTABSFX();
			end;
	end;

end;
