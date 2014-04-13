#include "header.h"

#include "Graphics.h"
#include "Osc.h"
#include "D3.h"
#include "..\main\App.h"


///  Voice print . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
//
#define  PlR  (bPaus ? false : (bPlay || bRec))
void cOsc::Vpr()
{
	//  get data
	if (view.eVis==viPrint && surf && PlR)
	{
		EnterCriticalSection(&cs);
		BASS_ChannelGetData(bPlay ? chPl : chRec, fftB, fts[view.fftSize] );

		for (int x=0; x < view.xSize+1; x++)
		{
			float f = fftB[x+1];  if (f<0.000001f) f=0.000001f;
			float y = -log10(f) * view.fftMul /255.f -0.1f;  //par

			y = mia(0.f,1.0f, y);
			C[xpn][x] = 1.f-y;
		}
		// _ write pos
		xpr++;  if (xpn < PrLin)  xpn++;
		if (xpr >= view.xSize)  xpr = 0;
		
		LeaveCriticalSection(&cs);
	}else
	if (!bPaus)  {  xpr = 0;  xpn = 0;  }
}


///  FFT / Osc  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
//
void cOsc::Vis(IDirect3DDevice9* pDev, float y1,float y2)
{
	DWORD chan = bPlay ? chPl : chRec;

//  voice print
	if (view.eVis==viPrint && surf)
	{	
		///  Surf Lock  ................
		if (surf && xpn > 0)  // write new
		{
			EnterCriticalSection(&cs);
			D3DSURFACE_DESC d;  surf->GetDesc(&d);
			//UINT Omax = d.Height * d.Width;
		
			D3DLOCKED_RECT lr;
			HRESULT hr = surf->LockRect(&lr,0,0);
			UINT* O=static_cast<UINT*>(lr.pBits);  INT yO=lr.Pitch>>2;
			INT Omax = d.Height * yO - 1;
			if (hr == D3D_OK)
			{
				float l = PrClrs-1;  // vertical |
				//**/if (xpr + (view.ySize-1)*yO < Omax)
				{
					for (UINT x = 0; x <= xpn-1; ++x)
					for (UINT y=0; y < view.ySize; ++y)
					{
						float f = C[x][view.ySize-1-y];
						O[min(Omax, max(0, xpr-xpn-1+x) + y*yO)] =
							pcl[ (int)(l*f) ];  // split end 2x..
					}
				}
				xpn = 0;
				surf->UnlockRect();
			}
			LeaveCriticalSection(&cs);
		}

		pDev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&bckbuf);

		int x1 = 0, x2 = view.xSize;  //all
		//int x1 = xpr-5, x2 = xpr;
		RECT d = {x1,y1,x2,y2}, s = {x1,/*max(0,*/view.ySize+1-y2+y1, x2,view.ySize};
		pDev->StretchRect(surf,&s,bckbuf,&d,D3DTEXF_POINT);

		bckbuf->Release();	return;
	}

//  fft, osc
	bool bFFT = view.eVis==viFFT;
	//  get data
	if (PlR)
	{
		if (view.eVis==viFFT)
		{
			BASS_ChannelGetData(chan, fft, fts[view.fftSize] );

			for (int x=0; x < view.xSize+1; x++)
			{
				float f = fft[x+1];  if (f<0.000001f) f=0.000001f;
				float y = -log10(f) * view.fftMul /255.f -0.1f;  //par

				y = mia(0.f,1.0f, y);	A[x] = y;
			}
		}else
		if (view.eVis==viOsc)
		{
			BASS_ChannelGetData(chan, wav, 2*2*(view.xSize+1)*sizeof(short));
			UINT a = 0;  int w = 1;
				if (bPlay)  w=-1;  // osc -y play, rec y
			for (int x=0; x < view.xSize+1; x++)
			{
				int i = w*(wav[a++] + wav[a++]) / 2;  a+=2;
				float y = 0.5f + i/65536.f;
				A[x] = y;
				//A[x] = (0.5f - 0.5f*sin(float(x)/view.xSize*6.28) );  // test
			}
		}

		///  Lock, Draw  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 

		D3DLOCKED_RECT lr;
		visTex->LockRect(0, &lr, 0, D3DLOCK_DISCARD|D3DLOCK_NOOVERWRITE);
		if (bFltTex)  // float textures
		{
			float* o = (float*)lr.pBits;
			for (UINT x=0; x < view.xSize; ++x)
				o[x] = A[x];  // x%2; test

			if (!bFFT && visTex2)  // 2nd for Osc
			{
				visTex2->LockRect(0, &lr, 0, D3DLOCK_DISCARD|D3DLOCK_NOOVERWRITE);
				float* o = (float*)lr.pBits;
				for (UINT x=0; x < view.xSize; ++x)
					o[x] = A[x+1];
				visTex2->UnlockRect(0);
			}
		}
		else  // rgb byte textures
		{
			UINT* o = (UINT*)lr.pBits;
			if (bFFT) for (UINT x=0; x < view.xSize; ++x)  o[x] = DWORD(255*A[x]);
			else	  for (UINT x=0; x < view.xSize; ++x)  o[x] = DWORD(255*A[x]) | (DWORD(255*A[x+1])<<8 );
		}
		visTex->UnlockRect(0);
	}
	
	if (visTex)
	{
		//  vis fx
		LPD3DXEFFECT e = fx[bFFT ? FX_fft : FX_osc];
		float ftx = 0.5f/view.xSize;  // half pixel ofs

		//  scale osc for big views
		float xf = 1.f;
		if (!bFFT && view.xSize > xScreen/3)  xf = 0.5f/*+(float)view.xSize / xScreen*/;
		//if (!bFFT)  {
		//	e->SetFloat("gWidth",	0.0047f - 0.00469998f * view.visH / 1200.f);
		//	e->SetFloat("gPow",		0.18f - 0.08f * view.visH / 1200.f);  }

		e->Begin(0,0);  e->BeginPass(0);
		Rtex(pDev, visTex, 0.f, y1, view.xSize, y2, 0.f+ftx,xf+ftx);
		e->EndPass();  e->End();
	}

  /*  Line meter = */
	/*if (bLin)
	{
		float fL = LOWORD(BASS_ChannelGetLevel(chan)) / 128.f;
		float fR = HIWORD(BASS_ChannelGetLevel(chan)) / 128.f;

		float dL = fL-xL;  if (dL>0.f)  xL += Ladd*dt* dL;  else  xL += Lsub*dt* dL;  int liL = xL;
		float dR = fR-xR;  if (dR>0.f)  xR += Ladd*dt* dR;  else  xR += Lsub*dt* dR;  int liR = xR;
	}/**/
}
