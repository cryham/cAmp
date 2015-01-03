#include "header.h"

#include "Amp.h"
#include "..\cD3\Graphics.h"
#include "..\main\App.h"
#include "..\main\KeyNames.h"


//  Player Rect
//------------------------------------------------------------------------------------------------------------
void cAmp::DrawAmpRect()
{
/* Position bar */
	if (bPlay)
	{
		tmPl = BASS_ChannelBytes2Seconds(chPl, BASS_ChannelGetPosition(chPl, BASS_POS_BYTE));
		float xp = (tmPl/tmTot) * (1.f-xWpo), //marg,bar dim
			x1 = xp*view.xSize;  int xb = xWpo*view.xSize;
		Rtex(pDev, Tex, TX_PosDk, 0.f,yBpo, view.xSize,yEpo);
		Rtex(pDev, Tex, TX_PosBr, x1, yBpo, x1+xb, yEpo, 0.48f,0.52f); //xp,xp+xWpo);
	}

/* Buttons  |< >|  */
	if (bMInWnd && ym < yEplbt)  {
		int i1=0,i2=0;
		if (xm > view.xSize/2)  i2=2;  else  i1=2;
		float y1= 0.f, y2=23.f, x1= 96.f/4.f, x2= view.xSize*0.5f;  //96x23
		Rtex(pDev, Tex, TX_Prev, x2-x1, y1, x2, y2, i1*0.25f, (i1+1)*0.25f);
		Rtex(pDev, Tex, TX_Next, x2, y1, x2+x1, y2, i2*0.25f, (i2+1)*0.25f);	}
	
	//  btns pls tabs up,dn
	if (ed==ED_Help || ed==ED_Keys || ed==ED_Set1)  return;
	{	int i1=1,i2=1;
		if (bMInWnd && xm > view.xSize-xWptbt && ym > yBpt && ym < yEpt)  //13x16
			if (ym-yBpt > (yEpt-yBpt)/2)  i2=2;  else  i1=2;
		float y1= yBpt, y2= yEpt-4, y3=64.f/4.f, x1= view.xSize-xWptbt;  //16x64
		D3DRECT r= {view.xSize-xWptbt, yBpt, view.xSize, yEpt-4};  pDev->Clear(1, &r, clFl, 0, 1.f, 0);
		Rtex(pDev, Tex, TX_Up, x1, y1, view.xSize, y1+y3, 0.f,1.f, i1*0.25f, (i1+1)*0.25f);
		Rtex(pDev, Tex, TX_Dn, x1, y2-y3, view.xSize, y2, 0.f,1.f, i2*0.25f, (i2+1)*0.25f);  }
}

