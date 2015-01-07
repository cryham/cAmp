#include "header.h"
#include "Amp.h"
#include "..\cD3\Graphics.h"
#include "..\main\App.h"
#include "..\main\KeyNames.h"


/* . . .  .   .                                 . .. GRAPH .. .                                .   .  . . . */
void cAmp::Graph()
{
	if (clear==0 && view.iSleep >= 0)
		Sleep(view.iSleep);
			
	Tim.update();
		dt = float(Tim.dt);  if (dt > 0.1f)  dt = 0.1f;
	
	MoveWnd();  //upd inp
	Mouse();
	
	if (!Check())  return;

// Begin
	if (clear > 0) {  clear--;
		pDev->Clear(0,0,clFl, 0, 1.f,0);
		bDrawPlst = true;	}

	if (FAILED( pDev->BeginScene() ))  return;
		
	GraphAmp();

// End
	pDev->EndScene();
	pDev->Present(0,0,0,0);
}


/* .  .                                            .  Draw  .                                          .  . */
//------------------------------------------------------------------------------------------------------------
void cAmp::GraphAmp()
{
	TimPr.iv = 1.0 / double(view.pr_fq);  ///--
	
//  Clear
	if (ed==ED_Keys || ed==ED_Set1)
		bDrawPlst = false;
	else
	if (pls)
	if (bDrawPlst2  || pls->bThr || pls->bThrTi ||
		ed==ED_nTab || ed==ED_nFind ||  alt || bAltOld && !alt)
		bDrawPlst = true;
	bAltOld = alt;  bDrawPlst2 = false;
	
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//if (view.eVis != viPrint || bDrawPlst) //!-
	{
	if (bDrawPlst)  pDev->Clear(0,0, clFl, 0/*200*/, 1.f, 0);  // clear all
	else {  D3DRECT r1= {0,0, view.xSize,yB_pt};
		pDev->Clear(1, &r1, clFl, 0, 1.f, 0);  }  // amp only
	}

/* plr back */
	if (view.eVis != viPrint && view.eVis != viOsc) //!
	{	float th = (view.eVis==viNone||view.eVis==viPrint)? 0.4f: 1.f; //min(1.f, float(yEvi)/view.visH);
		Rtex(pDev, Tex, TX_BackPlr, 0.f, 0.f, view.xSize, yE_vis/*yBpo*/, 0.f,1.f, 0.f,th);  }
	
/* Vis Tex */
	//pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (view.eVis!=viNone && !bFInfo)  Vis(pDev, yB_vis, yE_vis);
	//pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

/* pls bottom _ */
	/**/if ((bDrawPlst || ed!=ED_Pls) && yE_pl+5 < view.ySize)
		Rtex(pDev, Tex, TX_BackBtm, 0.f,yE_pl, view.xSize,view.ySize);

/* Backgr */
	if (yB_pt < view.ySize)  {
	if (bDrawPlst && ed==ED_Help)	Rtex(pDev, Tex, TX_BackHelp, 0.f, yB_pt, view.xSize, yE_pl);
	if (ed==ED_Keys || ed==ED_Set1)	Rtex(pDev, Tex, TX_BackPlr,  0.f, yB_pt, view.xSize, yE_pl);  }


	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	
//  Rect  ------------------------
	DrawAmpRect();
	if (bDrawPlst && ed!=ED_Help)    DrawPlstRect();
	if (ed==ED_Keys)  DrawKeysRect();  else
	if (ed==ED_Set1)  gui[gpg].DrawRect();


//  Text  Begin  ------------------------
	CFont::Begin(1);
	for (int i=0;i <NumFnt; ++i)  // reset fonts
	{	cfont[i]->dir = 1;  cfont[i]->xmax = view.xSize;
		cfont[i]->Fclr = D3DXCOLOR(1,1,1,1);
		cfont[i]->Fs[' '] = cfont[i]->Fs['0']/2;
		cfont[i]->SetTex(1);
	}	cfdig->xmax = view.xSize;

	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	DrawKeysText();
	DrawAmpText();
	if (ed==ED_Set1)  gui[gpg].DrawText(dt);

	if (ed==ED_Help)
	{	if (bDrawPlst) DrawHelpText();
		bDrawPlst = false;	}
	
	if (bDrawPlst)    DrawPlsText();

	CFont::End();
//  Text End  ------------------------

	if (bDrawPlst)    DrawSlider();
	
	bDrawPlst = false;
}
