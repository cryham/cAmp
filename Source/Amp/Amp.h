#pragma once
#include "..\cD3\D3.h"
#include "..\cD3\Snd.h"
#include "..\List\List.h"
#include "..\cD3\GuiSys.h"
#include "Amp_def.h"


struct SetPls  // load only
{
	std::string name;
	byte bokm;

	SetPls() : bokm(0) {  }
};


class cAmp : public cSnd
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
	void DrawPlsText(), DrawPlstRect(), DrawSlider(), DrawHelpText();
	void DrawAmpRect(), DrawAmpText(),  DrawKeysRect(), DrawKeysText();

	//  set
	void SetLoad(), SetSave(), SetDefault(),  ClrLoad(), ClrSave();
	std::string sExtAll, sModAll;
		
	//  tabs
	void tabPrev(bool row, bool ofs), tabNext(bool row, bool ofs);
	void tabNew(int m),tabClose(), tabMove(int n), PlsOpen();

	//  mouse
	int xm,ym,xms,yms, xMs,yMs,yMd,yMFvi, xLs,yLs;
	bool bLsl, bMInWnd;  float mti,mtiv;
	
	// view
	const static int MaxViews = 8;
	CViewSet views[MaxViews];
	void ViewSave(int v),ViewLoad(int v),ViewUpd();
	
	
	//  Keys hook  ----
	/*HHOOK hKbd;*/  bool bHKeys;
	bool KeysHook(byte vk, byte sc, byte fl);
	void KbdInit(), KbdDest();

	CKey vKeys[FU_ALL];
	int iPriority;	void GuiOff();
	int yPickKey;  bool bPickingKey, btnKeysOk,btnKeysOn;


	///  play  ------------------------------------------------
	char sPlInf[100];  //file info

	//----  play control
	bool bWasPlay, bRepAll,bRep1;
	float fVol;
	
	double tmPl, tmTot, lastPos;  //play, total time, relpos
	double time;  //save time
	bool bNextPrev;  //last dir for next>

	bool Play(bool get=true,bool fget=false), PlayFrom(double t);
	void Pause(),Stop(), Prev(),Next(), PlayPrevTab(),PlayNextTab();

	void repAll(),rep1();  // toggle repeat

	//  seek  ---
	int aSpdSeek, aSpdVol;  // all
	int iSpdSeek, iSpdVol;  // cur

	struct SpdSeek  // seek
	{
		int add, rnd, perc;
		SpdSeek() : add(0), rnd(0), perc(0)
		{	}
	};  // const sec add, random sec, percent of track time

	struct SpdSeek3
	{	SpdSeek s[3];  // 0 slow, 1 norm, 2 fast
	};
	struct SpdVol3
	{	float v[3];
		SpdVol3()
		{	v[0]=v[1]=v[2] = 0.f;  }
	};
	
	std::vector<SpdSeek3> vSpdSeek;
	std::vector<SpdVol3> vSpdVol;

	void chPos(bool back, bool slow, bool fast), chPosAbs(double pos);
	void chVol(bool back, bool slow, bool fast);  // seek/change position, volume
	

	//  System  ----
	HANDLE thrIns;
	std::string sInsPath;
	BOOL BrowseDir(LPCSTR sCurrent, LPSTR sPath);
	LRESULT OnDropFiles(WPARAM wp, LPARAM lp);

	
	//  editbox
	float fTi;  //dt
	int ed/*mode*/, hlpPg;
	//  edit name
	char sed[MP],srch[MP];
	int ied;  int* Lxm;  // name long x max


	///  PlayLists  ------------------------------------------------
	std::vector<CList*> vPlst;
	int nTabMov;
	CList* pls,*plsPl,*plsSel;  //cur, playing, copy from
	int plsId,plsPlId,plsSelId;

	void plsChg(int clrSel=0), plsPlChg(int id);
	void updSelId(int clear=0), clrSelId();

	void LoadPlsts(), DestPlsts();  int cntrPls;  //for new
	std::vector<SetPls> vSetPls;  //set load

	void RenameAll(int type), DoSearch();
	bool bShowSrch, bAltOld;  int iSrchAll;
	
	//  Queue  ---
	CList* plq;
	void AddToQueue();
	
	//todo: history..
	
	//  copy  ---
	std::string copyPath;
	int copyType;
	HANDLE thrCopy;
	bool bThrCopy,bCopyAbort;
	float copyMBCur,copyMBAll;
	void CopyThread(), destCopyThr();
	void CopyAll(), CopyPls(bool b, CList* l);


	///  colors, set  ------------------------------------------------
	std::vector<D3DXCOLOR> vRclr,vTclr;  //rating,time
	D3DXCOLOR rtx;  //rate tex uv
	int tmClrMode;
	
	//  dimensions  B=begin E=end W=width H=height
	float xW_pos;  // position marker
	int yB_fi,  // file info
		yB_vis,yE_vis,  // visualization
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
