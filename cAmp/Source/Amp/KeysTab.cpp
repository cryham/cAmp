#include "header.h"

#include "Amp.h"


///  Rename rating all  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

void cAmp::RenameAll(int type)
{
	if (!pls || !pls->ll)  return;
	bool e = (plsPl==pls) && bPlay;
	double tm = tmPl/tmTot;
	pTrk tkPl = e ? pls->vList[pls->idPl] : NULL;
	pTrk tcur = pls->vList[pls->lCur];

	pTrk q = pls->ll;  int i=0;
	switch (type)
	{
		case 2: // all
		case 1: // dir
			while (q)
			{
				if (!q->isDir())
				if (type==2 || strcmp(q->path, tcur->path)==0)
				{
					if (e && q==tkPl)  Stop();
					if (pls->RenameRate(q))  i++;
					if (e && q==tkPl)  PlayFrom(tm);
				}
				q = q->next;
			}	break;
			// many entries in plst, other vplst--

		case 0: // cur
		{	bool pl = e && (pls->lCur==pls->idPl);
			if (pl)  Stop();
			pls->RenameRate(tcur);
			if (pl)  PlayFrom(tm);
		}	break;
	}
	bDrawPlst = true;  //show i, fade-
}


///  * * *  do search  * * *
void cAmp::DoSearch()
{
	iSrchAll = 0;
	if (srch[0]!=0)
	for (int a=0; a < vPlst.size(); a++)
	{
		CList* pl = vPlst[a];
		int ii = 0;  // hits
		
		for (int i=0; i < pl->listLen; i++)
		{	pTrk q = pl->vList[i];
			char* name = _strdup(q->name);	_strlwr(name); //no case
			bool bSch = strstr(name, srch)!=NULL;  // found
			q->srch = bSch ? 1:0;
			if (bSch)  ii++;
			free(name);
		}
		pl->iSrch = ii;  iSrchAll += ii;
	}
	bShowSrch = 1;
}


//	. . . . . . . . . . . . . . . . . .  Edit Keys  . . . . . . . . . . . . . . . . . . 
const static char chShNum[11]=")!@#$%^&*(";
bool cAmp::KeysEdit(bool shift, WPARAM k)
{
	//  Edit Tab Name
	switch(k)  {	// strlen, left,right,ctrl, home,end ...
		//  <del
		case VK_BACK:  ied--;  if (ied<0) ied=0;  sed[ied]=0;  fTi=0.5f;  rt
		
		case VK_ESCAPE: case VK_F2: case VK_F3:  GuiOff();  rt	//  exit

		case VK_RETURN: //  accept
			switch(ed)	{
				case ED_nTab:
				{	scpy(pls->name, sed);	GuiOff();
					pls->Save();  SetSave();  }  rt
				case ED_nFind:
				{	scpy(srch, sed);  GuiOff();  DoSearch();  }  rt
			}	rt

		default:	//  add char
		{	if (ied >= (ed==ED_nTab?NamePls:NameSrch)-1)  rt
			char c = 0;
			//  key char codes
			if (k>='0' && k<='9'){	if (shift) c=chShNum[k-'0']; else c=k;  }  else
			if (k>='A' && k<='Z'){	if (shift) c=k; else c=k-'A'+'a';  }  else
			switch (k)
			{
				case VK_SPACE:		c=' ';  break;
				case VK_OEM_3:		if (shift) c='~'; else c='`';  break;
				case VK_OEM_MINUS:	if (shift) c='_'; else c='-';  break;
				case VK_OEM_PLUS:	if (shift) c='+'; else c='=';  break;
				case VK_OEM_4:		if (shift) c='{'; else c='[';  break;
				case VK_OEM_6:		if (shift) c='}'; else c=']';  break;
				case VK_OEM_7:		c='\'';  break;
				case VK_OEM_PERIOD:	c='.';  break;
				case VK_OEM_COMMA:	c=',';  break;
			}
			if (c==0)  rt	//  write char
			sed[ied]= (char)c;  ied++;  sed[ied]=0;  fTi=0.5f;
			//... le ri del bck
		}  rt
	}	rf
}

//								. . . . .  Tabs  . . . . .
void cAmp::tabPrev(bool row, bool ofs)
{
	int l = vPlst.size()-1;
	if (ofs)
	{	int d= row ? 1 : view.xNpt;
		view.ofsTab -= d;  if (view.ofsTab<0) view.ofsTab=0;
		return;
	}
	pls->Save();  // changed only-
	plsId -= (row && l > view.xNpt) ? view.xNpt : 1;
	if (plsId < 0)	plsId += l+1;
	plsChg();
}
void cAmp::tabNext(bool row, bool ofs)
{
	int l = vPlst.size()-1;
	if (ofs)
	{	int d= row ? 1 : view.xNpt;
		view.ofsTab += d;  if (view.ofsTab>l) view.ofsTab-=d;
		return;
	}
	pls->Save();
	plsId += (row && l > view.xNpt) ? view.xNpt : 1;
	if (plsId > l)  plsId -= l+1;
	plsChg();
}

