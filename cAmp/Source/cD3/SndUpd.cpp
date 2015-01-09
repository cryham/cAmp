#include "header.h"
#include "Snd.h"



//------------------------------  init
cSnd::cSnd()
	:bFps(0), bFInfo(0),bAllInfo(0)
	,xpr(0),xpn(0)
	,lock(false)
	,tmd(0),tmD(0),xt_(0)
	,chRec(0), chPl(0), chSync(0)
	,bPlay(0),bPaused(0), bRec(0),bRecSet(0)
	,nDev(0), nFreq(0)
	,dt(0.01f)
	////float  A[sWAV], C[PrLin][sWAV];  //fft,wav clr
{
	view.fftSize=1;
	memset(A,0,sizeof(A));
	UpdPClr();
}

//  update print colors
//------------------------------
void cSnd::UpdPClr()
{
	//  print colors
	float f, l = PrClrs-1, r,g,b, h,s,v;
	for (int i=0; i < PrClrs; ++i)
	{
		f = float(i)/l;
		//float r = 0.5*f, g = 0.78*f, b = 1.0*f;
		//pcl[i] = D3DCOLOR_COLORVALUE(r,g,b,0);
		//_par
		h = 0.65f - 0.13f * pow(f, 1.2f);
		s = 0.9f - 0.8f * pow(f, 1.6f);
		v = pow(f, 0.4f);
		hsv2rgb(h,s,v,&r,&g,&b);
		pcl[i] = D3DCOLOR_COLORVALUE(r,g,b,0);
	}
}
