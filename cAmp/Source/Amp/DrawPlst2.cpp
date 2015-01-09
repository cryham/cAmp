#include "header.h"
#include "Amp.h"
#include "..\cD3\Graphics.h"
#include "..\main\App.h"
#include "..\main\KeyNames.h"


//  Playlist Rect
//------------------------------------------------------------------------------------------------------------
void cAmp::DrawPlstRect()
{
/* pls Tab  [] */
	int curPlx,curPly=-1, plaPlx,plaPly=-1, selPlx,selPly=-1, movPlx,movPly=-1; //id=*
	int ntab = vPlst.size();  // find tab x,y
	if (ntab > 0)
	{	int x,y, a = view.ofsTab;
		for (y=0; y < view.yNpt; y++)
		for (x=0; x < view.xNpt; x++)  {  if (a < ntab)  {
			if (a==plsId)	{  curPlx = x;  curPly = y;  }  // cur^
			if (a==plsPlId)	{  plaPlx = x;  plaPly = y;  }  // play^
			if (a==plsSelId){  selPlx = x;  selPly = y;  }  // sel^
			if (a==nTabMov) {  movPlx = x;  movPly = y;  }  // mov
		}  a++;  }
	}
	// cur
	if (curPly >=0 && curPly < view.yNpt)  {
		float yt= yB_pt+curPly*yH_pt;  float x1= curPlx*xW_pt, x2= x1+xW_pt;
		Rtex(pDev, Tex, TX_TabC, x1, yt, x2, (yt+yH_pt));  }
	// play
	if (plaPly >=0 && plaPly < view.yNpt)  {
		float yt= yB_pt+plaPly*yH_pt;  float x1= plaPlx*xW_pt, x2= x1+xW_pt;
		Rtex(pDev, Tex, TX_TabP, x1, yt, x2, (yt+yH_pt));  }
	// sel
	if (selPly >=0 && selPly < view.yNpt)  {
		float yt= yB_pt+selPly*yH_pt;  float x1= selPlx*xW_pt, x2= x1+xW_pt;
		Rtex(pDev, Tex, TX_TabS, x1, yt, x2, (yt+yH_pt));  }
	// mov
	if (movPly >=0 && movPly < view.yNpt && alt)  {
		float yt= yB_pt+movPly*yH_pt;  float x1= movPlx*xW_pt, x2= x1+xW_pt;
		Rtex(pDev, Tex, TX_TabC, x1, yt, x2, (yt+yH_pt));  }


/* Pls cursor, sel */	///  pls backgr
	if (!pls)  return;
	bool bList = pls->listLen > 0;
	if (pls->vList.size()>0 && bList /*&& !plst->bThr..*/)
	{
		int i = pls->lOfs, y = yB_pl, im = min(pls->listLen, i+yL_pl);
		float fxw = view.xSize-view.xW_plS;
		int Fy = cfont[view.cfP]->Fy;

		for (i = pls->lOfs; i < im; i++)	/**/if (i < pls->vList.size())
		{	pTrk q = pls->vList[i];  if (q)  {
			float yf = y;

			if (q->sel > 0)  if (i != pls->lCur)	//  sel
				Rtex(pDev, Tex, TX_PlsS, 0.f, yf, fxw, (yf+Fy), 0.f,1.f, 0.1f,0.9f);
			if (i == pls->lCur)  Rtex(pDev, Tex, TX_PlsC, 0.f, yf, fxw, (yf+Fy));	//  cur
			if (i == pls->idPl)  Rtex(pDev, Tex, TX_PlsP, 0.f, yf, fxw, (yf+Fy));	//  play
			if (q->bokm > 0)	 Rtex(pDev, Tex, TX_PlsB1 + q->bokm - 1,
														  0.f, yf, fxw, (yf+Fy));	//  bookm
			//  rating backgr
			int rr = q->rate, r = mia(0,chRall, rr+cR0);
			if (r != cR0)
			{
				LPD3DXEFFECT e = fx[FX_rClr];
				e->SetValue("color", &vRclr[r], sizeof(float)*3);
				e->SetTexture("TexDiff", Tex/*[]*/);

				e->Begin(0,0);  e->BeginPass(0);
				Graphics::DrawRectTexC(pDev,0,mia(TX_Rate1,TX_Rate5, TX_Rate1-1+abs(rr)),
					0.f, yf, fxw, (yf+Fy), rtx.r, rtx.g, rtx.b, rtx.a);
				e->EndPass();  e->End();
			}
			
			//  dir show+ hide-
			if (q->isDir() && q->next && (i+1 < im) && q->next->hide > 0)  // _1st char?
				Rtex(pDev, Tex, q->next->hide==1 ? TX_DHide : TX_DShow, fxw-Fy, yf, fxw, (yf+Fy));

			y += Fy;
		}  }
	}
}


