procedure controlEditKeys(key:byte; winSize:shortint; max:byte; var _pos,_shift:byte; pcKey,ncKey:byte);
var step:shortint;

begin
	if (key and $80=$80) then
	begin
		key:=key and $7f;
		step:=winSize;
	end
	else
		step:=1;
	if key=pcKey then
		moveCursor(-step,winSize,max,_pos,_shift)
	else
		if key=ncKey then
			moveCursor(step,winSize,max,_pos,_shift);
end;
