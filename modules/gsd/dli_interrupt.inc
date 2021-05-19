const
	colTab:array[0..9*5-1] of byte = (
		$00,$00,$00,$00,$00,
		$00,$00,$00,$00,$00,
		$00,$00,$00,$00,$00,
		$00,$00,$00,$00,$00,
		$00,$00,$00,$00,$00,
		$00,$00,$00,$00,$00,
		$00,$00,$00,$00,$00,
		$00,$00,$00,$00,$00,
		$00,$00,$00,$00,$00
	);
	colTabAdr = word(colTab);
	DLICount = $df;	// DLI counter used for current color table offset
							// on each interrupt incrase about 4
							// max. value 44
							// reset to 0 before initialize interrupt

var
	DLIThemeCnt:byte absolute DLICount;

procedure theme_dli;assembler;interrupt;
asm {
; Register backup for DLI interrupt
_regA 	= $fd;
_regX 	= $fe;
_regY 	= $ff;

	sta _regA			; store registers A & Y
	sty _regY

	ldy DLICount		; read line counter register

	lda colTabAdr,y	; read first color entry
	sta $D012			; store player 0 color
	iny					; increment y reg
	lda colTabAdr,y	; 2nd color
	sta $D013
	iny
	lda colTabAdr,y	; 3rd color
	sta $d014
	iny
	lda colTabAdr,y	; 4th color
	sta $d015
	iny
	lda colTabAdr,y	; 5th color (for background)
	sta $d018
	iny

	cpy #45			; check, if its last line?
   bne storeRegY
   ldy #0			; yes, reset register

storeRegY:
	sty DLICount	; store register value

   ldy _regY		; restore old A & Y register
   lda _regA
};
end;
