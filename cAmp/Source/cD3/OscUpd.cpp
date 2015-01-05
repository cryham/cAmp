#include "header.h"

#include "Osc.h"



//------------------------------  init
cOsc::cOsc()
{
	/*xL = xR = 0.f;*/
	dt = 0.01f;  tmd = 0;  view.fftSize=1;
	memset(A,0,sizeof(A));
	lock = false;  xpr = 0;  xpn = 0;  //ptim.iv
	bRec=0; bPlay=0; bPaused=0;
	UpdPClr();
}

//  update print colors
//------------------------------
void cOsc::UpdPClr()
{
	//  print colors
	float l = PrClrs-1,f, r,g,b, h,s,v;
	for (int i=0; i<PrClrs; ++i)
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
