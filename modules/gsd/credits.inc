procedure credits();
begin
   box(0,1,20,11,$80);
   putMultiText(resptr[msg_Credits],2);
   screen2video();
   kbcode:=255; repeat until kbcode<>255; kbcode:=255;
   move(@tmpbuf[20],@screen[20],220);
end;
