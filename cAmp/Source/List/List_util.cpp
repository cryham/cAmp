#include "header.h"
#include "List.h"
#include "..\main\App.h"
using namespace std;


//  get tkPl index  ---

int CList::getTkPlIdx()  //on unhid
{
	int i=0,id=-1;
	pTrk tkPl = vList[idPl];

	pTrk q=ll;
	while (q)
	{	if (tkPl==q)  id = i;
		q = q->next;  ++i;
	}
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
		sel(lCur, 1 - vList[lCur]->sel > 0);
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
		sel(cr, 1 - vList[cr]->sel > 0);
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
	HSTREAM chan;
	allTime = 0.0;

	for (itu=0; itu<listLen; ++itu)
	{	pTrk q = vList[itu];  if (q)  if (q->type == TY_AUDIO)  {

		if (q->time == 0.0)
		{	string name = q->getFullPath();
		
			if ( chan = BASS_StreamCreateFile(FALSE, name.c_str(), 0,0,0) )
			{	QWORD bytes = BASS_ChannelGetLength(chan,BASS_POS_BYTE);
				double time = BASS_ChannelBytes2Seconds(chan,bytes);
				q->time = time;
				BASS_StreamFree(chan);  }
			//else  // ti get error ..
			//	log("ti error: "+iToStr(BASS_ErrorGetCode()));
			//Sleep(200);//
		}
		allTime += q->time;
	}  }
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
		delete q;  q = qd;
	}
	ll = NULL;  //end list

	destDirs();
}

///TODO: auto save opts to new dirs-... new dir creation
void CList::destDirs()
{
	for (size_t i=0; i<vDirs.size(); i++)
		delete vDirs[i];
	vDirs.clear();
}


//  ctors  --------------------------------------------------------------------------

CList::CList()
{	tt=NULL; ww=NULL; ll=NULL;
	lCur=0; lOfs=0;  listLen=0;  idPl=0;

	allSize=0; dirSize=0; allTime=0; allDirs=0; allFiles=0;
	selSize=0; selTime=0;

	sPath[0]=0;  pp[0]=0;  ss[0]=0;
	lev=0;  itu=0;  bThr=0; bThrTi=0;  bbThr=0;
	lInsPos=0; lInsM=-1;  ylastSel, numSel=0;  InsM=-2;//top
	iRFilt=-cR0; iRFilU=cR1; //all
	name[0]=0;  trGet=true;  iSrch=0;  trRem=false;
}
CList::~CList()
{
	destroyThr();	destList();
}

CTrk::CTrk(const string& Name, const string& Path)
	//
	:nx(0),pv(0),/*p(0),*/ prev(0),next(0)
	,time(0.0), size(0L)
	,sel(0), dis(0), tab(0)
	,type(TY_FILE), srch(0)
	,hide(0), rate(0), bokm(0)
	,name(Name), path(Path)
{
	p=0;
	updName();
}

CTrk::~CTrk()
{
}
