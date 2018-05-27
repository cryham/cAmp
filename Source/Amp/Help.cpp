#include "header.h"
#include "Amp.h"


void cAmp::DrawHelpText()									//  help  //
{
	CFont* cf = cfont[view.cfH];
	int Fy = cf->Fy;
	int y = /*(yBpt+100 > view.ySize) ? yBpl :*/ yB_pt+Fy,
		x1,x2, xt, t = 20;
	cf->Fs[' '] = cf->Fs['0']/2;
	cf->dir = 1;

	#define  sc(a)		cf->StrCopy(a)
	#define  wr(x,y)	cf->Write(x,y)
	#define  wR(x,y)	cf->Write(x,y);  cf->Write(x,y)
	#define  clr(r,g,b)  cf->Fclr = D3DXCOLOR(r,g,b,1)
	#define  X(a1,a2)  x1 = a1;  xt = x1+t;  x2 = x1 + a2;

	clr(0.7,0.7,0.7);    sc("Keyboard help");  wr(20,y);  //y+=Fy;
	clr(0.8,0.8,0.9);    cf->Format("Page: %s %d/%d %s", hlpPg==0?"  ":"<", hlpPg+1,HelpPages, hlpPg==HelpPages-1?"  ":">");
	wr(view.xSize-110,y);  y+=Fy*3/2;	clr(1,1,1);
	
	sc(HPnames[hlpPg]);  int o=cf->GetWidth(); //center
	wr(view.xSize/2-o/2,y);  y = yB_pt+5*Fy;
	#define dark()  clr(0.75,0.85,0.95)
	
	switch (hlpPg)
	{
	case 0:  X(20,35);		//  Player basic
		clr(0.3,0.7,0.7);
		sc("Enter, RMB    Play from cursor  |>");	wR(x1,y);  y+=Fy*4/2;  clr(0.7,0.95,1);
		sc("Z");	wr(x1,y);  sc("Play     |>");	wr(x2,y);  y+=Fy;
		sc("X");	wr(x1,y);  sc("Pause   ||");	wr(x2,y);  y+=Fy;
		sc("C");	wr(x1,y);  sc("Stop    []");	wr(x2,y);  y+=Fy*3/2;  clr(0.95,1,1);
		
		sc("A");	wr(x1,y);  sc("Previous  |<");	wr(x2,y);  y+=Fy;
		sc("S");	wr(x1,y);  sc("Next        >|");	wr(x2,y);  y+=Fy;	dark();
			sc("alt- dec/inc playing track rating");	wr(xt,y);  y+=Fy;
			sc("ctrl- play prev/next playlist");		wr(xt,y);  y+=Fy*5/2;  clr(0.9,0.95,1);

		X(20,80,40);
		sc("Left   Q");		wr(x1,y);  sc("Backward  <<");	wr(x2,y);  y+=Fy;
		sc("Right  W");		wr(x1,y);  sc("Forward    >>");	wr(x2,y);  y+=Fy;    dark();
			sc("shift- small  ctrl- big");					wr(xt,y);  y+=Fy*2;  clr(0.8,0.8,1);
		X(20,100,40);
		sc("alt-Left   Q");	wr(x1,y);  sc("Volume Down  v");wr(x2,y);  y+=Fy;
		sc("alt-Right  W");	wr(x1,y);  sc("Volume Up  ^");	wr(x2,y);  y+=Fy*5/2;  clr(0.95,0.8,1.0);
		
		X(20,35,40);
		sc("R");	wr(x1,y);  sc("Repeat All    @A");		wr(x2,y);  y+=Fy;
		sc("1 T");	wr(x1,y);  sc("Repeat One  @1");		wr(x2,y);  y+=Fy*5/2;  clr(0.6,0.8,1.0);

		sc("F1");	wr(x1,y);  sc("show/hide Help");		wr(x2,y);  y+=Fy*2/2;
		x2=x1+60;	clr(0.3,0.6,0.8);
		sc("ctrl-F1");	wR(x1,y);  sc("Global Keys config");	wR(x2,y);  y+=Fy;
		sc("ctrl-F2");	wR(x1,y);  sc("Player Settings");		wR(x2,y);  y+=Fy;
		break;

	case 1:  t=15; X(20,50);		//  Mouse
		clr(0.4,0.8,0.8);
		sc("Left");									wR(x1,y);  y+=Fy;  clr(1,1,1);
		sc("at player-  change track position (seek)");	wr(xt,y);  y+=Fy*3/2;
		sc("at tabs-  change Tab");					wr(xt,y);  y+=Fy;
			sc("alt-  move Tab");					wr(xt+t,y);  y+=Fy*2/2;  dark();
			sc("dn/up buttons - change offset");	wr(xt+t,y);  y+=Fy;
			sc("ctrl- tab rows  shift- tab cols");	wr(xt+t,y);  y+=Fy*2;  clr(1,1,1);

		sc("at playlist-  pick track (cursor)");	wr(xt,y);  y+=Fy;  dark();
			sc("ctrl-  select/unselect one");		wr(xt+t,y);  y+=Fy;  clr(0.6,0.8,1.0);
			sc("shift-  select range");				wr(xt+t,y);  y+=Fy;
			sc("shift-ctrl-  unselect range");		wr(xt+t,y);  y+=Fy*2/2;  clr(1,0.9,0.8);
			sc("alt-  Move track/selection");		wr(xt+t,y);  y+=Fy*4/2;  clr(0.4,0.7,1.0);
			//  slider pls |

		sc("Right");								wR(x1,y);  y+=Fy;  clr(1,1,1);
		sc("at playlist-  Play track  |>");			wr(xt,y);  y+=Fy;  dark();
		sc("at player-   change rating");			wr(xt,y);  y+=Fy*3/2;  clr(0.4,0.6,1);
			sc("shift-         Move Window +");		wR(xt,y);  y+=Fy;
			sc("shift-ctrl-  Resize Window []");	wR(xt,y);  y+=Fy*5/2;  clr(0.7,0.6,1.0);
		
		sc("Wheel");								wR(x1,y);  y+=Fy;    clr(0.8,0.8,1);
		sc("at player-  change volume  ^v");		wr(xt,y);  y+=Fy;
		sc("at playlist-  scroll");					wr(xt,y);  y+=Fy;    dark();
			sc("shift- less  ctrl- more");			wr(xt+t,y);  y+=Fy*2;  clr(0.7,0.5,0.5);

		sc("Middle");								wR(x1,y);			 //clr(1,0.9,0.8);
		sc("joy scroll list");						wR(x2,y);  y+=Fy;    clr(0.9,0.8,1);
			sc("shift- change Visualization height");	wr(xt,y);  y+=Fy*2;  clr(0.35,0.5,0.8);
		break;

	case 2:  X(20,80);		//  Playlist basic
		sc("Up,Down");		wr(x1,y);
		sc("move cursor,  ctrl- x8");			wr(x2,y);  y+=Fy;	dark();
			sc("alt- move 1 track up/dn");		wr(xt,y);  y+=Fy;
			sc("alt-shift- to Top  alt-ctrl- to End");	wr(xt,y);  y+=Fy*3/2;	clr(1,1,1);
	
		sc("PgUp,PgDn");	wr(x1,y);
		sc("move page/4,  ctrl- page");						wr(x2,y);  y+=Fy;		dark();
			sc("alt- offset page  alt-ctrl- offset x8");	wr(xt,y);  y+=Fy*3/2;	clr(1,1,1);
		
		sc("Home,End");		wr(x1,y);
		sc("prev/next dir");						wr(x2,y);  y+=Fy;		dark();
			sc("shift- move to begin/end of dir");	wr(xt,y);  y+=Fy;		
			sc("ctrl- of view  alt- of list");		wr(xt,y);  y+=Fy*5/2;	clr(1,1,1);

		X(20,80);  clr(0.85,0.8,1.0);
		sc("Space");		wr(x1,y);  sc("Unselect");					wr(x2,y);  y+=Fy;
		sc("Backspace");	wr(x1,y);  sc("move cursor to playing");	wr(x2,y);  y+=Fy;
			sc("ctrl- also change tab and center");	wr(xt,y);  y+=Fy*4/2;  clr(1,1,1);

		X(20,60);  clr(1,0.9,0.8);
		sc("Delete");		wr(x1,y);  sc("Delete track/selection");	wr(x2,y);  y+=Fy;  clr(1,0.8,0.6);
			sc("shift-");	wr(xt,y);  sc("Delete from Disk");			wr(x2,y);  y+=Fy;  clr(1,0.7,0.85);
			sc("ctrl-");	wr(xt,y);  sc("Clear playlist");			wr(x2,y);  y+=Fy*4/2;
	
		clr(0.7,0.7,1.0);
		sc("Insert");		wr(x1,y);
		sc("alt- Insert Directory at cursor");		wr(x2,y);  y+=Fy;		dark();
			sc("shift- at Top   ctrl- at End");		wr(xt,y);  y+=Fy*4/2;	clr(1,1,1);

		clr(0.4,0.63,0.85);
		sc("Drop  at playlist - insert at cursor");	wR(x1,y);  y+=Fy;  //dark();
			sc("shift- at Top   ctrl- at End");		wR(xt,y);  y+=Fy*2;	clr(1,1,1);
		break;

	case 3:  X(20,55);		//  Playlist advanced
		clr(0.25,0.2,0.3);
		sc("Num"); wR(x1+20,y);  clr(0.6,0.5,0.7);  y+=Fy;
		sc("-   -");	wR(x1,y);	sc("dec track Rating");		wR(x2,y);  y+=Fy;
		sc("=   +");	wR(x1,y);	sc("inc track Rating");		wR(x2,y);  y+=Fy;	clr(0.9,0.7,0.9);
			sc("at playing cursor  ctrl- at cursor");			wr(xt,y);  y+=Fy*3/2;	clr(0.7,0.5,0.8);
		sc("[    /");	wR(x1,y);	sc("Dec rating Filter (lower)");	wR(x2,y);  y+=Fy;
		sc("]    *");	wR(x1,y);	sc("Inc rating Filter");	wR(x2,y);  y+=Fy;	clr(0.9,0.7,0.9);
			sc("ctrl- upper Filter");							wr(xt,y);  y+=Fy*4/2;	clr(0.7,0.5,0.8);

		X(20,40);  clr(0.3,0.65,0.75);
		sc("\\");	wR(x1,y);	sc("inc Bookmark at cursor");	wR(x2,y);  y+=Fy;  clr(0.2,0.55,0.65);
		sc("  ctrl- dec   alt- playlist (tab)");		wR(x1,y);	y+=Fy*3/2;  clr(0.3,0.65,0.75);
		sc("/  .");	wR(x1,y);	sc("next/prev Bookmark in playlist or");	wR(x2,y);  y+=Fy;  clr(0.45,0.7,0.4);
		sc("next/prev Search result, when showing");	wR(x2,y);  y+=Fy*2;  clr(1,1,1);

		sc("D");	wr(x1,y);  clr(0.8,0.9,0.9);
		sc("Duplicate track");							wr(x2,y);  y+=Fy;		dark();
			sc("shift- at Top   ctrl- at End");			wr(xt,y);  y+=Fy*5/4;	clr(1,1,1);
		sc("H");	wr(x1,y);	sc("Hide directory -");	wr(x2,y);  y+=Fy;
		sc("J");	wr(x1,y);	sc("Show directory +");	wr(x2,y);  y+=Fy*2;
		break;

	case 4:  X(20,50);		//  Tabs, visual
		sc("Tab `");wr(x1,y);
		sc("Next/prev playlist Tab");			wr(x2,y);  y+=Fy;	dark();
			sc("ctrl-  row down/up");			wr(xt,y);  y+=Fy;	clr(0.7,0.75,0.8);
			sc("shift-  offset row down/up");	wr(xt,y);  y+=Fy;
			sc("ctrl-shift-  offset 1");		wr(xt,y);  y+=Fy*2;	clr(1,1,1);
		
		sc("N");	wr(x1,y);
			sc("add New Tab after current");		wr(x2,y);  y+=Fy;  dark();
			sc("ctrl- at End  shift- at Begin");	wr(xt,y);  y+=Fy;
			sc("alt- before current");				wr(xt,y);  y+=Fy*4/2;  clr(0.3,0.55,0.8);

		sc("F2");	wR(x1,y);  sc("Rename Tab");	wR(x2,y);  y+=Fy;  clr(1.0,0.85,0.75);
		sc("F8");	wr(x1,y);  sc("Close Tab");		wr(x2,y);  y+=Fy*5/2;  clr(0.7,0.7,0.8);

		X(20,35);			//  Player visual
		sc("O P");	wR(x1,y);  sc("prev/next Visualization");	wR(x2,y);  y+=Fy*3/2;  clr(0.8,0.9,1);
		
		sc("3");	wr(x1,y);  sc("show path, filename info");	wr(x2,y);  y+=Fy*4/2;	clr(0.6,0.75,0.9);
		
		sc("2");	wr(x1,y);  sc("change dir/path display");	wr(x2,y);  y+=Fy;
		sc("0");	wr(x1,y);  sc("draw rating on slider");		wr(x2,y);  y+=Fy*3/2;	clr(0.8,0.7,0.9);
		
		sc("4");	wr(x1,y);  sc("all playlists info");		wr(x2,y);  y+=Fy;
		sc("5");	wr(x1,y);  sc("filtered playlist info");	wr(x2,y);  y+=Fy*4/2;

		X(20,50);	clr(0.65,0.7,0.75);
		sc("I");	wr(x1,y);  sc("Frames per second display");		wr(x2,y);  y+=Fy;
		sc("Ctr-I");wr(x1,y);  sc("Time colors test");		wr(x2,y);  y+=Fy;	clr(1,1,1);
		break;

	case 5:  X(20,50);		//  Player functions
		sc("F4");	wr(x1,y);	sc("Save playlist (quick)");	wr(x2,y);  y+=Fy;
		sc("F5");	wr(x1,y);	sc("Reload playlist");			wr(x2,y);  y+=Fy;
		sc("ctrl-F5");	wr(x1,y);	sc("open playlist");		wr(x2,y);  y+=Fy*4/2;  clr(0.9,0.8,1);
		
		sc("F6");	wr(x1,y);	sc("Re-Insert directory at cursor");	wr(x2,y);  y+=Fy;	dark();
			sc("shift- at Top   ctrl- at End");				wr(xt,y);  y+=Fy*3/2;	clr(0.8,0.9,1);
		sc("U");	wr(x1,y);	sc("Update playlist (times)");	wr(x2,y);  y+=Fy*2;

		clr(0.7,0.55,0.8);
		sc("F10");	wR(x1,y);	sc("apply rating in playlist and save");	wR(x2,y);  y+=Fy;  clr(0.8,0.6,0.8);
		sc("F9");	wr(x1,y);	sc("apply rating to cur filename");			wr(x2,y);  y+=Fy;  //clr(0.8,0.6,0.8);
			sc("shift- in dir  ctrl- in playlist");							wr(x1+t,y);  y+=Fy*2;

		clr(0.45,0.7,0.4);
		sc("F3  F");	wR(x1,y);  sc("Search name");	wR(x2,y);  y+=Fy;
			sc("shift-  hide/show results");			wR(x1+t,y);  y+=Fy*2;	 clr(1,1,1);

		clr(0.6,0.8,0.7);
		sc("alt 1..8");  wr(x1,y);  sc("load View 1..8");	wR(x2+10,y);  y+=Fy;
		sc("ctrl 1..8");  wr(x1,y);  sc("save View 1..8");	wR(x2+10,y);  y+=Fy;
			sc("window position,size,visualization etc.");  wr(xt,y);  y+=Fy*2;

		clr(0.8,0.8,0.6);
		sc("F11");	wr(x1,y);  sc("Reload resources, check window");	wr(x2,y);  y+=Fy*2;
		
		clr(0.75,0.75,0.85);
		sc("F12");	wr(x1,y);  sc("Copy selected to other path");	wr(x2,y);  y+=Fy;
			sc("shift-  whole playlist   ctrl- all   alt- abort");	wr(xt,y);  y+=Fy*2;	 clr(1,1,1);
		break;
	}
}
