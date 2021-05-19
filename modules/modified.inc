procedure updateModified();
var
	modTm:longint;
	modState:boolean;
	col:byte;

begin
	if (getTime-modTm>25) then
	begin
		if (modified) then
		begin
			modState:=not modState;
			col:=screen[203] and $c0;
			if modState then
				screen[203]:=col or $46
			else
				screen[203]:=col or $4c;
		end
		else
			screen[393]:=$40;
		screen2video();
		modTm:=getTime;
	end;
end;
