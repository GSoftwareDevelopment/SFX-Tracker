function getResPtr(resId:byte):pointer;
begin
	result:=resptr[resId];
end;

procedure uncolorWorkarea();
var i:byte;

begin
	for i:=1 to 11 do
		colorHLine(0,i,20,0);
end;

procedure clearWorkarea();
begin
	box(4,2,16,9,0);
end;

procedure clearStatusBar();
begin
	fillchar(@statusBar,20,0);
end;

procedure clearModuleBar();
begin
	fillchar(@moduleBar,20,0);
end;

procedure clearModule();
begin
	fillchar(@moduleBar,220,0);
end;
