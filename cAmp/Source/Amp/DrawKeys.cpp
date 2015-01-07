#include "header.h"
#include "Amp.h"
#include "..\cD3\Graphics.h"
#include "..\main\App.h"
#include "..\main\KeyNames.h"


//  Keys Rect
//------------------------------------------------------------------------------------------------------------
void cAmp::DrawKeysRect()
{
///  Keys pick
	int Fy = cfont[view.cfG]->Fy;  //~
	//D3DRECT rVis= {0,3, 205,502};
	//pDev->Clear(1, &rVis, clFl, RGB(20,147,170), 1.f, 0);

	/*on*/Rtex(pDev, Tex, TX_Checks,  16.f, float(yB_pt+36),
		(16.f+19.f), float(yB_pt+36+18), bHKeys?0.25f:0.f, bHKeys?0.5f:0.25f, 0.f,1.f);  if (btnKeysOn)
		  Rtex(pDev, Tex, TX_Checks,  16.f, float(yB_pt+36),	(16.f+19.f), float(yB_pt+36+18), 0.f, 0.25f, 0.f,1.f);
	/*ok*/Rtex(pDev, Tex, TX_PlsC, 150.f, (yB_gc-80.f), 200.f, (yB_gc-60.f));  if (btnKeysOk)
		  Rtex(pDev, Tex, TX_PlsS, 150.f, (yB_gc-80.f), 200.f, (yB_gc-60.f));

	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	int y = yB_gc, /*x=5,*/ v;
	for (int i=0; i<FU_ALL; i++)
	{
		int mod = vKeys[i].mod;  // checks
		int b[3] = {mod & 0x1, mod & 0x2, mod & 0x4};
		/*x sel*/int m = 0;  for (int j=0; j<3; j++)  if (xm > xB_gc+xW_gc*j)  m = j;
		for (int j=0; j<3; j++)
		{	/*m sel x2*/int mch = (j==m && xm > xB_gc && xm < xB_gc+3*xW_gc && ym > y && ym < y+yH_gc) ? 2:1;
			for (int a=0; a < mch; a++)  Rtex(pDev, Tex, TX_Checks,
				float(xB_gc+ xW_gc*j), float(y),
				float(xB_gc+14+ xW_gc*j), float(y+14), b[j]?0.25f:0.f, b[j]?0.5f:0.25f, 0.f,1.f);  }

		if (yPickKey >= 0 && i==yPickKey)
		{
			pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
			if (bPickingKey)
			Rtex(pDev, Tex, TX_PlsC, (xB_gck+5.f), float(y-3), (xB_gck+125.f), float(y+Fy+2));
			Rtex(pDev, Tex, TX_PlsP, (xB_gck+5.f), float(y-3), (xB_gck+125.f), float(y+Fy+2));
			pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		v = vKeys[i].vis;
		if (v > 0)
		{
			D3DRECT rVis= {0,y-3, 95,y+Fy+2};
			pDev->Clear(1, &rVis, clFl, D3DCOLOR_ARGB(0,(int)(v*0.7f)+20,(int)(v*0.9f)+47,(int)(v*1.2f)+70), 1.f, 0);
			vKeys[i].vis-=2;
		}
		y += yH_gc + 16*cyFadd[i]/2;
	}
}

//  Keys Text
//------------------------------------------------------------------------------------------------------------
void cAmp::DrawKeysText()
{
	CFont* cf = cfont[view.cfP];  //~
	#define  clr(r,g,b)  cf->Fclr = D3DXCOLOR(r,g,b,1)
	//if (shift) {  cf->StrWr("shift", xSize/2,Fy);  }
	//if (ctrl) {  cf->StrWr("ctrl", xSize/2+40,Fy);  }
	//if (alt) {  cf->StrWr("alt", xSize/2+70,Fy);  }

/*  Edit name  */
	if (ed==ED_nTab)
	{
		clr(0.5,0.9,1);  int y = yB_pli;
		cf->StrWr("Tab name: ", 1,y);
		cf->StrWr(sed, cf->xwr+10,y);
		
		int x = cf->xwr+1;  // cursor |
		fTi+=dt;  if (fTi > 0.7f) fTi=0.f;  //blink
		D3DRECT r= {x, y, x+2, y+cf->Fy};  pDev->Clear(1, &r, clFl, RGB(fTi*200+50,fTi*200,0), 1.f, 0);
	}
/*  Edit search  */
	if (ed==ED_nFind)
	{
		clr(0.3,1,0);  int y = yB_pli;
		cf->StrWr("Search: ", 1,y);
		cf->StrWr(sed, cf->xwr+10,y);  //x2
		
		int x = cf->xwr+1;  // cursor |
		fTi+=dt;  if (fTi > 0.7f) fTi=0.f;  //blink
		D3DRECT r= {x, y, x+2, y+cf->Fy};  pDev->Clear(1, &r, clFl, RGB(0,fTi*200+50,0), 1.f, 0);
	}

///  Keys list
	if (ed!=ED_Keys)  return;
	cf = cfont[view.cfG];  //~

	int y = yB_gc, x=5;
	clr(1,1,1);
	cf->StrWr("Global HotKeys", 15, yB_gc-90+15);
	cf->StrWr("OK", 150+20, yB_gc-80+5);
	clr(0.5,0.8,1.0);
	cf->StrWr("Function        Shift Ctrl Alt      Key", 15, yB_gc-25);
	clr(0.6,0.7,0.7);
	cf->StrWr("LMB- choose  RMB- on/off", x+120, yB_gc-90+40);
	for (int i=0; i<FU_ALL; i++)
	{
		if (vKeys[i].on)
			clr(1,1,1);  else  clr(0.5,0.6,0.7);

		cf->StrWr(cswFuns[i], 5+x, y);

		if (i == yPickKey && bPickingKey)
		{	clr(1,1,1);
			cf->StrCopy(" Press new key...");	}
		else
			strKeyName(vKeys[i].vk, vKeys[i].sc, cf->str);
		
		cf->Write(xB_gck+5+x, y);
		y += yH_gc + 16*cyFadd[i]/2;
	}
}