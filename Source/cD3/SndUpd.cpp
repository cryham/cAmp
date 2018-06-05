#include "header.h"
#include "Snd.h"



//------------------------------  init
cSnd::cSnd()
	:bFps(0), bFInfo(0),bAllInfo(0), iTimeTest(0)
	,xpr(0),xpn(0)
	,lock(false)
	,tmd(0),tmD(0),xt_(0)
	,chRec(0), chPl(0), chMod(0), chSync(0)
	,bPlay(0),bPaused(0), bRec(0),bRecSet(0)
	,nDev(0), nFreq(0), maxFreq(44100)
	,dt(0.01f)
{
	view.fftSize = 1;
	memset(visA,0,sizeof(visA));
	//C, fft,fftB,wav
	UpdPClr();

	//  zero vis
	for (int i=0; i < sFFT; ++i)
	{
		fft[i]=0.00001f; fftB[i]=0.00001f;  wav[i]=0;
	}
	for (int i = 0; i < sWAV; ++i)
	{
		visA[i] = 0.f;
		for (int j = 0; j < PrLin; ++j)
			visC[j][i] = 0.f;
	}
}

//  update print colors
//------------------------------
void cSnd::UpdPClr()
{
	//  print colors
	float f, l = PrClrs-1, r,g,b, h,s,v;
	for (int i=0; i < PrClrs; ++i)
	{
		f = float(i)/l;  //_par
		h = 0.65f - 0.13f * pow(f, 1.2f);
		s = 0.9f - 0.9f * pow(f, 1.8f);
		v = pow(f, 0.4f);
		hsv2rgb(h,s,v,&r,&g,&b);
		pcl[i] = D3DCOLOR_COLORVALUE(r,g,b,0);
	}
}
