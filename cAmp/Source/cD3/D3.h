#pragma once
#include "CFont.h"
#include "View.h"


enum eETex {
	TX_Prev=0, TX_Next, TX_Up, TX_Dn, TX_Checks, TX_LeRi,
	TX_PosBr, TX_PosDk,
	TX_TabC, TX_TabP, TX_TabS,  //_Cur,Play,Sel,Find,Bookm
	TX_BackPlr, TX_BackBtm, TX_BackHelp,
	TX_PlsC, TX_PlsP, TX_PlsS, //TX_PlsF,
	TX_Slid, TX_SliC, TX_SliP, TX_SliF,
	TX_DShow, TX_DHide,
	TX_PlsB1, TX_PlsB2, TX_PlsB3, TX_PlsB4, TX_PlsB5, TX_PlsB6,
	TX_SliB1, TX_SliB2, TX_SliB3, TX_SliB4, TX_SliB5, TX_SliB6,
	TX_Rate1, TX_Rate2, TX_Rate3, TX_Rate4, TX_Rate5,  NumTex };

const int texUV[NumTex+1][4] =  // size x,y, pos x,y on player.png texture, in pixels
{
	96, 23,		0, 20,		// prev
	96, 23,		100, 20,	// next
	16, 64,		220, 0,		// up
	16, 64,		200, 0,		// down
	109, 29,	250, 0,		// checks
	192, 16,	0, 0,		// leri

	288, 14,	0, 150,		// pos_bright
	288, 14,	0, 130,		// pos_dark

	33, 16,		0, 50,		// tab_cur
	33, 13,		40, 50,		// tab_play
	33, 13,		80, 50,		// tab_sel

	16, 96,		260, 170,	// back_plr
	16, 40,		260, 80,	// back_btm
	16, 96,		260, 270,	// back_help

	241, 10,	0, 70,		// pls_cur
	252, 13,	0, 90,		// pls_play
	252, 13,	0, 110,		// pls_sel

	16, 512,	490, 0,		// slider
	16, 24,		250, 40,	// slid_cur
	16, 24,		270, 40,	// slid_play
	16, 24,		290, 40,	// slid_find
	
	14, 14,     262,380,    // dir show+
	14, 14,     262,400,    // dir hide-

	252, 13,	0, 360,		// 1 pls_bookm
	252, 13,	0, 380,		// 2
	252, 13,	0, 400,		// 3
	252, 13,	0, 420,		// 4
	252, 13,	0, 440,		// 5
	252, 13,	0, 460,		// 6

	16, 24,		320, 40,	// 1 slid_bookm
	16, 24,		340, 40,	// 2
	16, 24,		360, 40,	// 3
	16, 24,		380, 40,	// 4
	16, 24,		400, 40,	// 5
	16, 24,		420, 40,	// 6

	252, 13,	0, 180,		// 1 rate
	252, 13,	0, 200,		// 2
	252, 13,	0, 220,		// 3
	252, 13,	0, 240,		// 4
	252, 13,	0, 260,		// 5
	512,512,	0, 0,		// whole tex
};

enum eEffx {
	FX_fft=0, FX_osc, /*FX_ff2,*/ FX_rClr, FX_ALL };

const int NumFnt = 6;  // playlist font sizes  find in dir..


class D3
{
public:
	D3();

	//  log file
	std::ofstream* flog;
	int errCnt;
	void log(std::string s);
	void Err(std::string s);


	Timer Tim;  HHOOK hKbd;
	HINSTANCE hInst;  HWND hWnd;
	CRITICAL_SECTION cs,csNext;
	bool bFltTex;  // hq vis
	
	bool Init(),   Init2(),    Check(),   CreateWnd(), WndCheck();
	void Destroy(),Destroy2(), ScrSize(), MoveWnd();
	
	CViewSet view;  // current
  
	int xScreen,yScreen, xScrMin,yScrMin,xScrMax,yScrMax,  // screen range
		xMpos,yMpos, xMold,yMold, xWclick,yWclick;  // Mouse
	bool bLs,bL, bRs,bR, bMs,bM,  // Buttons
		 shift,ctrl,alt;  // modkeys
	int mod, act, clear, iSize, iResets;

	//  str
	std::string appPath;
	
	//  d3d
	LPDIRECT3D9 pD3d;  PDev pDev;
	D3DPRESENT_PARAMETERS ppar;  D3DDISPLAYMODE dmod;
	
	CFont*  cfont[NumFnt];
	CFont*  cfdig;
	float  fRfq;  // refr freq

	PTex  Tex, visTex,visTex2;
	LPDIRECT3DSURFACE9  surf,bckbuf;
	LPD3DXEFFECT  fx[FX_ALL];
};


LRESULT CALLBACK MsgProc(HWND,UINT,WPARAM,LPARAM);


// utility functions -------------------------------------------------------------------

static inline bool isKeyDown(int vKey)
{
	return GetAsyncKeyState(vKey) != 0;
}

// ---- random ----

static inline float random()
{
	return float(rand())/float(RAND_MAX);
}

static inline float random(float a,float b)
{
	return  (b-a) * float(rand())/float(RAND_MAX) + a;
}
