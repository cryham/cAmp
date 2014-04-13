#pragma once
#include "CFont.h"
#include "View.h"


enum eETex {
	TX_Prev=0, TX_Next, TX_Up, TX_Dn, TX_Checks, TX_LeRi,
	TX_PosBr, TX_PosDk,
	TX_TabC, TX_TabP, TX_TabS,  //_Cur,Play,Sel,Find,Bookm
	TX_BackPlr, TX_BackBtm, TX_BackHelp,
	TX_PlsC, TX_PlsP, TX_PlsS, //TX_PlsF,
		TX_PlsB1, TX_PlsB2, TX_PlsB3,
	TX_Slid, TX_SliC, TX_SliP, TX_SliF,
		TX_SliB1, TX_SliB2, TX_SliB3,
	TX_Rate1, TX_Rate2, TX_Rate3, TX_Rate4, TX_Rate5,  NumTex };
const int NumBtnsLast = TX_LeRi;

const char csTexName[NumTex][20] = {
	"prev","next","up","down", "checks","leri",
	"pos_bright","pos_dark",
	"tab_cur","tab_play", "tab_sel",
	"back_plr", "back_btm", "back_help",
	"pls_cur", "pls_play", "pls_sel",
		"pls_bookm1", "pls_bookm2", "pls_bookm3",
	"slider", "slid_cur", "slid_play", "slid_find",
		"slid_bookm1", "slid_bookm2", "slid_bookm3",
	"rate1","rate2","rate3","rate4","rate5" };

enum eEffx {
	FX_fft=0, FX_osc, /*FX_ff2,*/ FX_rClr, FX_ALL };
enum eEdit {
	ED_Pls=0/*none*/, ED_Help, ED_nTab, ED_nFind, ED_Keys, ED_Set1, ED_ALL };

const int NumFnt = 6;  // playlist font sizes  find in dir..


class D3
{
public:
	D3();

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
	static char s[300], appPath[MAX_PATH];
	
	//  d3d
	LPDIRECT3D9 pD3d;  PDev pDev;
	D3DPRESENT_PARAMETERS ppar;  D3DDISPLAYMODE dmod;
	
	CFont*  cfont[NumFnt];
	CFont*  cfdig;
	float  fRfq;  // refr freq

	PTex  Tex[NumTex], visTex,visTex2;
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
