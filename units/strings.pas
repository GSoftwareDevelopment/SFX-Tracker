unit Strings;

interface

function findChar(var s:string; search:char; start:byte):byte;
function findChar_(var s:string; search:char; start:byte):byte;
function indexOf(var s:string; var search:string; start:byte):byte;
function indexOf_(var s:string; var search:string; start:byte):byte;
function splice(var s:string; start:byte; len:shortint):string;
procedure joinPathName(var path,name,result:string);

implementation

(* Find a character `search` in string `s`
 * `start` parameter - the initial search location in `s` string, or if negative, search from the end of `s` string
 *)
function findChar:byte;
var sLen:byte;

begin
   sLen:=length(s);
   result:=0;
   if (sLen=0) then exit;
   if (start=0) then start:=1;
   while (start<=slen) do
   begin
      if (s[start]=search) then
         exit(start);
      start:=start+1;
   end;
end;

function findChar_:byte;
var sLen:byte;

begin
   sLen:=length(s);
   result:=0;
   if (sLen=0) then exit;
   if (start=0) then start:=1;
   start:=sLen-start+1;
   while (start>0) do
   begin
      if (s[start]=search) then
         exit(start);
      start:=start-1;
   end;
end;

(* Search for a string `search` in string `s`
 * `start` parameter - the initial search location in `s` string, or if negative, search from the end of `s` string
 *)
function indexOf:byte;
var j:byte;
   sLen,searchLen:byte;

begin
   sLen:=length(s); searchLen:=length(search);
   result:=0;
   if (sLen=0) or (searchLen=0) then exit;
   if (start=0) then start:=1;
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
end;

function indexOf_:byte;
var j:byte;
   sLen,searchLen:byte;

begin
   sLen:=length(s); searchLen:=length(search);
   result:=0;
   if (sLen=0) or (searchLen=0) then exit;
   if (start=0) then start:=1;
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

//
//

function splice:string;
var i,slen:byte;

begin
   slen:=length(s);
   if (start<1) then
      start:=1
   else
      if (start>slen) then start:=slen+1;
   if (len>slen) then len:=slen;
   if (len+start-1>=slen) then len:=slen-start+1;
   if len<=0 then exit('');
   i:=1;
   result[0]:=char(len);
   while len>0 do
   begin
      result[i]:=s[start];
      i:=i+1; start:=start+1;
      len:=len-1;
   end;
end;

procedure joinPathName;
var
   last:byte;

begin
   last:=findChar_(path,'/',0); // slash (/)
   if (last=0) then
      last:=findChar_(path,'>',0); // more sign (>)
   if (last=0) then
      last:=findChar_(path,':',0); // colon (:)
   if last=0 then
      result:=name
   else
   begin
      result[0]:=char(last+length(name));
      move(@path[1],@result[1],last);
      move(@name[1],@result[1+last],length(name));
//    result:=concat(splice(path,1,last),name);
   end;
end;

end.