//  Player  Text
//------------------------------------------------------------------------------------------------------------
void cAmp::DrawAmpText()
{
	CFont* cf = cfont[view.cfA];  CFont* cd = cfdig;

/* File Info  path,name */
	if (bFInfo && view.eVis!=viNone && pls && pls->ll)
	{
		cf->bSl=false;
		cf->Fclr = D3DXCOLOR(0.8,0.93,1,1);  pTrk tk = pls->vList[pls->lCur];

		//cf->StrWr(ExtAud[tkPl->ext], 1,yBvi);
		cf->StrWr(tk->path, 3,yBvi+cf->Fy/2);
		cf->Format("%s.%s",tk->name, ExtAud[tk->ext]);
		//cf->StrCopy(tk->name);  cf->StrAdd(".");  cf->StrAdd(ExtAud[tk->ext]);
		cf->Write(3,yBvi+4*cf->Fy-cf->Fy/2);
		cf->bSl=true;
	}

/* File info up  ext  kbps  khz  size */
	if (bPlay && plsPl)
	if (plsPl->idPl >=0 && plsPl->idPl <= plsPl->listLen-1)//-
	{	cf->Fclr = D3DXCOLOR(0.7,0.8,1,1);  pTrk tkPl = plsPl->vList[plsPl->idPl];
		cf->StrWr(ExtAud[tkPl->ext], 1,yBfi+1);
		cf->StrWr(sPlInf, 36,yBfi+1);  cf->Fclr = D3DXCOLOR(1,1,1,1);

/* Time 1:23  total,cur */
		cd->dir = -1;  int x = view.xSize-5/*-15*/;  //cd->Fs['.'] = 3;//cd->Fs['0'];

		//if (tmTot > 0.0 && tmPl > 0.0 && tmPl <= tmTot)
		if (tmPl<0.0)  tmPl=0.0;
		bool d = tmTot < 10.0;
		
		cd->Fclr = D3DXCOLOR(1,0.9,1,1);  // cur
		strTime(cd->str, tmPl, d);  cd->Write(x, yBfi+2);

		cd->Fclr = D3DXCOLOR(0.7,0.75,0.8,1);  // total
		strTime(cd->str, tmTot, d);  cd->Write(x-74, yBfi+2);
		cd->dir = 1;
		
	//  file rate
		cd->Fclr = D3DXCOLOR(0.4,0.6,0.9,1);
		/*if (plst->tkPl)*/  {  cd->str[0]= chDRates[tkPl->rate+cR0];  cd->str[1]=0;
			cd->Write(x-60-10, yBfi+2/*+25*/);	}
	}
	if (!bPlay)  // logo not playing
	{	cf->Fclr = D3DXCOLOR(0.7,0.8,1,1);
		cf->StrWr("Crystal  AMP  1.07", 1,yBfi+1);  cf->Fclr = D3DXCOLOR(1,1,1,1);  }

	//  pls inf  rate filters  =*
	if (!pls)  return;
	if (bDrawPlst && !pls->bThrTi && ed==ED_Pls)
	{	cd->Fclr = D3DXCOLOR(0.8,0.8,0.8,1);
		cd->str[0]= chDRates[pls->iRFilt+cR0];  cd->str[1]=0;
		cd->Write(view.xSize/2, yBpli-1);
		cd->Fclr = D3DXCOLOR(0.6,0.6,0.6,1);
		cd->str[0]= chDRates[pls->iRFilU+cR0];  cd->str[1]=0;
		cd->Write(view.xSize/2+12, yBpli-1);	}

/* Vol %,  @1 @A */
	if (tmd>0) {  tmd--;  int y = yBfi + (view.eVis==viNone ? 23 : 26);
		float f = min(1.f, 0.5f+float(tmd)/tmD);  cf->Fclr = D3DXCOLOR(0.7*f,0.8*f,1*f,1);
		if (bRep1)   cf->StrWr("@1", view.xSize-135,y);
		if (bRepAll) cf->StrWr("@A", view.xSize-110,y);
		cf->Format("Vol:%3.0f%%",100.f*fVol);  cf->Write(view.xSize-66,y);  }
		
// search results
	if (bShowSrch)
	{	cf->Fclr = D3DXCOLOR(0.3,1,0,1);
		cf->Format("Found: %d / %d",pls->iSrch,iSrchAll);  cf->Write(21,yBfi+18);  //cf->Write(3,yw-17);
	}

	//cf->Fclr = D3DXCOLOR(0.3,1,0,1);
	//cf->Format("m: %d, %d",xm,ym);  cf->Write(21,yBfi+18);

	//sv tm --
	//clr(0.9,0.8,1);  p(cf->s)"ms: %3d",plst->svtm);  cf->Write(20,yBfi+26);

	//*err*/ if (err[0]!=0) {  cf->StrWr(err, 370,Fy);  if (err[0]==' ')  err[0]=0;  }
	
	/*fps*/ if (bFps) {  cf->Format("%6.2f",Tim.FR);  cf->Write(view.xSize-51,45);  }
	if (bFps && view.eVis == viPrint) {  cf->Format("%6.2f",TimPr.FR);  cf->Write(view.xSize-51,65);  }

	//cf->Format("b %4d e %4d s %4d h %4d", yBvi, yEvi, view.ySize, view.visH);  cf->Write(view.xSize/2,65);

	//*ms*/ if (bFps) {  cf->Format("%6.1f",time*1000.);  cf->Write(xSize-51,31);  }
	//*rst*/if (!bFps) {  cf->Format("%4d",iResets);  cf->Write(xSize-71,31);  }
	//*cfP*/{  cf->Format("%4d",cfP);  cf->Write(xSize-71,31);  }
}
//------------------------------------------------------------------------------------------------------------
