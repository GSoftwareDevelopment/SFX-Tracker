unit Strings;

interface

function indexOf(var s:string; var search:string; start:smallint):smallint;
function splice(var s:string; start:smallint; len:smallint):string;

implementation

(* Search for a string `search` in string `s`
 * `start` parameter - the initial search location in `s` string, or if negative, search from the end of `s` string
 *)

function indexOf:smallint;
var j:byte;
	sLen,searchLen:byte;

begin
	sLen:=length(s); searchLen:=length(search);
	result:=-1;
	if (sLen=0) or (searchLen=0) then exit;
	if (start=0) then start:=1;
	if (start>0) then
	begin
		j:=1;
		while (start<=slen) and (j<=searchLen) do
		begin
			if (s[start]=search[j]) then
			begin
				if (j=1) then result:=start;
				j:=j+1;
				if (j>searchLen) then exit;
			end
			else
				j:=1;
			start:=start+1;
		end;
	end
	else
	begin
		start:=sLen-start+1; j:=searchLen;
		while (start>0) and (j>0) do
		begin
			if (s[start]=search[j]) then
			begin
				j:=j-1;
				if (j=0) then
				begin
					result:=start;
					exit;
				end;
			end
			else
				j:=searchLen;
			start:=start-1;
		end;
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
