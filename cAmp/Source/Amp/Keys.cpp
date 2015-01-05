#include "header.h"

#include "Amp.h"


DWORD WINAPI BrowseDirThr(LPVOID lp)
{ 
	cAmp* pA = (cAmp*)lp;
	char pa[MP];
	if (pA->BrowseDir(pA->sInsPath, &pa[0]))  {
		pA->pls->trGet = true;
		pA->pls->InsertDir(pa);  scpy(pA->sInsPath, pa);  }
	pA->thrIns = 0;
    return 0;
}

void cAmp::GuiOff()
{
	ed = ED_Pls;  UpdDim();
	bDrawPlst = true;  bPickingKey = false;
}

/*-   -   -   -   -   -   -   -   -   -   -   -   -   Keys   -   -   -   -   -   -   -   -   -   -   -   -  -*/
bool cAmp::Keys(WPARAM k)
{
	bDrawPlst = true;
	
	//  Edit Tab Name
	if (ed==ED_nTab || ed==ED_nFind)  {	KeysEdit(shift, k);  rt  }
	if (ed==ED_Keys)  // config Keys
	{	if (k==VK_F1 && (ctrl || alt))  GuiOff();  rt  }
	//  gui keys ...
	
	if (shift && k =='1' && ed==ED_Pls)  {  if (view.cfP > 0)		 view.cfP--;  UpdDim();  rt}
	if (shift && k =='2' && ed==ED_Pls)  {  if (view.cfP < NumFnt-1) view.cfP++;  UpdDim();  rt}
	//  load/save view
	if (alt || ctrl /*&& ed==ED_Pls*/)
	{
		int v = k - '1';
		if (v >= 0 && v < MaxViews)
		{
			if (ctrl)		ViewSave(v);
			else if (alt)	ViewLoad(v);
			rt
		}
	}
	
	switch (k)  /* ALL */
	{
		//  player
		case 'Q': case VK_LEFT:	if (alt)  chVol(shift?-0.005f: ctrl?-0.05f:-0.02f);  else  chPos(shift?-1: ctrl?-15:-5);  rt
		case 'W':case VK_RIGHT:	if (alt)  chVol(shift? 0.005f: ctrl? 0.05f: 0.02f);  else  chPos(shift? 1: ctrl? 15: 5);  rt

		case VK_RETURN:  Play(1,1);	rt
		case 'Z':  Play(0);	rt		case 'X':  Pause();	rt		case 'C':  Stop();	rt
		case 'A':  if (ctrl) PlayPrevTab();  else  if (alt && plsPl) plsPl->DecRatePl();  else  Prev();  rt
		case 'S':  if (ctrl) PlayNextTab();  else  if (alt && plsPl) plsPl->IncRatePl();  else  Next();  rt
		
		case 'R':  repAll();  rt	case '1': case 'T':  rep1();  rt  // repeat

		//  show
		case 'O':  view.eVis= (EVis)((view.eVis-1+viALL)%viALL);  UpdDim();  rt	 // vis prev
		case 'P':  view.eVis= (EVis)((view.eVis+1)%viALL);        UpdDim();  rt  // vis next
		case 'I':  bFps= !bFps;  rt	 // fps
		
		case '3':  bFInfo= !bFInfo;  rt  // info  file,dir
		case '2':  if (shift)
		#define idv  CList::iDirView
		{	idv--;  if (idv < 0)  idv = 2;  }else
		{	idv++;  if (idv > 2)  idv = 0;  }  rt  // paths

		case '4':  bAllInfo= !bAllInfo;  if (bAllInfo) updAllInfo();  rt  // all plst stats
		case '5':  CList::bFilInf= !CList::bFilInf;  pls->listUpd(/**/);  rt  // plst stats
		case '0':  view.bSlDrawR= !view.bSlDrawR;  rt  // slider draw rating

		case VK_F1:
			if (ctrl || alt) {  for (int i=0; i < FU_ALL; ++i)  vKeys[i].vis = 0;
				ed=ED_Keys;  bPickingKey = false;	} else
			if (ed==ED_Help)  GuiOff();  else  ed=ED_Help;  rt	 // help

		case VK_F2:  if (ctrl || alt)  {
			if (ed==ED_Set1)  GuiOff();  else  ed=ED_Set1;  rt}	 // set1
			break;

		case VK_F3:  if (ctrl || alt)  {
			if (ed==ED_Set1)  GuiOff();  else  ed=ED_Set1;  rt}	 // set1
			break;

		case VK_F11:  ReInit(false);  rt

		// rename file,dir ...
	}
	switch(ed)
	{
		case ED_Pls:	 /* PLS */
		{	switch(k)  {	//  shift- up,dn,pg,hm,  keyb sel- ...
				
			// none- up/dn 1   ctrl- x8
			//	alt- move1   alt-shift- move1  alt-ctrl- move1
			case VK_UP:		if (alt)  pls->Move1(shift? -2: ctrl? 2: -1,0);  else  pls->Up(ctrl ? 8:1);  rt
			case VK_DOWN:	if (alt)  pls->Move1(shift? -2: ctrl? 2:  1,0);  else  pls->Dn(ctrl ? 8:1);  rt
		
			// none- page/4  ctrl- page   alt- offset 1  ctrl-alt- ofs x8
			case VK_PRIOR:	if (alt)  pls->PgOfsUp(ctrl ? 8:1);  else  pls->PgUp(ctrl ? yL_pl : yL_pl/4);  rt
			case VK_NEXT:	if (alt)  pls->PgOfsDn(ctrl ? 8:1);  else  pls->PgDn(ctrl ? yL_pl : yL_pl/4);  rt
			
			// none- move by dir   ctrl- view   alt- list
			case VK_HOME:	pls->Home(alt ? 2: ctrl ? 1:0);  rt
			case VK_END:	pls->End( alt ? 2: ctrl ? 1:0);  rt


			///  -,+ rate file
			// none- playing cur  ctrl- cursor
			case VK_SUBTRACT: case VK_OEM_MINUS:
				if (ctrl) pls->DecRate(); else  pls->DecRatePl();	rt
			case VK_ADD:	  case VK_OEM_PLUS:
				if (ctrl) pls->IncRate(); else  pls->IncRatePl();	rt

			///  rating filter
			case VK_DIVIDE:   case VK_OEM_4:
				pls->DecRFil(ctrl);  rt
			case VK_MULTIPLY: case VK_OEM_6:
				pls->IncRFil(ctrl);  rt

			//  next/prev tab   ctrl- dn/up row  shift- ofs row
			case VK_OEM_3:  tabPrev(ctrl,shift);  rt  // tilde`
			case VK_TAB:    tabNext(ctrl,shift);  rt

			///  save/load quick
			case VK_F4:  if (pls)  pls->Save();  SetSave();  rt
			case VK_F5:  if (ctrl)	PlsOpen();  else  	//  open plst dialog
			{	bool b = bPlay;  Stop();  clrSelId();
				pls->Clear();  pls->Load();  if (b) Play(0);  }  rt
			
			//  bookmark
			case VK_OEM_5:  pls->Bookm(ctrl||shift ? -1 : 1);  bDrawPlst = true;  rt  // backslash
			//  goto prev/next
			case VK_OEM_PERIOD:	pls->BookmPrev(alt ? 3 : ctrl ? 2 : 1, bShowSrch);  rt
			case VK_OEM_2:		pls->BookmNext(alt ? 3 : ctrl ? 2 : 1, bShowSrch);  rt  // slash
			
			//  duplicate  none- below   shift- top   ctrl- end
			case 'D':  pls->Insert1(shift? -2: ctrl? 2: 1, NULL);  rt
				// dupl sel- ..
			
			//  hide dir   // file..., err-disabled
			case 'H':  pls->Hide(false);  rt
			//  show dir
			case 'J':  pls->Hide(true);  rt
			
			//  spc UnSel
			case VK_SPACE:  pls->UnSel();  clrSelId();  rt
			//  back gotoPlay
			case VK_BACK:  if (ctrl) {  plsId = plsPlId;  plsChg();  }  pls->GotoPlay();  rt
			
			/// new copy files			
			case VK_F12:
				pls->CopySelFiles();  rt


			//  del
			case VK_DELETE:  //none- del one/sel  shift- from disk  ctrl- clear plst
				if (ctrl)  {  pls->Clear();  clrSelId();  }  else
				{	if (pls->numSel == 0)  {  // del one
						if (shift && pls == plsPl && pls->lCur == pls->idPl)
						{	Next();  if (plsPl->idPl == 0)  Stop();  }  // del playing
						pls->Del(shift);  }
					else  {  pls->DelSel(shift);  clrSelId();  }  }  rt  // del sel
			
			//  ins dir
			case VK_INSERT:
				if (shift)	pls->InsM = -2; else  //top,end, cur
				if (ctrl)	pls->InsM =  2; else  pls->InsM = (pls->lCur==0) ? -1 : 1;
				
				if (!pls->bThr && !thrIns)
				//if (alt)
					thrIns = CreateThread(NULL,0,BrowseDirThr,(LPVOID)this,0,NULL);
				rt

			//  New tab
			case 'N':  tabNew(alt? -1: ctrl? 2 : shift? -2: 1);  rt
			//  Close tab
			case VK_F8:  tabClose();  rt
			
			//  Edit tab name
			case VK_F2:
			{	scpy(sed, pls->name.c_str());  ied=strlen(sed);  ed=ED_nTab;  }  rt
			//  Search
			case VK_F3:	case 'F':
				if (shift)  bShowSrch=!bShowSrch;  else
				{	scpy(sed, srch);  ied=strlen(sed);  ed=ED_nFind;  }  rt
			
			//... update names (search rating)

			//  Reload Dir
			case VK_F6:
			if (pls && pls->ll)  if (pls->vList[pls->lCur]->isDir())
			{	if (shift)  pls->InsM =-2; else  //top,end, cur
				if (ctrl)   pls->InsM = 2; else  pls->InsM = (pls->lCur == 0) ? -1 :1;
				bool rem = !alt && !ctrl && !shift;  // alt duplicate

				// select dir
				if (rem) {	pls->UnSel();  clrSelId();  pls->SelDir(pls->lCur);  }

				pls->trGet = true;  pls->trRem = true;
				pls->InsertDir(pls->vList[pls->lCur]->path);

				// delete duplicated-
				//if (rem) {	int i=0;
				//	while (pls->bThr)  Sleep(20);  // lock..
				//	pls->DelSel();	}
			}  rt
			
			//  update list
			case 'U':  {  pls->listUpd(0);  pls->UpdTimes();  }  rt

			//  Rename rating   none- cur   shift- dir   ctrl- plst
			case VK_F9:  RenameAll(ctrl? 2: shift? 1: 0);  rt
			//  Rename all + save
			case VK_F10:
			{	RenameAll(2);	pls->Save();  SetSave();  }  rt
			
		}	}	break;

		
		case ED_Set1:	// gui set
			rt
	
		case ED_Help:  /* Help */
			switch(k)
			{	case VK_HOME:  hlpPg = 0;	 rt
				case VK_END:   hlpPg = HelpPages-1;  rt
				case VK_PRIOR:case VK_LEFT: case VK_UP:
					hlpPg--; if(hlpPg < 0)  hlpPg = 0;  rt
				case VK_NEXT: case VK_RIGHT:case VK_DOWN:
					hlpPg++; if(hlpPg > HelpPages-1)  hlpPg = HelpPages-1;  rt
			}	break;
	}
	rf
}