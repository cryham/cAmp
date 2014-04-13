#include "header.h"
#include "..\cD3\D3.h"
#include "..\main\resource.h"
#include "..\main\App.h"


//  Init D3D
//--------------------------------------------------------------------------------------

bool D3::Init()
{
	ScrSize();
	if (!CreateWnd())  Wrng("Can't CreateWindow !","init D3D")
	DragAcceptFiles(hWnd, true);
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&csNext);

	if (FAILED( pD3d = Direct3DCreate9(D3D_SDK_VERSION) ))  Wrng("Direct3DCreate9 Failed !","init D3D")
	
	pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dmod);
	ZeroMemory(&ppar,sizeof(ppar));
	ppar.Windowed = true;	ppar.SwapEffect = D3DSWAPEFFECT_DISCARD;
	ppar.BackBufferFormat = dmod.Format;
	ppar.BackBufferWidth = view.xSize;  ppar.BackBufferHeight = view.ySize;
	ppar.PresentationInterval = view.vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
		
	if (FAILED( pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &ppar, &pDev) ))  Wrng("CreateDevice Failed !","init D3D")
		
	App::pAmp->GuiCreate();  rt
}

bool D3::Init2()
{
	ScrSize();
	HRESULT hr;
	
	//  CFonts
	for (int i=0; i<NumFnt; ++i)
	{
		cfont[i] = new CFont();
		p(cfont[i]->Fname) "%sMedia\\fonts\\%d", appPath,i);
		scpy(cfont[i]->Fext, ".png");  cfont[i]->Init(pDev);
	}
	cfdig = new CFont();  scpy(cfdig->Fname, appPath);
	sadd(cfdig->Fname,  "Media\\fonts\\dig");
	scpy(cfdig->Fext, ".png");  cfdig->Init(pDev);
	
	//  Plr Texture
	scpy(s,appPath);  sadd(s,"Media\\player.png");
	Vd( D3DXCreateTextureFromFileA(pDev, s, &Tex) );

	///  Surf, clear  ................
	int d = max(view.xSize,view.ySize);
	Vd( pDev->CreateOffscreenPlainSurface(view.xSize,view.ySize, D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&surf,0) );
	if (surf)  {	D3DLOCKED_RECT lr;
		surf->LockRect(&lr,0,0);  UINT* Os=static_cast<UINT*>(lr.pBits);  INT yO=lr.Pitch>>2;
		int l = view.ySize*view.xSize;  int a=0;
		for (int y=0; y < view.ySize; ++y)	{  a = y*yO;
		for (int x=0; x < view.xSize; ++x,++a)	Os[a] = 0;  }
		surf->UnlockRect();  }
	//if (!surf)	Info("Can't create voice print surface", "Init D3D 2");

	//  Vis Tex
	D3DFORMAT fmt = bFltTex ? D3DFMT_R32F : D3DFMT_X8R8G8B8;
	Vd( D3DXCreateTexture( pDev, NextPow2(view.xSize), 1, 0,	D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &visTex ) );
	//if (!visTex)	Info("Can't create vis Texture", "Init D3D 2");
	Vd( D3DXCreateTexture( pDev, NextPow2(view.xSize), 1, 0,	D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, &visTex2 ) );
	//if (!visTex2)	Info("Can't create vis Texture", "Init D3D 2");

	//  Vis Fx
	scpy(s,appPath);  sadd(s,"Media\\vis-fft.fx");	CFont::LoadFX( pDev, s, fx[FX_fft] );
	scpy(s,appPath);  sadd(s,"Media\\vis-osc.fx");  CFont::LoadFX( pDev, s, fx[FX_osc] );
	//scpy(s,appPath);  sadd(s,"Media\\vis-prt.fx");  CFont::LoadFX( pDev, s, fx[FX_ff2] );
	const char* tch = bFltTex ? "T1" : "T0";
	if (fx[FX_fft]) {  fx[FX_fft]->SetTechnique(tch);  fx[FX_fft]->SetTexture("TexVal", visTex);  }
	if (fx[FX_osc]) {  fx[FX_osc]->SetTechnique(tch);  fx[FX_osc]->SetTexture("TexVal", visTex);  fx[FX_osc]->SetTexture("TexVal2", visTex2);  }
	//if (fx[FX_ff2]) {  fx[FX_ff2]->SetTechnique(tch);  fx[FX_ff2]->SetTexture("TexVal", visTex);  }
	scpy(s,appPath);  sadd(s,"Media\\rate.fx");	CFont::LoadFX( pDev, s, fx[FX_rClr] );
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
		if (cfdig)  {  cfdig->Destroy();  delete cfdig;  cfdig = NULL;  }
}

void D3::Destroy()
{
	REL(pDev)	REL(pD3d)
	DeleteCriticalSection(&csNext);
	DeleteCriticalSection(&cs);
}


D3::D3()	// ctor default
{
	xScreen=800;yScreen=600; fRfq=60.f;
	xMold=yMold=xMpos=yMpos=xWclick=yWclick=0; act=0;  iResets=0;
	bLs=bL=0; bRs=bR=0; bMs=bM=0;  shift=ctrl=alt=0;  hKbd = 0;

	hInst=NULL; hWnd=NULL;  pD3d=NULL; pDev=NULL;
	visTex=NULL; visTex2=NULL;	surf=NULL; bckbuf=NULL;
	Tex=NULL;
	for (int i=0; i<FX_ALL; ++i)  fx[i]=NULL;
	for (int i=0; i<NumFnt; ++i)  cfont[i]=NULL;
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
