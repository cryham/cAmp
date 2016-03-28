#include "header.h"
#include "Amp.h"
#include "..\main\App.h"
#include "..\cD3\Graphics.h"
#include "MMSystem.h"
using namespace std;


///* - -  -   -                                -  - -- Begin -- -  -                                 -  -  - - */
DWORD WINAPI VprThread(LPVOID lpParam)
{ 
	cAmp* pA = (cAmp*)lpParam;
	while (pA->run)
	{
		if (pA->TimPr.update())
			pA->Vpr();

		if (pA->view.pr_fq < 80)	Sleep(5);
		else if (pA->view.pr_fq < 120)	Sleep(2);
		else if (pA->view.pr_fq < 200)	Sleep(1);
		else Sleep(0);  //!-
	}
    return 0;
}

bool cAmp::Begin()
{
	ScrSize();
	SetLoad();	// set
	ClrLoad();
	
	SetPriorityClass(GetCurrentProcess(),  // not here-
		iPriority>=2 ? REALTIME_PRIORITY_CLASS :
		(iPriority==1 ? HIGH_PRIORITY_CLASS : NORMAL_PRIORITY_CLASS) );

	InitSnd();
	KbdInit();
	
	LoadPlsts();
	
	if (!D3::Init())  rf
	if (!D3::Init2())  rf

	run = true;
	TimPr.iv = 1.0 / double(view.pr_fq);  ///-
	/**/hpr = CreateThread(NULL,0,VprThread,(LPVOID)this,0,NULL);
	rt
}

void cAmp::ReInit(bool reset)
{
	// chk wnd pos/size
	ScrSize();

	if (view.ySize > yScreen)  view.ySize = yScreen;
	WndCheck();
	MoveWindow(hWnd, view.xPos,view.yPos, view.xSize,view.ySize, true);

	ppar.BackBufferWidth = view.xSize;	ppar.BackBufferHeight = view.ySize;
	ppar.PresentationInterval = view.vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

	// D3D
	D3::Destroy2();

	//if (reset)
	{	HRESULT hr;  iResets++;
		Vd( pDev->Reset(&ppar) );  }

	D3::Init2();
	UpdDim();

	ClrLoad();
	UpdPClr();
	clear = 2;
}

void cAmp::End()
{
	log("----\nDestroy");

	run = false;
	/**/TerminateThread(hpr, 1);

	D3::Destroy2();
	D3::Destroy();

	KbdDest();
	DestSnd();

	SetSave();	// set
	//ClrSave();
	DestPlsts();
}


//  upd all info
void cAmp::updAllInfo()
{
	aaD=0;  aaF=0;  aaSi=0;  aaTm=0;

	for (size_t i=0; i < vPlst.size(); ++i)
	{	CList* pl = vPlst[i];

		aaD += pl->allDirs;  aaF += pl->allFiles;
		aaSi += pl->allSize/1000000;
		aaTm += pl->allTime;
	}
}

//  Update dims
//------------------------------------------------------------------------

void cAmp::UpdDim(float rfrFq)
{
	Graphics::SetupStates(pDev);
	if (view.visH > view.ySize)  view.visH = view.ySize;  //vis max

	//  file info
	yB_fi = 0;	yE_pl_btn = 20;  // btnsH |< >|

	//  visualization
	yB_vis = 16;  //yFvi = 64;
	yE_vis = min(view.ySize, 28 + ((view.eVis!=viNone)? view.visH: 0));
	
	//  pos bar
	yB_pos = yE_vis;  yE_pos = yB_pos+9;
	xW_pos = 0.03f;

	//  pt = playlist tabs
	yB_pt = yE_pos;  /*par+- +2 8pos*/
	xW_pt_btn = 13;  // btnsW up,dn
		xW_pt = (view.xSize - xW_pt_btn)/view.xNpt;
		yH_pt = cfont[view.cfT]->Fy+2;  // <^ dim pls tabs
	yE_pt = yB_pt+ view.yNpt*yH_pt+4;

	//  pl = playlist tracks
	yB_pli = yE_pt;
	yB_pl = yB_pli +cfont[view.cfP]->Fy+2/*yHpli*/;
	yE_pl = view.ySize -cfont[view.cfP]->Fy;
	yH_pl = yE_pl-1-yB_pl;  yL_pl = max(0, yH_pl/cfont[view.cfP]->Fy);
		delete[] Lxm;  Lxm = new int[yL_pl+4];
	yE_pl = yL_pl*cfont[view.cfP]->Fy+yB_pl;  yH_pl = yE_pl-1-yB_pl;

	/*xWplS = 14;*/  xW_plSm = 40;  //|sliderW, mW
	xTm = view.xSize - view.xW_plS-3;  //|
	CList::Lin = yL_pl;

	if (rfrFq > 0.f)
		tmD = int(rfrFq*1.f);	clear=1;
	
	// for..
	cfont[view.cfP]->StrCopy(" 0:00");  xt_ = cfont[view.cfP]->GetWidth();

	//  gui checks pos
	xB_gc = 106;  yB_gc = yB_pt+90;  xW_gc = 18;
	xB_gck = 170;  yH_gc = cfont[view.cfG]->Fy+6; //[0
	
	GuiReInit();
}

