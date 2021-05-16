unit io;

(*
This unit is a part of the original SYSUTILS unit.

I have selected only the parts that are responsible for Input/Output operations,
i.e. for communicating with the floppy drive or other derivatives.
*)

interface
type
	TSearchRec = record
		Attr: Byte;
		ExcludeAttr: Byte;
		Name: TString;
		FindHandle: Pointer;
	end;

const
	faReadOnly	= $01;
	faHidden		= $02;
	faSysFile	= $04;
	faVolumeID	= $08;
	faDirectory	= $10;
	faArchive	= $20;
	faAnyFile	= $3f;

function FileExists(name: PString): Boolean;
procedure FindClose(var f: TSearchRec); assembler;
function FindFirst (const FileMask: TString; Attributes: Byte; var SearchResult: TSearchRec): byte;
function FindNext(var f: TSearchRec): byte; assembler;

implementation
function FindFirst(const FileMask: TString; Attributes: Byte; var SearchResult: TSearchRec): byte;
(*
@description: Start a file search and return a findhandle

@param: FileMask: string[32]
@param: Attributes: Byte
@param: SearchResult: TSearchRec

@returns: =0 file matching the specified criteria is found
*)
var f: file;
begin
	assign(f, FileMask);
asm
{	txa:pha

	clc			; iocheck off
	@openfile f #6
loop
	mwa SearchResult :bp2

	ldy #SearchResult.ExcludeAttr-DATAORIGIN
	lda Attributes
	sta (:bp2),y

	ldy #SearchResult.FindHandle-DATAORIGIN

	lda f
	sta (:bp2),y
	iny
	lda f+1
	sta (:bp2),y

	mwa f :bp2

	ldy #s@file.record
	mva <1 (:bp2),y
	iny
	mva >1 (:bp2),y

	ldy #s@file.nrecord
	mva <64 (:bp2),y
	iny
	mva >64 (:bp2),y

	ldy #s@file.buffer
	mva <@buf (:bp2),y
	iny
	mva >@buf (:bp2),y

	@ReadDirFileName f
	sta Result

	adw SearchResult #SearchResult.Name-DATAORIGIN :bp2

	jsr @DirFileName

	mwa SearchResult :bp2

	ldy #SearchResult.Attr-DATAORIGIN
	txa
	sta (:bp2),y

	and Attributes
	ora Result
	beq loop

	pla:tax
};
end;


function FindNext(var f: TSearchRec): byte; assembler;
(*
@description: Find the next entry in a findhandle

@param: var f: TSearchRec

@returns: =0 record matching the criteria, successful
*)
asm
{	txa:pha

loop	mwa f :bp2
	ldy #f.FindHandle-DATAORIGIN
	mva (:bp2),y edx
	iny
	mva (:bp2),y edx+1

	@ReadDirFileName edx
	sta Result

	adw f #f.Name-DATAORIGIN :bp2

	jsr @DirFileName

	mwa f :bp2

	ldy #f.Attr-DATAORIGIN
	txa
	sta (:bp2),y

	ldy #f.ExcludeAttr-DATAORIGIN
	and (:bp2),y
	ora Result
	beq loop

	pla:tax
};
end;


procedure FindClose(var f: TSearchRec); assembler;
(*
@description: Close a find handle

@param: var f: TSearchRec
*)
asm
{	txa:pha

	mwa f :bp2
	ldy #f.FindHandle-DATAORIGIN
	mva (:bp2),y edx
	iny
	mva (:bp2),y edx+1

	clc			; iocheck off
	@closefile edx

	pla:tax
};
end;


function RenameFile(var OldName,NewName: TString): Boolean; assembler;
(*
@description: Renames a file from OldName to NewName

@param: var OldName: string[32]
@param: var NewName: string[32]

@returns: TRUE - successful
@returns: FALSE - I/O error
*)
asm
{	txa:pha

	mva #0 @buf

	@addString OldName
	ldy @buf
	lda #','
	sta @buf+1,y
	inc @buf
	@addString NewName

	sec			; iocheck on
	jsr @openfile.lookup
	bmi stop

	lda #32
	sta iccmd,x
	lda <@buf+1
	sta icbufa,x
	lda >@buf+1
	sta icbufa+1,x
	lda #$00
	sta icax1,x
	sta icax2,x

	m@call	ciov

stop	sty MAIN.SYSTEM.IOResult

	bpl ok

	lda #false
	seq

ok	lda #true
	sta Result

	pla:tax
};
end;


function DeleteFile(var FileName: TString): Boolean; assembler;
(*
@description: Delete a file from the filesystem

@param: var FileName: string[32]

@returns: TRUE - the file was successfully removed
@returns: FALSE - I/O error
*)
asm
{	txa:pha

	sec			; iocheck on
	jsr @openfile.lookup
	bmi stop

	lda #33
	sta iccmd,x
	lda FileName
	add #1
	sta icbufa,x
	lda FileName+1
	adc #0
	sta icbufa+1,x
	lda #$00
	sta icax1,x
	sta icax2,x

	m@call	ciov

stop	sty MAIN.SYSTEM.IOResult

	bpl ok

	lda #false
	seq

ok	lda #true
	sta Result

	pla:tax
};
end;


function FileExists(name: PString): Boolean;
(*
@description: Check whether a particular file exists in the filesystem

@param: name: string[32]

@returns: TRUE - file exists
@returns: FALSE - file not exists
*)
var f: file;
    fm: byte;
begin

{
; XIO 13,#1,0,0,"D:FOOBAR12.DAT"

ciov = $e456

fname .byte "D:FOOBAR12.DAT",$9b

get_status
      ldx #$10            ;IOCB #1
      lda #$0d            ;komenda: STATUS
      sta iccmd,x
      lda #<fname         ;adres nazwy pliku
      sta icbufa,x
      lda #>fname
      sta icbufa+1,x
      lda #$00
      sta icax1,x
      sta icax2,x
      jsr ciov
      ...
}
  fm:=FileMode;

  {$I-}
  Assign (f, name);
  FileMode := fmOpenRead;
  Reset (f);
  Result:=(IoResult<128) and (length(name)>0);
  Close (f);
  {$I+}

  FileMode:=fm;
end;

end.
