unit PMGraph;

interface
type
   byteArray = array[0..0] of byte;

const
   PSizeByOne  = 0;
   PSizeByTwo  = 1;
   PSizeByFour = 3;

var
   playersData:byteArray;
   playersHPos:byteArray absolute $d000;
   playersColor:byteArray absolute 704;
   playersSize:byteArray absolute $d008;

procedure PMGInit(pmg_base_page:byte);
procedure PMGClear();

implementation

var
   GPRIOR:byte absolute $26f;
   GRACTL:byte absolute 53277;
   SDMCTL:byte absolute $22f;
   PMBASE:byte absolute 54279;

procedure PMGInit;
begin
   PMBASE:=pmg_base_page;
   SDMCTL:=%00111010;   // Antic on; PM Regular height; Players On; Missiles off; Regular playfield size;
   GRACTL:=3;           // turn on P/M graphics
   GPRIOR:=%00010001;   //
   playersData:=pointer((pmg_base_page+4)*$100);
end;

procedure PMGClear;
begin
   fillchar(@playersData,$400,0);
end;

end.
