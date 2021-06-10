function SONGEntryEdit(SONGOfs:byte):boolean;
var
	opt:byte;

	procedure editTemp();
	begin
		SONGData[SONGOfs]:=chnOrder_Tempo;
		SONGData[SONGOfs+1]:=song_lpb;
		SONGChn:=1;
	end;

	procedure editJumpTo();
	begin
		SONGData[SONGOfs]:=chnOrder_JumpTo;
		SONGData[SONGOfs+1]:=0;
		SONGChn:=1;
	end;

	procedure editRepeat();
	begin
		SONGData[SONGOfs]:=chnOrder_Repeat;
		SONGData[SONGOfs+1]:=0;
		SONGData[SONGOfs+2]:=0;
		SONGChn:=1;
	end;

begin
	opt:=SONGEditOption_BackToEdit;
	if optionsList(resptr[menu_song_edit],width_menuTABFunc,TABEditOptions,opt,key_Up,key_Down) then
	begin

		if (SONGData[SONGOfs] and chnOrder<>0) then
			fillchar(@SONGData[SONGOfs],4,chnBlank);

		case opt of
			0: SONGData[SONGOfs]:=chnOrder_EndSong;
			4: SONGData[SONGOfs+SONGChn]:=chnOff;
			5: SONGData[SONGOfs+SONGChn]:=chnBlank;
		end;

		box(4,2,16,9,0);
		updateSONG(false);
		if opt>=SONGEditOption_BackToEdit then exit(opt=SONGEditOption_BackToMenuBar);

		case opt of
			1: editTemp();
			2: editJumpTo();
			3: editRepeat();
		end;

		fillchar(@statusBar,20,0);
		result:=false;
	end
	else
	begin
		box(4,2,16,9,0);
		updateSONG(false);
	end;
end;