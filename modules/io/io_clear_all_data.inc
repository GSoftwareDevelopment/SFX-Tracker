procedure IO_clearAllData();
begin
	HEAP_Init();	// clear all data
	fillchar(@SFXPtr,maxSFXs*2,$ff);
	fillchar(@TABPtr,maxTABs*2,$ff);
	fillchar(@SONGData,256,$ff);
	fillchar(@SONGTitle,SongNameLength,0);

	currentSFX:=0;
	currentTAB:=0;
	currentOct:=$10;
	song_tact:=4;
	song_beat:=4;
	song_lpb:=4;
end;
