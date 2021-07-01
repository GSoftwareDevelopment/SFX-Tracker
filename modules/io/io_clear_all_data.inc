procedure IO_clearAllData();
begin
	// Clear HEAP
	HEAP_TOP:=0;
	fillchar(@_mem,HEAP_MEMORY_SIZE,$FF);
	fillchar(@_heap_sizes,HEAP_ENTRIES*2,$FF);
	fillchar(@HEAP_PTR,256,$FF); // This will also delete the indicators for SFX and TABs

	// Clear other data
	fillchar(@SFXNoteSetOfs,maxSFXs,0);
	fillchar(pointer(NOTE_TABLE_ADDR),256,0);
	fillchar(@SFXModMode,maxSFXs,$00);
	fillchar(@SONGData,256,chnBlank);
	fillchar(@SONGTitle,SONGNameLength+1,0);
	move(@defaultSongTitle,@SONGTitle,length(defaultSongTitle)+1);

	// Set defaults
	SONGChn:=0; SONGPos:=0; SONGShift:=0;
	currentSFX:=0;	currentTAB:=0;
	currentOct:=defaultPianoOctave;
	SONG_Beat:=defaultSongBeat;
	SONG_tempo:=defaultSongTempo;
end;
