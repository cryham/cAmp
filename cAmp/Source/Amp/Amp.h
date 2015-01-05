#pragma once
#include "..\cD3\D3.h"
#include "..\cD3\Osc.h"
#include "..\List\List.h"
#include "..\cD3\GuiSys.h"
#include "Amp_def.h"


class cAmp : public cOsc
{
public:
	cAmp();  ~cAmp();
	HANDLE hpr;  bool run;

	///  main  ------------------------------------------------
	bool Begin();
	void End(), ReInit(bool reset);
	void Graph(), GraphAmp();

	bool Keys(WPARAM k), KeysEdit(bool shift, WPARAM k);
	void Mouse(), Wheel(int nWheel);

	//  Draw  --------
	bool bDrawPlst,bDrawPlst2;
	void DrawPlsText(), DrawPlstRect(), DrawSlider(), DrawHelpText(), //draw
		DrawAmpRect(), DrawAmpText(),  DrawKeysRect(), DrawKeysText();

	//  set
	void SetLoad(), SetSave(),  ClrLoad(), ClrSave();
		
	void tabPrev(bool row, bool ofs), tabNext(bool row, bool ofs), // Tabs
		tabNew(int m),tabClose(), tabMove(int n), PlsOpen();

	//  mouse
	int xm,ym,xms,yms, xMs,yMs,yMd,yMFvi, xLs,yLs;
	bool bLsl, bMInWnd;  float mti,mtiv;
	
	// view
	const static int MaxViews = 8;
	CViewSet views[MaxViews];
	void ViewSave(int v),ViewLoad(int v),ViewUpd();
	
	//  Keys hook  ----
	/*HHOOK hKbd;*/  bool bHKeys;
	bool KeysHook(DWORD vk,DWORD sc,DWORD fl);
	void KbdInit(), KbdDest();

	CKey vKeys[FU_ALL];
	int iPriority;	void GuiOff();
	int yPickKey;  bool bPickingKey, btnKeysOk,btnKeysOn;


	///  play bass  ------------------------------------------------
	char sPlInf[100];  //file info

	//----  play control
	bool bWasPlay, bRepAll,bRep1;
	float fVol;
	
	double tmPl, tmTot, lastPos;  //play, total time, relpos
	double time; //save time

	bool Play(bool get=true,bool fget=false), PlayFrom(double t);
	bool bNextPrev;  //last dir for next>
	void chPos(double add),chPosAbs(double pos), chVol(float add),
		Pause(),Stop(), Prev(),Next(), PlayPrevTab(),PlayNextTab(),
		repAll(),rep1();  //toggle repeat

	//  System  ----
	HANDLE thrIns;
	char sInsPath[MP];
	BOOL BrowseDir(LPCSTR sCurrent, LPSTR sPath);
	LRESULT OnDropFiles(WPARAM wp, LPARAM lp);

	
	//  editbox
	float fTi;  //dt
	int ed/*mode*/, hlpPg;
	//  edit name
	char sed[MP],srch[MP];
	int ied;  int* Lxm;  // name long x max


	///  PlayLists  ------------------------------------------------
	std::vector<CList*> vPlst;  int nTabMov;
	CList* pls,*plsPl,*plsSel;  //cur, playing, copy from
	int plsId,plsPlId,plsSelId;
	void plsChg(int clrSel=0), plsPlChg(int id), updSelId(int clear=0), clrSelId();

	void LoadPlsts(), DestPlsts();  int cntrPls;  //for new
	std::vector<std::string> vPlsNames;  //set load
	void RenameAll(int type), DoSearch();
	bool bShowSrch, bAltOld;  int iSrchAll;


	///  colors, set  ------------------------------------------------
	std::vector<D3DXCOLOR> vRclr,vTclr;  //rating,time
	D3DXCOLOR rtx;  //rate tex uv
	int tmClrMode;
	
	//  dimensions  B=begin E=end W=width H=height
	float xW_pos;  // position marker
	int yB_fi,  // file info
		yB_vis,yE_vis,  // visualisation
		yB_pos,yE_pos, yE_pl_btn,  // posbar, btns
		//  pls tabs, size, btns
		yB_pt,yE_pt, xW_pt,yH_pt, xW_pt_btn,
		// pls tracks  lines, |sliderW,mW, xTime
		yB_pli, yB_pl,yE_pl, yH_pl,yL_pl, /*xWplS,*/xW_plSm,xTm,
		// gui checks
		xB_gc,xB_gck,yB_gc, xW_gc,yH_gc;

	void UpdDim(float rfrFq=-1.f);
	//  stats
	DWORD aaD,aaF, aaSi, aaTm;
	void updAllInfo();
	
	
	//  gui
	GuiSys gui[GuiPages];  int gpg;  // page
	void GuiReInit(), GuiCreate();
	void GuiEvent(GuiCtrl* ctrl, int IDc, eGuiEvent evt);
	static void GuiEvCall(void* pInst, GuiCtrl* ctrl, int IDc, eGuiEvent evt);
	///------------------------------------------------
};
