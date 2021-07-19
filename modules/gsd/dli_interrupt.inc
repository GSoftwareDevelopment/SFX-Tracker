const
   DLICount = $1c;   // DLI counter used for current color table offset
                     // on each interrupt incrase about 4
                     // max. value 44
                     // reset to 0 before initialize interrupt

var
   DLIThemeCnt:byte absolute DLICount;
   DLIColTab:array[0..6*5-1] of byte absolute DLI_COLOR_TABLE_ADDR;
   NMIEN:byte absolute $d40e;
   oldDLI:pointer;

procedure DLI_theme;assembler;interrupt;
asm
; Register backup for DLI interrupt
_regA    = $1d;
_regX    = $1e;
_regY    = $1f;

   sta _regA         ; store registers A & Y
   sty _regY

   ldy DLICount      ; read line counter register

   lda DLI_COLOR_TABLE_ADDR,y ; read first color entry
   sta $D012         ; store player 0 color
   iny               ; increment y reg
   lda DLI_COLOR_TABLE_ADDR,y ; 2nd color
   sta $D013
   iny
   lda DLI_COLOR_TABLE_ADDR,y ; 3rd color
   sta $d014
   iny
   lda DLI_COLOR_TABLE_ADDR,y ; 4th color
   sta $d015
   iny
   lda DLI_COLOR_TABLE_ADDR,y ; 5th color (for background)
   sta $d018
   iny

   cpy #30        ; check, if its last line?
   bne storeRegY
   ldy #0         ; yes, reset register

storeRegY:
   sty DLICount   ; store register value

   ldy _regY      ; restore old A & Y register
   lda _regA
end;

procedure DLIInit();
begin
   SFX_End();
   NMIEN:=%00000000; // turn off DLI
   DLIThemeCnt:=0; // reset DLI counter
   getIntVec(iDLI, oldDLI);
   SetIntVec(iDLI, @DLI_theme);
   NMIEN:=%11000000; // turn on DLI
end;

procedure DLIDone();
begin
   NMIEN:=%01000000; // turn off DLI
   setIntVec(iDLI, oldDLI);
   SFX_Start();
end;