cAmp::cAmp()
	:hpr(0), run(0)
	,bDrawPlst(1), bDrawPlst2(0)
	,xm(0),ym(0),xms(0),yms(0), xMs(0),yMs(0),yMd(0)
	,yMFvi(0), xLs(0),yLs(0)
	,bLsl(0), bMInWnd(0), mti(0.f),mtiv(0)
	,yPickKey(-1), bPickingKey(0), btnKeysOk(0),btnKeysOn(0)
	,tmPl(0), tmTot(0), time(0), bNextPrev(1)
	,thrIns(0)
	,fTi(0.01f)
	,ed(ED_Pls), hlpPg(0)
	,ied(0), Lxm(0)
	,nTabMov(0)
	,pls(0),plsPl(0), plsSel(0),plsSelId(-1)
	,bShowSrch(0), bAltOld(0), iSrchAll(0)
	,thrCopy(0), copyType(0)
	,bThrCopy(0), bCopyAbort(0)
	,copyMBCur(0.f),copyMBAll(0.f)
	,tmClrMode(0) //rtx
	,aaD(0),aaF(0), aaSi(0), aaTm(0)
	,gpg(0)
{
	appPath[0]=0;  sPlInf[0]=0;
	sed[0]=0;  srch[0]=0;
	
	SetDefault();
}

void cAmp::SetDefault()
{
	iPriority=1;  nDev=-1;  nFreq=48000;  bHKeys=0;
	bFltTex = 1;  bRecSet = 1;
	iSpdSeek = 1;  iSpdVol = 1;

	bFps=0;  bFInfo=0;  CList::bFilInf=1;
	fVol=1.f;  bRepAll=1; bRep1=0;  cntrPls=0;  sInsPath[0]=0;
	bWasPlay=0;  lastPos=0.0;  plsId=0; plsPlId=0;
}

cAmp::~cAmp()
{
	delete[] Lxm;
}



///  load playlists  * * * * * * * * * * * * * * * * * * * * * * * * * * 

void cAmp::LoadPlsts()
{
	// load last -from set
	if (vSetPls.empty())
	{
		CList* pl = new CList();
		vPlst.push_back(pl);
		pl->Save();
	}
	else
	for (size_t i=0; i < vSetPls.size(); ++i)
	{
		CList* pl = new CList();
		pl->name = vSetPls[i].name;
		pl->bokm = vSetPls[i].bokm;
		pl->Load();
		vPlst.push_back(pl);
	}

	//  old ids
	int l = int(vPlst.size())-1;
	plsPlId = mia(0,l, plsPlId);  plsPl = vPlst[plsPlId];
	  plsId = mia(0,l, plsId);		pls = vPlst[plsId];
	
	// last state play
	if (bWasPlay)
	if (plsPl && plsPl->idPl >= 0 && plsPl->idPl < plsPl->vList.size())
	{
		pTrk tkPl = plsPl->vList[plsPl->idPl];
		if (tkPl && !tkPl->isDir())
		{
			string s = tkPl->getFullPath();
			if (boost::filesystem::exists(s))
				PlayFrom(lastPos);
	}	}
}

bool cAmp::PlayFrom(double t)
{
	float vol = fVol;	fVol = 0.f;
	Play(false);  fVol = vol;
	chPosAbs(t);
	if (bPlay)
		BASS_ChannelSetAttribute(chPl, BASS_ATTRIB_VOL, fVol);
	rt
}

void cAmp::DestPlsts()
{
	if (pls)  pls->Save();
	if (plsPl && plsPl != pls)  plsPl->Save();

	for (size_t i=0; i < vPlst.size(); ++i)
		delete vPlst[i];
	vPlst.clear();
}
