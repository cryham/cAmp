#include "header.h"

#include "Amp.h"


void cAmp::DrawHelpText()									//  help  //
{
	CFont* cf = cfont[view.cfH];
	int Fy = cf->Fy;
	int y = /*(yBpt+100 > view.ySize) ? yBpl :*/ yB_pt+Fy,
		y1 = y, x1,x2,x3;
	cf->Fs[' '] = cf->Fs['0']/2;
	cf->dir = 1;

	#define  sc(a)		cf->StrCopy(a)
	#define  wr(x,y)	cf->Write(x,y)
	#define  wR(x,y)	cf->Write(x,y);  cf->Write(x,y)
	#define  clr(r,g,b)  cf->Fclr = D3DXCOLOR(r,g,b,1)

    clr(0.7,0.7,0.7);    sc("Keyboard help");  wr(20,y);  //y+=Fy;
    clr(0.8,0.8,0.9);    cf->Format("Page: %s %d/%d %s", hlpPg==0?"  ":"<", hlpPg+1,HelpPages, hlpPg==HelpPages-1?"  ":">");
    wr(view.xSize-110,y);  y+=Fy*3/2;	clr(1,1,1);
    
    sc(HPnames[hlpPg]);  int o=cf->GetWidth(); //center
    wr(view.xSize/2-o/2,y);  y = yB_pt+5*Fy;
    #define dark()  clr(0.75,0.85,0.95)
	
	switch (hlpPg)
	{
    case 0:  x1=20; x2=x1+35; x3=x2+40;  y1 = y;  //  Player basic
		sc("Enter, RMB   Play from cursor |>");		wr(x1,y);  y+=Fy*3/2;
		sc("Z");	wr(x1,y);  sc("Play    |>");	wr(x2,y);  y+=Fy;
		sc("X");	wr(x1,y);  sc("Pause  ||");		wr(x2,y);  y+=Fy;
		sc("C");	wr(x1,y);  sc("Stop    []");	wr(x2,y);  y+=Fy*3/2;
		
		sc("A");	wr(x1,y);  sc("Previous  |<");	wr(x2,y);  y+=Fy;
		sc("S");	wr(x1,y);  sc("Next       >|");	wr(x2,y);  y+=Fy;	dark();
		sc("  alt- dec/inc playing track rating");	wr(x1,y);  y+=Fy;
		sc("  ctrl- play prev/next playlist");		wr(x1,y);  y+=Fy*2; clr(1,1,1);

		sc("Left   Q");		wr(x1,y);  sc("Backward  <<");	wr(x3,y);  y+=Fy;
		sc("Right  W");		wr(x1,y);  sc("Forward    >>");	wr(x3,y);  y+=Fy;	dark();
		sc("  shift- small  ctrl- big");					wr(x1,y);  y+=Fy;	clr(1,1,1);
		sc("alt-Left  Q");	wr(x1,y);  sc("  Volume Down");	wr(x3,y);  y+=Fy;
		sc("alt-Right W");	wr(x1,y);  sc("  Volume Up ^");	wr(x3,y);  y+=Fy*4/2;
		
		sc("R");	wr(x1,y);  sc("Repeat All  @A");		wr(x2,y);  y+=Fy;
		sc("1 T");	wr(x1,y);  sc("Repeat One  @1");		wr(x2,y);  y+=Fy*2;  clr(0.94,0.8,1.0);

		sc("F1");	wr(x1,y);  sc("show/hide Help");		wr(x2,y);  y+=Fy*2/2;
		clr(0.4,0.6,0.8);  x2=x1+60;
		sc("ctrl-F1");	wR(x1,y);  sc("Global Keys config");	wR(x2,y);  y+=Fy;
		sc("ctrl-F2");	wR(x1,y);  sc("Player Settings");		wR(x2,y);  y+=Fy;
		break;

    case 1:  x1=20; x2=x1+50;  y1 = y;		//  Mouse
		sc("Left");		wr(x1,y);  y+=Fy;
		sc("  at player-  change track position");	wr(x1,y);  y+=Fy*3/2;
		sc("  at tabs-  change Tab");				wr(x1,y);  y+=Fy;
		sc("    alt-  move Tab");					wr(x1,y);  y+=Fy*2/2;  dark();
		sc("    dn/up buttons - change offset");	wr(x1,y);  y+=Fy;
		sc("    ctrl- tab rows  shift- tab cols");	wr(x1,y);  y+=Fy*2;  clr(1,1,1);

		sc("  at playlist-  pick track");			wr(x1,y);  y+=Fy;	dark();
		sc("    ctrl-  select/unselect one");		wr(x1,y);  y+=Fy;
		sc("    shift-  select range");				wr(x1,y);  y+=Fy;
		sc("    shift-ctrl-  unselect range");		wr(x1,y);  y+=Fy*2/2;	clr(1,1,1);
		sc("    alt-  Move track/selection");		wr(x1,y);  y+=Fy*4/2;
		//  slider pls |

		sc("Right");								wr(x1,y);  y+=Fy;
		sc("  at playlist-  Play track");			wr(x1,y);  y+=Fy;	dark();
		sc("  at player-   change rating");			wr(x1,y);  y+=Fy*3/2;  clr(0.4,0.6,1);
		sc("  shift-         Move Window");			wR(x1,y);  y+=Fy;
		sc("  shift-ctrl-  Size Window");			wR(x1,y);  y+=Fy*2;	clr(1,1,1);
		
		sc("Wheel");								wr(x1,y);  y+=Fy;
		sc("  at player-  change volume");			wr(x1,y);  y+=Fy;
		sc("  at playlist-  scroll");				wr(x1,y);  y+=Fy;	dark();
		sc("    shift- less  ctrl- more");			wr(x1,y);  y+=Fy*2;	clr(1,1,1);

		sc("Middle");								wr(x1,y);
		sc("joy scroll list");						wr(x2,y);  y+=Fy;	clr(0.9,0.8,1);
		sc("  shift- change Visualization height");	wr(x1,y);  y+=Fy*2;	clr(0.35,0.5,0.8);
		break;

    case 2:  x1=20; x2=x1+80;  y1 = y;		//  Playlist basic
		sc("Up,Down");		wr(x1,y);
		sc("move cursor,  ctrl- x8");			wr(x2,y);  y+=Fy;	dark();
		sc("  alt- move 1 track up/dn");		wr(x1,y);  y+=Fy;
		sc("  alt-shift- to Top  alt-ctrl- to End");	wr(x1,y);  y+=Fy*3/2;	clr(1,1,1);
	
		sc("PgUp,PgDn");	wr(x1,y);
		sc("move page/4,  ctrl- page");					wr(x2,y);  y+=Fy;		dark();
		sc("  alt- offset page  alt-ctrl- offset x8");	wr(x1,y);  y+=Fy*3/2;	clr(1,1,1);
		
		sc("Home,End");		wr(x1,y);
		sc("move to begin/end of dir");			wr(x2,y);  y+=Fy;		dark();
		sc("  ctrl- of view  alt- of list");	wr(x1,y);  y+=Fy*4/2;	clr(1,1,1);

		x2=x1+70;  clr(0.85,0.8,1.0);
		sc("Space");		wr(x1,y);  sc("Unselect");					wr(x2,y);  y+=Fy;
		sc("Backspace");	wr(x1,y);  sc("move cursor to playing");	wr(x2,y);  y+=Fy;
		sc("  ctrl- tab too");	wr(x1,y);  y+=Fy*4/2;  clr(1,1,1);

		x2=x1+50;
		sc("Del");			wr(x1,y);  sc("Delete track/selection");	wr(x2,y);  y+=Fy;
		sc("  shift-");		wr(x1,y);  sc("Delete from Disk");			wr(x2,y);  y+=Fy;
		sc("  ctrl-");		wr(x1,y);  sc("Clear playlist");			wr(x2,y);  y+=Fy*4/2;
	
		clr(0.7,0.7,1.0);
		sc("Ins");		wr(x1,y);
		sc("Insert Directory at cursor");		wr(x2,y);  y+=Fy;		dark();
		sc("  shift- at Top   ctrl- at End");	wr(x1,y);  y+=Fy*4/2;	clr(1,1,1);

		clr(0.4,0.6,0.8);
		sc("Drop  at playlist - insert at cursor");	wR(x1,y);  y+=Fy;  //dark();
		sc("  shift- at Top   ctrl- at End");		wR(x1,y);  y+=Fy*2;	clr(1,1,1);
		break;

    case 3:  x1=20; x2=x1+80;  y1 = y;		//  Playlist advanced
		x2=x1+55;  clr(0.6,0.5,0.7);
		sc("-   -"); wR(x1,y);	sc("dec track Rating");		wR(x2,y);  y+=Fy;
		sc("=   +"); wR(x1,y);	sc("inc track Rating");		wR(x2,y);  y+=Fy;	clr(0.9,0.7,0.9);
		sc("  at playing cursor  ctrl- at cursor");			wr(x1,y);  y+=Fy*3/2;	clr(0.7,0.5,0.8);
		sc("[    /");wR(x1,y);	sc("Dec rating Filter (lower)");	wR(x2,y);  y+=Fy;
		sc("]    *");wR(x1,y);	sc("Inc rating Filter");	wR(x2,y);  y+=Fy;	clr(0.9,0.7,0.9);
		sc("  ctrl- upper Filter");		wr(x1,y);  y+=Fy*4/2;	clr(0.7,0.5,0.8);

		x2=x1+40;  clr(0.3,0.65,0.75);
		sc("\\");	wR(x1,y);	sc("toggle Bookmark at cursor");		wR(x2,y);  y+=Fy;  clr(0.2,0.55,0.65);
		sc("  ctrl- level 2  alt- level 3");	wR(x1,y);	y+=Fy;  clr(0.3,0.65,0.75);
		sc("/  .");	wR(x1,y);	sc("next/prev Bookmark in playlist or");	wR(x2,y);  y+=Fy;  clr(0.2,0.85,0.65);
		sc("next/prev Search result, when showing");  wR(x2,y);  y+=Fy*2;  clr(1,1,1);

		sc("D");	wr(x1,y);
		sc("Duplicate track");								wr(x2,y);  y+=Fy;		dark();
		sc("  shift- at Top   ctrl- at End");				wr(x1,y);  y+=Fy*5/4;	clr(1,1,1);
		sc("H");	wr(x1,y);	sc("Hide -> directory");	wr(x2,y);  y+=Fy;
		sc("J");	wr(x1,y);	sc("Show +> directory");	wr(x2,y);  y+=Fy*2;
		break;

    case 4:  x1=20; x2=x1+50;  y1 = y;		//  Tabs, visual
		sc("Tab ~");wr(x1,y);
		sc("Next/prev playlist Tab");			wr(x2,y);  y+=Fy;	dark();
		sc("  ctrl-  row down/up");				wr(x1,y);  y+=Fy;
		sc("  shift-  offset row down/up");		wr(x1,y);  y+=Fy;
		sc("  ctrl-shift-  offset 1");			wr(x1,y);  y+=Fy*2;	clr(1,1,1);
		
		sc("N");	wr(x1,y);
		sc("add New Tab after current");		wr(x2,y);  y+=Fy;		dark();
		sc("  ctrl- at End  shift- at Begin");	wr(x1,y);  y+=Fy;
		sc("  alt- before current");			wr(x1,y);  y+=Fy*4/2;	clr(1,1,1);

		sc("F2");	wr(x1,y);  sc("Rename Tab");	wr(x2,y);  y+=Fy;
		sc("F8");	wr(x1,y);  sc("Close Tab");		wr(x2,y);  y+=Fy*2;

		x1=20; x2=x1+35;  y1 = y;			//  Player visual
		sc("O,P");	wr(x1,y);  sc("prev/next Visualization");		wr(x2,y);  y+=Fy*3/2;
		
		sc("3");	wr(x1,y);  sc("show path,filename info");		wr(x2,y);  y+=Fy;	clr(0.8,0.7,0.9);
		sc("2");	wr(x1,y);  sc("change dir/path display");		wr(x2,y);  y+=Fy*3/2;
		
		sc("4");	wr(x1,y);  sc("all playlists info");		wr(x2,y);  y+=Fy;
		sc("5");	wr(x1,y);  sc("filtered playlist info");	wr(x2,y);  y+=Fy*3/2;

		sc("0");	wr(x1,y);  sc("draw rating on slider");	wr(x2,y);  y+=Fy*3/2;
		
		sc("I");	wr(x1,y);  sc("Fps display");			wr(x2,y);  y+=Fy;	clr(1,1,1);
		break;

    case 5:  x1=20; x2=x1+50;  y1 = y;		//  Player functions
		sc("F4");	wr(x1,y);	sc("Save playlist (quick)");	wr(x2,y);  y+=Fy;
		sc("F5");	wr(x1,y);	sc("Reload playlist");			wr(x2,y);  y+=Fy;
		sc("ctrl-F5");	wr(x1,y);	sc("open playlist");		wr(x2,y);  y+=Fy*4/2;
		
		sc("F6");	wr(x1,y);	sc("Insert selected dir");	wr(x2,y);  y+=Fy;	dark();
		sc("  shift- at Top   ctrl- at End");	wr(x1,y);  y+=Fy*3/2;	clr(1,1,1);
		sc("U");	wr(x1,y);	sc("Update playlist");		wr(x2,y);  y+=Fy*2;

		clr(1.0,0.8,1.0);
		sc("F9");	wr(x1,y);	sc("apply rating to cur filename");			wr(x2,y);  y+=Fy;  clr(0.9,0.7,0.9);
		sc("  shift- in dir  ctrl- in playlist");							wr(x1,y);  y+=Fy;  clr(0.7,0.55,0.8);
		sc("F10");	wR(x1,y);	sc("apply rating in playlist and save");	wR(x2,y);  y+=Fy*2;
		
		clr(0.5,0.7,0.5);
		sc("F3  F");	wR(x1,y);  sc("Search name");	wR(x2,y);  y+=Fy;
		sc("  shift-  hide/show results");				wR(x1,y);  y+=Fy*2;	 clr(1,1,1);

		sc("F11");	wr(x1,y);  sc("Reload resources,check window");	wr(x2,y);  y+=Fy*2;
		
		clr(0.7,0.9,0.8);
		sc("alt 1..8");  wr(x1,y);  sc("load View 1..8");	wR(x2+10,y);  y+=Fy;
		sc("ctrl 1..8");  wr(x1,y);  sc("save View 1..8");	wR(x2+10,y);  y+=Fy;
		sc("  window position,size,visualization etc.");  wr(x1,y);  y+=Fy;
		
		//  alt 1..8 load 
		//ctrl -view 
		break;
	}
}
