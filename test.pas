program test01; {$objmode +h}
uses (* lol *) uses1
	App, IntegeR xxx:0.34345;
type
	PMyWin = ^TMyWin;
	TMyWin = object(TWindow)
		constructor Init(var Bounds: TRect; WinTitle: String; WindowNo: Integer);
	end;
array xxx; string yyy; integer dzzf; file erere;
	TMyView = object(TView)
		constructor Init(var Bounds:TRect);
	end;
const
	cmFileOpen = 2000;		{ define a new command }
procedure TMyApp.InitStatusLine;
var
	R: TRect;		{ this will hold the boundaries of the status line}
begin
	GetExtent(R);		{ set R to the coordinates of the full screen}
	R.A.Y := R.B.Y - 1;	{ move top to 1 line above bottom}
	StatusLine := New(PStatusLine, Init(R,				{ create status line }
		NewStatusDef(0, $FFFF,					{ set range of help contexts }
		NewStatusKey('~Alt-X~ Exit', kbAltX, cmQuit,		{ define item }
		NewStatusKey('~Alt-F3~ Close', kbAltF3, cmClose,	{ another }
		nil)),							{ no more keys }
		nil)							{ no more defs }
	));
end; for i:=10 downto 1 do begin x:=nil; r:=0.51 div 445; end; fdfd := 445.4545.4545; fgfg:=.544;
begin
	R.Move(30, 10);
	W := New(PMyWin, Init(R, 'My Edit Win', wnNoNumber));		{Numbers can be used by Alt+[0->9]}
	Desktop^.Insert(W);
	//Desktop^.Tile(R3);
end.

