#pragma once
#include "..\cD3\D3.h"
#include "..\cD3\Osc.h"
#include "..\List\List.h"
#include "..\cD3\GuiSys.h"


//  Gui pages
const int GuiPages = 3;

//  help pages
#define  HP  6
const static char HPnames[HP][20]={
	"Player","Mouse","Playlist basic","Playlist advanced","Tabs, visual","Player functions"};

//  global keys
enum ePlrFuns {
	FU_PLAY=0,FU_PAUSE,FU_STOP, FU_PREV,FU_NEXT, FU_PREVPLS,FU_NEXTPLS, 
	FU_BACK,FU_FORW, FU_BACK1,FU_FORW1, FU_BACK2,FU_FORW2,
	FU_VOLDN,FU_VOLUP, /*FU_VOLDN1,FU_VOLUP1,*/ FU_VOLDN2,FU_VOLUP2,
	FU_RATINC,FU_RATDEC, FU_REPONE, FU_ALL};

const char cswFuns[FU_ALL][24] = {
	"Play  |>","Pause ||","Stop  []",  "Prev  |<","Next  >|",  "Prev < playlist","Next > playlist",
	"Backward <<","Forward  >>", "Bck  << small","Frw >> small", "Bck  << big","Frw >> big",
	"Volume Down","Volume Up ^", /*"Vol Dn small","Vol Up small",*/ "Vol Dn big","Vol Up big", 
	"Inc + Rating","Dec - Rating", "Repeat 1 One"};

const int cyFadd[FU_ALL] = {0,0,2, 0,1, 0,2, 0,1,0,1,0,1, 0,1,0,2, 0,1,0};


class CKey  {  public:
	BYTE vk,sc, mod;
	bool on;  int vis;
	CKey() : vk(0),sc(0),mod(0), on(false),vis(0) {}
};


class cAmp : public cOsc
{
public:
	cAmp();  ~cAmp();
	HANDLE hpr;  bool run;

	///  main  ------------------------------------------------
	bool Begin(),  Keys(WPARAM k), KeysEdit(bool shift, WPARAM k), bDrawPlst,bDrawPlst2;
	void End(), ReInit(bool reset),  Graph(), GraphAmp(),
		DrawPlsText(), DrawPlstRect(), DrawSlider(), DrawHelpText(), //draw
		DrawAmpRect(), DrawAmpText(),  DrawKeysRect(), DrawKeysEditsText(),
		SetLoad(), SetSave(),  ClrLoad(), ClrSave(), //set
		Wheel(int nWheel), Mouse();
	void tabPrev(bool row, bool ofs),tabNext(bool row, bool ofs), // Tabs
		tabNew(int m),tabClose(),tabMove(int n), tabOpen();

	//  mouse
	int xm,ym,xms,yms, xMs,yMs,yMd,yMFvi, xLs,yLs;
	bool bLsl, bMInWnd;  float mti,mtiv;
	
	// view
	CViewSet views[MaxViews];
	void ViewSave(int v),ViewLoad(int v),ViewUpd();
	
	//  Keys hook
	/*HHOOK hKbd;*/  bool bHKeys;
	bool KeysHook(DWORD vk,DWORD sc,DWORD fl);
	void KbdInit(), KbdDest();

	CKey vKeys[FU_ALL];
	int iPriority;	void GuiOff();
	int yPickKey;  bool bPickingKey, btnKeysOk,btnKeysOn;


	///  play bass  ------------------------------------------------
	char sPlInf[100];  //file info

	//----  play control
	bool bWasPlay, bRepAll,bRep1;  float fVol;
	
	double tmPl, tmTot, lastPos;  //play, total time, relpos
	double time; //save time

	bool Play(bool get=true,bool fget=false), PlayFrom(double t), bNextPrev;//dir>
	void chPos(double add),chPosAbs(double pos), chVol(float add),
		Pause(),Stop(), Prev(),Next(), PlayPrevTab(),PlayNextTab(),
		repAll(),rep1();  //toggle
	//----

	char sInsPath[MP];  HANDLE thrIns;
	BOOL BrowseDir(LPCSTR sCurrent, LPSTR sPath);
	LRESULT OnDropFiles(WPARAM wp, LPARAM lp);
	
	float fTi;  //dt
	int ed/*mode*/, hlpPg;
	//  edit name
	char sed[MP],srch[MP];  int ied;  int* Lxm;//name long


	///  PlayLists  ------------------------------------------------
	vector<CList*> vPlst;  int nTabMov;
	CList* pls,*plsPl,*plsSel;  //cur, playing, copy from
	int plsId,plsPlId,plsSelId;
	void plsChg(int clrSel=0), plsPlChg(int id), updSelId(int clear=0), clrSelId();

	void LoadPlsts(), DestPlsts();  int cntrPls;  //for new
	vector<char*> vPlsNames;  //set load
	void RenameAll(int type), DoSearch();
	bool bShowSrch, bAltOld;  int iSrchAll;


	///  colors, set  ------------------------------------------------
	vector<D3DXCOLOR> vRclr,vTclr;  //rating,time
	D3DXCOLOR rtx;  //rate tex uv
	int tmClrMode;
	
	//  dims
	float xWpo;  // pos mrkr
	int yBfi, yBvi,yEvi,/*yFvi,*/ yBpo,yEpo, yEplbt,  // vis, posbar, btns
		yBpt,yEpt, /*xNpt,yNpt,*/ xWpt,yHpt, xWptbt,  // pls tabs, num, dim1, btns
		// pls tracks  begin,end, height, lines, |sliderW,mW, xTime
		yBpli,yBpl,yEpl, yHpl,yLpl, /*xWplS,*/xWplSm,xTm,
		xBgc,xBgck,yBgc, xWgc,yHgc;  // gui checks

	void UpdDim(float rfrFq=-1.f);
	//  stats
	DWORD  aaD,aaF, aaSi, aaTm;  void updAllInfo();
	
	//  gui
	GuiSys gui[GuiPages];  int gpg;  // page
	void GuiReInit(), GuiCreate();
	void GuiEvent(GuiCtrl* ctrl, int IDc, eGuiEvent evt);
	static void GuiEvCall(void* pInst, GuiCtrl* ctrl, int IDc, eGuiEvent evt);
	///------------------------------------------------
};
