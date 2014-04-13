#include "header.h"

#include "Amp.h"
#include "..\cD3\Graphics.h"


/* - - - - - - - - - - - - - - - - - - - - - - - - -  Mouse  - - - - - - - - - - - - - - - - - - - - - - - - */

void cAmp::Wheel(int nWheel)	///  Wheel
{
	if (!nWheel)  return;
	if (ym < yBpt)  // plr vol
	{
		chVol((shift? 0.005f: ctrl? 0.05f: 0.02f)*(nWheel > 0? 1.f: -1.f));
		return;
	}
	//  plst scroll
	if (pls->listLen <= yLpl)  return;
	int m = shift ? 1 : ctrl ? yLpl/2 : 8;
	if (nWheel < 0)  pls->PgOfsDn(m);  else  pls->PgOfsUp(m);
	bDrawPlst = true;
}

void cAmp::Mouse()		///  move
{
	xms = xm;  xm = xMpos - view.xPos;
	yms = ym;  ym = yMpos - view.yPos;

	if (!act)  return;
	btnKeysOk = btnKeysOn = false;
	bMInWnd = xm >= 0 && xm < view.xSize && ym >= 0 && ym < view.ySize;
	if (!bMInWnd)  return;

	int Fy = cfont[view.cfP]->Fy;

	//  cur trk  ----
	if (!pls)  return; //-
	int cr = max(0, min(pls->listLen-1, (ym-yBpl)/Fy + pls->lOfs));
	pTrk dest = NULL;
	if (alt && pls->listLen > 0)
	{
		if (shift) {  pls->lInsM =-1;  pls->lInsPos = pls->lOfs;  } else
		if (ctrl)  {  pls->lInsM = 1;  pls->lInsPos = pls->lOfs+yLpl-1;  } else
		if (cr < pls->vList.size())
		{
			dest = pls->vList[cr];
			pls->lInsPos = cr;  //ins bar vis
			pls->lInsM = (ym-yBpl)%Fy >= Fy/2 ? 1 : -1;
			if (cr==pls->listLen-1)  pls->lInsM = 1;
			//  restrictions
			if (dest->sel > 0 || dest->isDir() /*|| // not near sel unhid-
				(dest->prev && dest->prev->sel > 0) ||
				(dest->next && dest->next->sel > 0)*/)  pls->lInsPos = -1;
			if (pls->numSel==0 && (cr==pls->lCur ||
				(cr-pls->lCur== 1 && pls->lInsM==-1) ||
				(cr-pls->lCur==-1 && pls->lInsM== 1)))  pls->lInsPos = -1;  // not near sel1
		}
	}	
		

	///  player
	//------------------------------------------------
	static bool bLbt = false;
	if (bL && (xm != xms || !bLs) && ym < yBpt)  // Left
	{
		if (!bLs && ym < yEplbt)  // prev,next  btns |< >|
		{
			if (xm < view.xSize/2)  Prev();  else  Next();
			bLbt = true;  return;
		}
		//  change pos <<| >>
		if (!bLs)  bLbt = false;
		if (!bLbt && ym > yBpt - 120)  // h
			chPosAbs(mia(0.,1., (double(xm) / view.xSize - xWpo*0.5) / (1.0-xWpo) ));
		return;
	}
	if (bR && !bRs && ym < yEplbt && plsPl)  //  Right rating
	{	// prev,next  btns |< >|
		if (xm < view.xSize/2)  plsPl->DecRatePl();  else  plsPl->IncRatePl();
		bDrawPlst = true;  return;
	}
	
	//  Mid
	if (bM && !bMs)
	{	xMs = xm;  yMs = ym;  mti = 0.f;  yMFvi = view.visH;  }
	if (shift)
	{
		if (bM && bMs)  // chng vis size
		{
			view.visH = mia(8, view.ySize/*-Fy*4*/, yMFvi + ym-yMs);
			UpdDim();
		}
	}


	if (ed==ED_Help)
		return;
	
	///  gui settings
	if (ed==ED_Set1)
	{
		gui[gpg].MouseMove(xm, ym, bL, bLs, bR && !bRs);
		return;
	}


	///  pick keys
	//------------------------------------------------
	if (ed==ED_Keys)
	{
		int y = yBgc, yy = -1;
		if (xm > xBgck)
		for (int i=0; i<FU_ALL; i++)
		{
			if (ym > y)  yy = i;
			y += yHgc + 16*cyFadd[i]/2;
		}
		if (ym > y)  yy = -1;

		//  sel
		if (!bPickingKey)	yPickKey = yy;
		if (yPickKey >= 0)
		{
			//  Left
			if (bL && !bLs)
				bPickingKey = !bPickingKey;
			
			//  Right
			if (bR && !bRs && !bPickingKey)
				vKeys[yPickKey].on = !vKeys[yPickKey].on;
		}
		
		//  checks
		y = yBgc;
		if (bL && !bLs)
		if (ym >= y && xm < xBgck && xm > xBgc && xm < xBgc+3*xWgc)
		for (int i=0; i<FU_ALL; i++)
		{
			if (ym > y && ym < y+yHgc)
			{
				int m = 0;
				for (int j=0; j<3; j++)  if (xm > xBgc+xWgc*j)  m = j;
				m = 1<<m;
				if (vKeys[i].mod & m)
					vKeys[i].mod &= ~m;
				else  vKeys[i].mod |= m;
			}
			y += yHgc + 16*cyFadd[i]/2;
		}
		// on
		if (xm > 16	   && ym > yBgc-90+36,
			xm < 16+19 && ym < yBgc-90+36+18)
		{	btnKeysOn = true;
			if (bL && !bLs)  bHKeys = !bHKeys;
		}
		// ok
		if (xm > 150 && ym > yBgc-80 && xm < 200 && ym < yBgc-60)
		{	btnKeysOk = true;
			if (bL && !bLs)  GuiOff();
		}
		return;
	}


	//  tabs  ------------------------------------------------
	nTabMov = -1;
	if (yBpt >= view.ySize)  return;  // not visible

	if (ym > yBpt && ym < yEpt)
	{	int y = (ym-yBpt)/yHpt, x = xm/xWpt, n = y*view.xNpt+x +view.ofsTab;
		if (y < view.yNpt && x < view.xNpt && n < vPlst.size())
			nTabMov = n;  }
	if (bL && !bLs && ym > yBpt && ym < yEpt)
	{
		bDrawPlst = true;
		if (xm > view.xSize-xWptbt)  // ofs btns up,dn
		{	if (ym-yBpt < (yEpt-yBpt)/2)
			{	// dec/inc tab x,y Num
				if (ctrl) {  if (view.yNpt>1) view.yNpt--;  }else
				if (shift){  if (view.xNpt>1) view.xNpt--;  }
				else  {  tabPrev(0,1);  return;  }
			}else
				if (ctrl)  view.yNpt++;  else  if (shift)  view.xNpt++;
				else  {  tabNext(0,1);  return;  }
			UpdDim();
		}else
		{	// change Tab
			if (nTabMov != -1)
			if (alt)
				tabMove(nTabMov);  // move
			else
			{	pls->Save();  plsId = nTabMov;  plsChg();	}
		}
		return;
	}
	//if (!bL && bLs)  bDrawPlst = true;


//  playlist
//------------------------------------------------------------------------------------------------
if (yBpl >= view.ySize)  return;  // not visible

if (ym > yBpl)
{
	///  Right  Play
	if (bR && !bRs && !shift/*move wnd*/ && pls->listLen > 0)
	{
		pTrk t = pls->vList[cr];
		if (!t->isDir())
		{	Stop();  //pls->idPl = cr;
			plsPlChg(plsId);  //pls id
			plsPl->idPl = cr;  //trk id
			Play(0);	}
		bDrawPlst = true;
	}
	
	///  Left
	if (bL && xm < view.xSize - xWplSm)
	{
		if (!bLs && ym > yBpl && ym < yEpl)
		{
			if (alt)		//  Move
			{
				int m = shift? -2: ctrl? 2: pls->lInsM;
				if (m == pls->lInsM && pls->lInsPos == -1)
					return;  //restr

				//CList* plFrom = plsSel ? plsSel : pls;
				if (/*plFrom->numSel > 0 ||/**/ plsSel && plsSel != pls)
					pls->MoveSel(m, dest, plsSel);
				else if (pls->numSel > 0)
					pls->MoveSel(m, dest, 0);
				else
					pls->Move1(m, dest);
			}
			else  if (shift) pls->SelRange(cr, ctrl);  //  select range
			else  if (ctrl)  pls->Select1(cr);  //  select 1
			else  pls->Pick(cr);  //  pick
			
			if (/*plsSel &&*/ (ctrl || shift || alt))  /*plsSel when sel chg*/
				updSelId(1);
			bDrawPlst = true;
		}
	}

	//  slider pls |
	if (bL && pls->listLen > yLpl)
	{
		if (xm > view.xSize - xWplSm)
		{	if (!bLs)
			{	/*ofs*/xLs = pls->lOfs;  yLs = ym; 	bLsl = true;  }
		}else
			if (!bLs)  bLsl = false;  
		
		if (bLs && bLsl)
		{
			float fle = float(pls->listLen);
			pls->lOfs = float(ym-yLs)/float(yHpl- yLpl/fle) *fle + xLs;
			pls->PgOfsDn(0);  pls->PgOfsUp(0);  //zOfs();  plst->zCur();  }
			bDrawPlst = true;
		}
	}
}

	///  Mid
	if (bM && bMs && pls->listLen > yLpl && !shift)
	{	yMd = ym - yMs;		// move pls
		if (abs(yMd) > 15)
		{
			mtiv = 0.6f - 0.07f* pow(float(abs(yMd)-15), 0.45f);  //ms
			if (mti > mtiv)
			{
				mti = 0.f;  int m = mtiv >= 0.f ? 1 : 1+ mtiv/-0.06f;
				if (yMd > 0)  pls->PgOfsDn(m);  else  pls->PgOfsUp(m);
				bDrawPlst = true;
			}
		}
		mti += dt;
	}
}