//  Slider
//------------------------------------------------------------------------------------------------------------
void cAmp::DrawSlider()
{
	if (!pls)  return;
	if (yB_pli >= view.ySize || view.xW_plS <= 0)  return;

	/* Pls slider | */
		if (pls->listLen <= yL_pl/2)  return;

		float fle = pls->listLen;
		float fc1 = pls->lCur /fle,  fc2 = (pls->lCur + 1) /fle;	if (fc2>1.f) fc2=1.f;  // cursor
		float fs1 = pls->lOfs /fle,  fs2 = (pls->lOfs +yL_pl) /fle;	if (fs2>1.f) fs2=1.f;  // vis list
		int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<2)  c2=c1+2;  // min h = 2 pix _
		int s1 = fs1 *yH_pl+yB_pl, s2 = fs2 *yH_pl+yB_pl;  if (s2-s1<2)  s2=s1+2;

		float xk1 = (view.xSize-view.xW_plS+1),	  xk2 = view.xSize;
		float xp1 = (view.xSize-2*view.xW_plS/3+1), xp2 = (view.xSize-view.xW_plS/2+1);

		D3DRECT rAll= {view.xSize-view.xW_plS+1, yB_pl, view.xSize, yE_pl};  pDev->Clear(1, &rAll, clFl, 0, 1.f, 0);
		if (pls->listLen > yL_pl)
		Rtex(pDev, Tex, TX_Slid, xk1, float(s1), xk2, float(s2));
		Rtex(pDev, Tex, TX_SliC, xk1, float(c1), xk2, float(c2));
		
	// playing _
		{
			float fc1 = pls->idPl /fle,  fc2 = /*fc1+4.f/yw*/(pls->idPl + 1.f) /fle;  if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;  if (c2-c1<2)  c2=c1+2;

			Rtex(pDev, Tex, TX_SliP, xk1, float(c1), xk2, float(c2));
		}
	// selected-
		float ySr = mia(1.f, 2.f, float(pls->listLen) / yL_pl);

	// rating ->
		if (view.bSlDrawR && !bShowSrch)
		for (int i=0; i < pls->listLen; i++)
		{
			int rr = pls->vList[i]->rate;	if (rr != 0)  {
			DWORD rgb = rr > 0 ? 0xFF40A0FF : 0xFF80FF00;

			float fc1 = i /fle,  fc2 = (i + ySr) /fle;		if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;	if (c2-c1<1) c2=c1+1;

			RtxC(pDev, Tex, mia(TX_Rate1,TX_Rate5, TX_Rate1-1+abs(rr)),
				xk1, float(c1), xk2, float(c2), rgb);//, 0.15f, 0.25f);
		}  }
	
	// search results +
		//ySr = mia(1.f, 1.f, float(pls->listLen) / yLpl);

		if (bShowSrch /*&& ySr > 0.8f*/)
		for (int i=0; i < pls->listLen; ++i)
		if (pls->vList[i]->srch > 0)
		{
			float fc1 = i /fle,  fc2 = (i + ySr) /fle;		if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;	if (c2-c1<1) c2=c1+1;

			Rtex(pDev, Tex, TX_SliF, xk1, float(c1), xp2, float(c2));
		}

	// bookmarks <*
		for (int i=0; i < pls->listLen; ++i)
		if (pls->vList[i]->bokm > 0)
		{
			float fc1 = i /fle,  fc2 = (i + ySr) /fle;		if (fc2>1.f) fc2=1.f;
			int c1 = fc1 *yH_pl+yB_pl, c2 = fc2 *yH_pl+yB_pl;	if (c2-c1<1) c2=c1+1;

			int tex = pls->vList[i]->bokm-1 + TX_SliB1;
			Rtex(pDev, Tex, tex, xp1, float(c1), xk2, float(c2));
		}
}
