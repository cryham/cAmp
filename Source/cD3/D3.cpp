#include "header.h"
#include "..\cD3\D3.h"
#include "..\main\resource.h"
#include "..\main\App.h"
using namespace std;


//  Init D3D
//--------------------------------------------------------------------------------------

bool D3::Init()
{
	log("D3 Init  ----");
	ScrSize();
	if (!CreateWnd()) {  log("Init D3D: Can't CreateWindow !");  rf  }

	DragAcceptFiles(hWnd, true);
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&csNext);

	if (FAILED( pD3d = Direct3DCreate9(D3D_SDK_VERSION) ))
	{	log("Init D3D: Direct3DCreate9 Failed !");  rf  }

	pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dmod);
	ZeroMemory(&ppar,sizeof(ppar));
	ppar.Windowed = true;	ppar.SwapEffect = D3DSWAPEFFECT_DISCARD;
	ppar.BackBufferFormat = dmod.Format;
	ppar.BackBufferWidth = view.xSize;  ppar.BackBufferHeight = view.ySize;
	ppar.PresentationInterval = view.vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
		
	if (FAILED( pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &ppar, &pDev) ))
	{	log("Init D3D: CreateDevice Failed !");  rf  }
		
	App::pAmp->GuiCreate();  rt
}

bool D3::Init2()
{
	log("D3 Init2 ----");
	ScrSize();
	HRESULT hr;  string p;
	
	//  CFonts
	for (int i=0; i<NumFnt; ++i)
	{
		cfont[i] = new CFont();
		cfont[i]->Fname = appPath+"Media\\fonts\\" + cStr::iToStr(i);
		cfont[i]->Fext = ".png";
		if (!cfont[i]->Init(pDev))
		{	Err("D3 Can't load CFont ");  rf  }
	}
	cfdig = new CFont();
	cfdig->Fname = appPath + "Media\\fonts\\dig";
	cfdig->Fext = ".png";  cfdig->Init(pDev);
	
	//  Plr Texture
	p = appPath + "Media\\player.png";
	if (FAILED( D3DXCreateTextureFromFileA(pDev, p.c_str(), &Tex) ))
		Err("D3 Can't create player texture "+p);

	///  Surf, clear  ................
	int d = max(view.xSize,view.ySize);
	if (FAILED( pDev->CreateOffscreenPlainSurface(view.xSize,view.ySize, D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&surf,0) ))
		Err("D3 Can't create spectrogram surface");
	if (surf) {  D3DLOCKED_RECT lr;  // clear
		surf->LockRect(&lr,0,0);  UINT* Os=static_cast<UINT*>(lr.pBits);  INT yO=lr.Pitch>>2;
		int l = view.ySize*view.xSize;  int a=0;
		for (int y=0; y < view.ySize; ++y)	{  a = y*yO;
		for (int x=0; x < view.xSize; ++x,++a)	Os[a] = 0;  }
		surf->UnlockRect();  }

	//  Vis Tex
	D3DFORMAT fmt = bFltTex ? D3DFMT_R32F : D3DFMT_X8R8G8B8;
	if (FAILED( D3DXCreateTexture( pDev, NextPow2(view.xSize), 1, 0,	D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &visTex ) ))
		Err("D3 Can't create vis Texture");
	if (FAILED( D3DXCreateTexture( pDev, NextPow2(view.xSize), 1, 0,	D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &visTex2 ) ))
		Err("D3 Can't create vis2 Texture");

	//  Vis Fx
	p = appPath + "Media\\vis-fft.fx";	CFont::LoadFX( pDev, p, fx[FX_fft] );
	p = appPath + "Media\\vis-osc.fx";  CFont::LoadFX( pDev, p, fx[FX_osc] );

	const char* tch = bFltTex ? "T1" : "T0";
	if (fx[FX_fft]) {  fx[FX_fft]->SetTechnique(tch);  fx[FX_fft]->SetTexture("TexVal", visTex);  }
	if (fx[FX_osc]) {  fx[FX_osc]->SetTechnique(tch);  fx[FX_osc]->SetTexture("TexVal", visTex);  fx[FX_osc]->SetTexture("TexVal2", visTex2);  }

	p = appPath + "Media\\rate.fx";  CFont::LoadFX( pDev, p, fx[FX_rClr] );
	if (fx[FX_rClr])   fx[FX_rClr]->SetTechnique("T0");
	
	App::pAmp->UpdDim(dmod.RefreshRate);  rt
}


//  Destroy D3D
//--------------------------------------------------------------------------------------

void D3::Destroy2()
{
	REL(surf)
	REL(visTex)  REL(visTex2)
	REL(Tex)
	for (int i=0; i<FX_ALL; ++i)  REL(fx[i])

	for (int i=0; i<NumFnt; ++i)
	if (cfont[i]){  cfont[i]->Destroy();  delete cfont[i];  cfont[i] = NULL;  }
		if (cfdig){    cfdig->Destroy();  delete cfdig;  cfdig = NULL;  }
}

void D3::Destroy()
{
	REL(pDev)	REL(pD3d)
	DeleteCriticalSection(&csNext);
	DeleteCriticalSection(&cs);
}


//  ctor default
D3::D3()
	:flog(0), errCnt(0)
	,xScreen(800), yScreen(600), fRfq(60.f)
	,xScrMin(0),yScrMin(0),xScrMax(2560),yScrMax(1600)
	,xMpos(0),yMpos(0), xMold(0),yMold(0), xWclick(0),yWclick(0)
	,mod(0), act(0), clear(0), iSize(0), iResets(0)
	,bLs(0),bL(0), bRs(0),bR(0), bMs(0),bM(0)
	,shift(0),ctrl(0),alt(0)

	,hKbd(0), hInst(0), hWnd(0)
	,pD3d(0), pDev(0)
	,visTex(0),visTex2(0),  surf(0), bckbuf(0)
	,Tex(0), bFltTex(1)
{
	int i;
	for (i=0; i<FX_ALL; ++i)  fx[i]=0;
	for (i=0; i<NumFnt; ++i)  cfont[i]=0;
	cfdig=0;
}


bool D3::Check()
{
	switch (pDev->TestCooperativeLevel())
	{
		case D3DERR_DEVICELOST:		iResets+=100;  return false;
		case D3DERR_DEVICENOTRESET:		App::pAmp->ReInit(true);
	}
	return true;
}


//  log
void D3::log(string s)
{
	if (!flog)  return;
	*flog << s << endl;
	(*flog).flush();
}

//  error
void D3::Err(string s)
{
	++errCnt;
	if (!flog)  return;
	*flog << s << endl;
	(*flog).flush();
}
