#include "header.h"
#include "List.h"
#include "..\main\App.h"


//  get tkPl index  ---

int CList::getTkPlIdx()  //on unhid
{
	int i=0,id=-1;  pTrk tkPl = vList[idPl];
	pTrk q=ll;	while (q)  {  if (tkPl==q) id=i;  q=q->next;  ++i;  }
	return id;
}

void CList::GotoPlay()  // move cursor to playing
{
	lCur = idPl;  Center();
}
void CList::Center()
{
	lOfs = lCur-Lin/2;
	zOfs();  Dn(0);  Up(0);
}

//  select  --------------------------------------------------------------------

void CList::Pick(int cr)	// pick
{	lCur = cr;  zCur();  ylastSel = cr;  }

void CList::sel(int ivL, bool sel)
{
	int i = ivL;	pTrk t = vList[i];
	if (t->isDir())  return;  //
	if (sel)  // select
	{	t->sel = 1;  selSize += t->size;  selTime += t->time;	++numSel;	}
	else	// unselect
	{	t->sel = 0;  selSize -= t->size;  selTime -= t->time;	--numSel;	}
}

void CList::SelDir(int cr)  // select dir
{
	lCur = cr;  zCur();  int lc = lCur;
	if (!vList[lCur]->isDir())  return;
	lCur++;
	while (lCur < listLen && !vList[lCur]->isDir())
	{
		sel(lCur, 1 - vList[lCur]->sel);
		lCur++;
	}
	lCur = lc;
}

void CList::SelRange(int cr, bool un)
{
	if (ylastSel <= 0)  return;

	int a = min(ylastSel,cr), b = max(ylastSel,cr);

	if (un)  //  unsel range
	{	for (int i = a; i <= b; ++i)
			if (vList[i]->sel)  sel(i, 0);
	}else{
		for (int i = a; i <= b; ++i)
			if (!vList[i]->sel)  sel(i, 1);
	}
	Pick(cr);
}

void CList::Select1(int cr)
{
	if (vList[cr]->isDir())  // sel dir
		SelDir(cr);
	else  {
		sel(cr, 1 - vList[cr]->sel);
		Pick(cr);  }
}


///  get Track Times  --------------------------------------------------------------------

DWORD WINAPI TimThread(LPVOID lpParam)
{ 
	CList* pL = (CList*)lpParam;
	pL->updTi();
	pL->bThrTi = false;
	App::pAmp->bDrawPlst2 = true;
    return 0;
}

void CList::UpdTimes()
{
	//if (bThr)  return;//
	destThrTi();
	thrTi = CreateThread(NULL,0,TimThread,(LPVOID)this,0,NULL);
	bThrTi = true;
}

void CList::updTi()
{
	HSTREAM chan;  char nf[MP];
	allTime = 0.0;

	for (itu=0; itu<listLen; ++itu)
	{	pTrk q = vList[itu];  if (q)  if (q->type == TY_AUDIO)  {

		if (q->time == 0.0)
		{	q->getFullName(nf);
		
			if ( chan = BASS_StreamCreateFile(FALSE, nf, 0,0,0) )
			{	QWORD bytes = BASS_ChannelGetLength(chan,BASS_POS_BYTE);
				double time = BASS_ChannelBytes2Seconds(chan,bytes);
				q->time = time;
				BASS_StreamFree(chan);  }
			/*else  // ti get error ..
			{	char s[200];
				p(s)"(error code: %nx)",BASS_ErrorGetCode());
				MessageBoxA(0, s, "bass", MB_OK|MB_ICONWARNING);
			}/**/
			//Sleep(200);//
		}
		allTime += q->time;
	}  }
}


//  Ext Find  -------

bool CList::ExtFind(const char* ex, int iExtCnt, const ExtTab extArr)
{
	bool dn = true;  exf = 0;
	while (dn && exf < iExtCnt)
	{
		if (_stricmp(ex, extArr[exf]) == 0)
			rt  //{	rt  dn = false;  }
		exf++;
	}	rf
}


//  Next >  -------------------------------------------------------------------------

bool CList::Next(pTrk& q)
{
	/* next */
	if (q->p)  q = q->p;  else
	if (q->nx)  q = q->nx;
	else
	{	/*back*/
		do  q = q->pv;  while (q && !q->nx);
		if (q && q->nx)  q = q->nx;  else  rt  // end
	}	rf
}

bool CList::NextLev(pTrk& q)
{
	/* next */
	if (q->p) {  q = q->p;  lev++;  }  else
	if (q->nx)  q = q->nx;
	else
	{	/*back*/
		do {  q = q->pv;  lev--;  } while (q && !q->nx);
		if (q && q->nx)  q = q->nx;  else  rt  // end
	}	rf
}


//  destroy list  --------------------------------------------------------------------

void CList::destList()
{
	pTrk q = ll, qd;
	while (q)
	{	qd = q->next;
		DEL(q)	q = qd;
	}
	ll = NULL;  //end list

	destDirs();
}

/// auto save opts to new dirs-... new dir creation
void CList::destDirs()
{
	for (int i=0; i<vDirs.size(); i++)
		DEL(vDirs[i])
}


//  ctors  --------------------------------------------------------------------------

CList::CList()
{	tt=NULL; ww=NULL; ll=NULL;
	lCur=0; lOfs=0;  listLen=0;  idPl=0;

	allSize=0; dirSize=0; allTime=0; allDirs=0; allFiles=0;
	selSize=0; selTime=0;

	sPath[0]=0;  pp[0]=0;  srchPath[0]=0;  ss[0]=0;
	lev=0;  itu=0;  bThr=0; bThrTi=0;  bbThr=0;
	lInsPos=0; lInsM=-1;  ylastSel, numSel=0;  InsM=-2;//top
	iRFilt=-cR0; iRFilU=cR1; //all
	name[0]=0;  trGet=true;  iSrch=0;  trRem=false;
}
CList::~CList() {	destroyThr();	destList();  }

CTrk::CTrk(const char* Name, const char* Path)
{	nx = p = pv = NULL;	name = NULL;  path = NULL; prev = next = NULL;  path2 = NULL;
	time = 0; size = 0;  ext = 0;  sel = 0;  srch = 0;
	type = 0; tab = 0;  hide = 0;  rate = 0;  dis = 0;  bokm = 0;
	
	int l = strlen(Name)+1;  name = new char[l];  strcpy(name, Name);
	int r = strlen(Path)+1;  path = new char[r];  strcpy(path, Path);
	
	updName();
}

void CTrk::AddPath2(const char* Path2)
{
	int r = strlen(Path2)+1;  path2 = new char[r];  strcpy(path2, Path2);
}
CTrk::~CTrk()
{	DELA(name)  DELA(path)  DELA(path2)  }
