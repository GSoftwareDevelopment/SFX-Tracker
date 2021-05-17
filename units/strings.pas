unit Strings;

interface

function indexOf(var s:string; var search:string):smallint;
function splice(var s:string; start:smallint; len:smallint):string;

implementation

function indexOf:smallint;
var i,j:byte;
	sLen,searchLen:byte;

begin
	sLen:=length(s); searchLen:=length(search);
	result:=-1;
	if (sLen=0) or (searchLen=0) then exit;
	i:=1; j:=1;
	while (i<=slen) and (j<=searchLen) do
	begin
		if (s[i]=search[j]) then
		begin
			if (j=1) then result:=i;
			j:=j+1;
			if (j>searchLen) then exit;
		end
		else
			j:=1;
		i:=i+1;
	end;
end;

function splice:string;
var i,slen:byte;
	ostr:string;

(* jest błąd, jeżeli length(s)=1 i len>1 *)

begin								// start=-1 len=3;
	slen:=length(s);			// slen=1;
	if (start<1) then			// -1<1=true
		start:=1					// start=1
	else
		if (start>slen) then start:=slen+1;
	if (len>slen) then len:=slen;	// 3>1=true; len=1
	if (len+start-1>=slen) then len:=slen-start+1;	// 1+1-1>=1?(2-1)>=1?-1>=1=false;
	if len<=0 then // (1<=0)=false
	begin
		result:='';
		exit;
	end;
	i:=0;
	ostr[0]:=char(len);
	while len>0 do
	begin
		ostr[1+i]:=s[start+i];
		i:=i+1;
		len:=len-1;
	end;
	result:=ostr;
end;

end.
