procedure ClearEditBufs(isSFX:boolean);
var
	a:byte;
begin
   fillchar(pointer(EDIT_BUF1_ADDR),128,0);
   if isSFX then a:=$00 else a:=$C0;
   fillchar(pointer(EDIT_BUF2_ADDR),128,a);
end;

procedure Copy2Clipboard(isSFX:boolean; size:byte);
begin
	IOOfs:=0;
	if isSFX then
	begin
		push2IOBuf(@section_SFX,5);
		push2IOBuf(@SFXModMode[currentSFX],1);
		push2IOBuf(@SFXNoteSetOfs[currentSFX],1);
	end
	else
	begin
		push2IOBuf(@section_TAB,5);
	end;
	push2IOBuf(@size,1);
	push2IOBuf(pointer(EDIT_BUF1_ADDR),size);
	push2IOBuf(pointer(EDIT_BUF2_ADDR),size);
	drawError(msg_ClipboardCopied);
end;

procedure Cut2Clipboard(isSFX:boolean; size:byte);
begin
	Copy2Clipboard(isSFX,size);
	ClearEditBufs(isSFX);
   modified:=true;
end;

procedure PasteClipboard(isSFX:boolean);
var
	size:byte;

begin
	if IOOfs>0 then
	begin
		if (isSFX and compareTag(IOBuf,section_SFX)) or
			(not isSFX and compareTag(IOBuf,section_TAB)) then
		begin
			ClearEditBufs(isSFX);
			// paste clipboard to current SFX
			IOOfs:=5;
			if isSFX then
			begin
				SFXModMode[currentSFX]:=IOBufGetByte;
				SFXNoteSetOfs[currentSFX]:=IOBufGetByte;
			end;
			size:=IOBufGetByte;
			IOBufGetBlock(pointer(EDIT_BUF1_ADDR),size);
			IOBufGetBlock(pointer(EDIT_BUF2_ADDR),size);
			modified:=true;
			drawError(msg_ClipboardPasted);
		end
		else
		begin
			// wrong data in clipboard
			drawError(msg_ClipboardBadData);
		end;
	end
	else
	begin
		// clipboard is empty
		drawError(msg_ClipboardEmpty);
	end;
end;

