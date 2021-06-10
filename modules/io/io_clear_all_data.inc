procedure IO_clearAllData();
begin
	HEAP_TOP:=0;
	fillchar(@_mem,HEAP_MEMORY_SIZE,$ff);
	fillchar(@_heap_sizes,HEAP_ENTRIES*2,$FF);

	fillchar(@SFXModMode,maxSFXs*2,$00);
	fillchar(@SFXPtr,maxSFXs*2,$ff);
	fillchar(@TABPtr,maxTABs*2,$ff);
	fillchar(@SONGData,256,chnBlank);
	fillchar(@SONGTitle,SONGNameLength+1,0);
	move(@defaultSongTitle,@SONGTitle,length(defaultSongTitle)+1);

	SONGChn:=0; SONGPos:=0; SONGShift:=0;
	currentSFX:=0;
	currentTAB:=0;
	currentOct:=defaultPianoOctave;
//	song_tact:=4;
	song_beat:=defaultSongBeat;
	song_lpb:=defaultSongTempo;
end;
