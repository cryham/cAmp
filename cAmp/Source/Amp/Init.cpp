#include "header.h"

#include "Amp.h"
#include "..\main\App.h"
#include "..\cD3\Graphics.h"
#include "MMSystem.h"


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
	GetModuleFileNameA(0,s,sizeof(s));
	scpy(appPath, s);  char* pe = strrchr(appPath,'\\')+1;  pe[0]=0;  //ends with\ 

	ScrSize();
	SetLoad();	// set
	ClrLoad();
	SetPriorityClass(GetCurrentProcess(),  // not here-
		(iPriority>=2) ? REALTIME_PRIORITY_CLASS :
		((iPriority==1) ? HIGH_PRIORITY_CLASS : NORMAL_PRIORITY_CLASS) );

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
	clear = 2;
}

void cAmp::End()
{
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

	for (int i=0; i < vPlst.size(); ++i)
	{	CList* pl = vPlst[i];

		aaD += pl->allDirs;  aaF += pl->allFiles;
		aaSi += pl->allSize/1000000;
		aaTm += pl->allTime;  }
}

//  Update dims
//------------------------------------------------------------------------

void cAmp::UpdDim(float rfrFq)
{
	Graphics::SetupStates(pDev);
	if (view.visH > view.ySize)  view.visH = view.ySize;  //vis max

	//  file info
	yBfi = 0;	yEplbt = 20;  // btnsH |< >|

	//  visual fft
	yBvi = 16;  //yFvi = 64;
	yEvi = min(view.ySize, 28 + ((view.eVis!=viNone)? view.visH: 0));
	
	//  pos bar
	yBpo = yEvi;  yEpo = yBpo+9;
	xWpo = 0.03f;

	//  pls tabs
	yBpt = yEpo;  /*par+- +2 8pos*/
	xWptbt = 13;  // btnsW up,dn
		xWpt = (view.xSize - xWptbt)/view.xNpt;
		yHpt = cfont[view.cfT]->Fy+2;  // <^ dim pls tabs
	yEpt = yBpt+ view.yNpt*yHpt+4;

	//  pls tracks
	yBpli = yEpt;
	yBpl = yBpli +cfont[view.cfP]->Fy+2/*yHpli*/;
	yEpl = view.ySize -cfont[view.cfP]->Fy;
	yHpl = yEpl-1-yBpl;  yLpl = max(0, yHpl/cfont[view.cfP]->Fy);
		DELA(Lxm);  Lxm = new int[yLpl+4];
		yEpl = yLpl*cfont[view.cfP]->Fy+yBpl;  yHpl = yEpl-1-yBpl;

	/*xWplS = 14;*/  xWplSm = 40;  //|sliderW, mW
	xTm = view.xSize - view.xWplS-3;  //|
	CList::Lin = yLpl;

	if (rfrFq > 0.f)
		tmD = int(rfrFq*1.f);	clear=1;
	
	// for..
	cfont[view.cfP]->StrCopy(" 0:00");  xt_ = cfont[view.cfP]->GetWidth();

	//  gui checks pos
	xBgc = 106;  yBgc = yBpt+90;  xWgc = 18;
	xBgck = 170;  yHgc = cfont[view.cfG]->Fy+6; //[0
	
	GuiReInit();
}

cAmp::cAmp()
{	bL= bR= bM= bLsl= false;  xm= 0; ym= 0;  xMs= 0; yMs= 0; yMd= 0;  bMInWnd=0;  mti=0.f;
	chPl= NULL;  bPaus= 0;  bPlay= 0;  sPlInf[0]=0;  thrIns = 0;  appPath[0]=0;
	pls=plsPl=plsSel=NULL;  plsSelId=-1;
	sed[0]=0;  ied=0;  srch[0]=0;  fTi=0.f;  Lxm=NULL;  bFInfo=0; bAllInfo=0;
	bNextPrev=1;  bDrawPlst=1; bDrawPlst2=0;  bShowSrch=0; iSrchAll=0; bAltOld=0;
	yPickKey=-1; bPickingKey=0; btnKeysOk=btnKeysOn=0;
	clear=1;  hlpPg=0;  hpr = NULL;
	ed=ED_Pls;  //ed=ED_Set1;  //*
	gpg=0;
}
cAmp::~cAmp()
{	DELA(Lxm)	}



///  load playlists  * * * * * * * * * * * * * * * * * * * * * * * * * * 

void cAmp::LoadPlsts()
{
	// load last -from set
	if (vPlsNames.size()==0)
	{
		CList* pl = new CList();
		vPlst.push_back(pl);  pl->Save();
	}else
	for (int i=0; i < vPlsNames.size(); ++i)
	{
		CList* pl = new CList();
		scpy(pl->name, vPlsNames[i]);  pl->Load();
		vPlst.push_back(pl);
	}

	//  old ids
	int l = vPlst.size()-1;
	plsPlId = mia(0,l, plsPlId);  plsPl = vPlst[plsPlId];
	  plsId = mia(0,l, plsId);		pls = vPlst[plsId];
	
	// last state play
	if (bWasPlay)
	if (plsPl && plsPl->idPl >= 0 && plsPl->idPl < plsPl->vList.size())
	{
		pTrk tkPl = plsPl->vList[plsPl->idPl];
		if (tkPl && !tkPl->isDir())
		{
			char s[320];
			tkPl->getFullName(s);
			if (boost::filesystem::exists(s))
				PlayFrom(lastPos);
	}	}
}

bool cAmp::PlayFrom(double t)
{
	float vol = fVol;	fVol = 0.f;
	Play(false);  fVol = vol;
	chPosAbs(t);
	if (bPlay)  BASS_ChannelSetAttribute(chPl, BASS_ATTRIB_VOL, fVol);	rt
}

void cAmp::DestPlsts()
{
	for (int i=0; i < vPlsNames.size(); ++i)
		DELA(vPlsNames[i])

	if (!pls)  return;
	/* */pls->Save();
	if (plsPl && plsPl != pls)  plsPl->Save();

	for (int i=0; i < vPlst.size(); ++i)
		DEL(vPlst[i]);
	vPlst.clear();
}