//  util
void cAmp::plsChg(int clrSel)
{
	pls = vPlst[plsId];  updSelId(clrSel);
}
void cAmp::plsPlChg(int id)
{
	plsPlId = id;  plsPl = vPlst[plsPlId];
}

void cAmp::updSelId(int clear)
{
	if (pls->numSel > 0)
	{	plsSelId=plsId;  plsSel=pls;  }
	else if (clear)
	{	plsSelId=-1;  plsSel=NULL;  }
}

void cAmp::clrSelId()
{
	if (pls==plsSel)
	{  plsSelId=-1;  plsSel=NULL;  }
}

///  Move tab  . . . . . . . 
void cAmp::tabMove(int n)
{
	if (vPlst.size() <= 1 || n == plsId)  return;
	/**/int e=0;  if (plsPlId == plsId)  e=1;  // move pl
	/**/int s=0;  if (plsSelId == plsId) s=1;  // move sel
	
	int l = vPlst.size()-1; //last
	if (plsId == l)
		vPlst.pop_back();
	else
		vPlst.erase(vPlst.begin() + plsId);

	if (n == l)  {
		vPlst.push_back(pls);	plsId = vPlst.size()-1;  }
	else  {
		vPlst.insert(vPlst.begin() + n, pls);	plsId = n;  }
	plsChg();

	/**/if (e==1)  plsPlChg(plsId);  else
	for (int i=0; i<=l; ++i)  // find plsPl
		if (plsPl==vPlst[i])  plsPlChg(i);
	/**/if (s==1)  updSelId(1);  else  //{  plsSelId = plsId;  plsSel = vPlst[plsPlId];  }  else
	for (int i=0; i<=l; ++i)  // find plsPl
		if (plsSel==vPlst[i])  {  plsSelId = i;  plsSel = vPlst[plsSelId];  }
}

///  New tab  . . . . . . . 
void cAmp::tabNew(int m)
{
	pls->Save();
	CList* pl = new CList();	p(pl->name) "%d", ++cntrPls);
	if (m==2)
	{	//  ctrl- at end
		vPlst.push_back(pl);
		plsId = vPlst.size()-1;  pls = vPlst[plsId];  }  // sel
	else
	{	//  none- after cur   alt- before cur   shift- at begin
		vPlst.insert(vPlst.begin()+ (m==-2? 0: (plsId+ (m==-1?0:1))), pl);
			// pl +-
		if (m==-2 || m==1 && plsId < plsPlId || m==-1 && plsId <= plsPlId)
		{  plsPlId++;  plsPl = vPlst[plsPlId];  }  // pl
			// sel +-
		if (plsSelId != -1)
		if (m==-2 || m==1 && plsId < plsSelId || m==-1 && plsId <= plsSelId)
		{  plsSelId++;  plsSel = vPlst[plsSelId];  }  // sel
		// cur +-
		if (m==-2)  plsId=0;  else
		if (m==1)  plsId++;		plsChg();
	}
}
	
///  Close tab  . . . . . .
void cAmp::tabClose()
{
	if (vPlst.size() <= 1)  return;  //else plst.Clear();
	pls->Save();
	/**/int e=0;  if (plsPl == pls)  e=1;  // close pl
	/**/int s=0;  if (plsSel == pls) s=1;  // close sel
	if (plsId <= plsPlId)  plsPlId--;  // pl-  after del
	if (plsSelId > 0 &&
		plsId < plsSelId)  {  plsSelId--;  // sel-
			plsSel = vPlst[plsSelId];	}
	
	if (plsId == vPlst.size()-1)
	{	DEL(pls);	vPlst.pop_back();	plsId--;  }
	else
	{	DEL(pls);	vPlst.erase(vPlst.begin() + plsId);  }
		
	pls = vPlst[plsId];  //plsChg(1);
	/**/if (e==1)  plsPlId = plsId;
	/**/if (s==1)  updSelId(1);
	plsPl = vPlst[plsPlId];
}		


//  open playlist dialog . . .

void cAmp::tabOpen()
{
	OPENFILENAMEA ofn;  ZeroMemory(&ofn,sizeof(ofn));  ofn.lStructSize = sizeof(ofn);
	char szFile[MP], sDir[MP];	ofn.hwndOwner = hWnd;

	ofn.lpstrFile = szFile;  ofn.lpstrFile[0]='\0';  ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "cAmp Playlists (*.cp)\0*.cp\0";	ofn.nFilterIndex=1;
	ofn.lpstrFileTitle = NULL;	ofn.nMaxFileTitle = 0;

	scpy(sDir, cOsc::appPath);  sadd(sDir, "playlists");
	ofn.lpstrInitialDir = sDir;
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_ENABLESIZING|
		OFN_NOCHANGEDIR|OFN_NONETWORKBUTTON|OFN_HIDEREADONLY/*|OFN_ALLOWMULTISELECT*/;

	if (GetOpenFileNameA(&ofn)==TRUE)  {
		char* pp = strrchr(ofn.lpstrFile,'\\')+1, *pe = strrchr(ofn.lpstrFile,'.');  pe[0]=0;  //no dir,ext
		//dont duplicate pls names--
		scpy(pls->name, /*ofn.lpstrFile*/pp);  pls->Load();  }
}
