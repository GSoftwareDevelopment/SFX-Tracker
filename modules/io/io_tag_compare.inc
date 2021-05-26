function compareTag(var srcTag,dstTag:TTag):boolean;
var
	i:byte;

begin
	i:=0;
	while (i<5) and (srcTag[i]=dstTag[i]) do i:=i+1;
	result:=(i=5);
end;
