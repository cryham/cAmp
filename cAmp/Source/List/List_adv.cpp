#include "header.h"
#include "List.h"
#include "..\main\App.h"


//  bookmarks
//-----------------------------------------------------------------
void CList::BookmPls(char add)
{
	bokm += add;  bokm = mia(0,6, bokm);
}
void CList::Bookm(char add)
{
	if (ll==NULL)  return;
	char b = vList[lCur]->bokm;
	b += add;  b = mia(0,6, b);
	vList[lCur]->bokm = b;
}

void CList::BookmPrev(int lev, bool bSrch)
{
	if (ll==NULL)  return;
	bool dn=false;  int subs=0, old = lCur;
	while (!dn && subs < listLen)
	{
		subs++;  lCur--;
		if (lCur < 0)  lCur = listLen-1;
		if (bSrch)	{  if (vList[lCur]->srch > 0)     dn=true;  }
		else		{  if (vList[lCur]->bokm >= lev)  dn=true;  }
	}
	if (!dn)  lCur = old;
	Center();
}
void CList::BookmNext(int lev, bool bSrch)
{
	bool dn=false;  int adds=0, old = lCur;
	while (!dn && adds < listLen)
	{
		adds++;  lCur++;
		if (lCur > listLen-1)  lCur = 0;
		if (bSrch)	{  if (vList[lCur]->srch > 0)     dn=true;  }
		else		{  if (vList[lCur]->bokm >= lev)  dn=true;  }
	}
	if (!dn)  lCur = old;
	Center();
}


//  rating Filtering

void CList::DecRFil(bool upr)
{
	if (upr) {	if (iRFilU > -cR0)  iRFilU--;  if (iRFilt > iRFilU)  iRFilt--;  }
	else	 {	if (iRFilt > -cR0)  iRFilt--;  }
	listUpd(1);
}
void CList::IncRFil(bool upr)
{
	if (upr) {	if (iRFilU < cR1)  iRFilU++;  }
	else	 {	if (iRFilt < cR1)  iRFilt++;  if (iRFilt > iRFilU)  iRFilU++;  }
	listUpd(1);
}

///  cur track rating
void CList::DecRate()
{
	if (ll==NULL)  return;
	if (vList[lCur]->rate > -cR0)  vList[lCur]->rate--;
}

void CList::IncRate()
{
	if (ll==NULL)  return;
	if (vList[lCur]->rate < cR1)  vList[lCur]->rate++;
}

///  playing rating
void CList::DecRatePl()  // plsPl
{
	if (ll==NULL)  return;
	if (vList[idPl]->rate > -cR0)  vList[idPl]->rate--;
}

void CList::IncRatePl()
{
	if (ll==NULL)  return;
	if (vList[idPl]->rate < cR1)  vList[idPl]->rate++;
}


//  hide-/show+ dir  >    hide/show file...
void CList::Hide(bool show)
{
	if (ll==NULL)  return;  //empty
	pTrk cur = vList[lCur];  //cur
	if (!cur->isDir())  return;

	cur = cur->next;  //next trk val
	if (!cur)  return;
		
	if (cur->hide == 0)  cur->hide = show ? 2 : 1;
	else  cur->hide = 0;
	listUpd(1);
}

//  unselect all
void CList::UnSel()
{
	pTrk q = ll;
	while (q) {  q->sel = 0;  q = q->next;  }  // clear sel
	unSel0();
}
void CList::unSel0()
{
	numSel = 0;  selSize = 0;  selTime = 0;
}


//  list move (keys)  --------------------------------------------------------------------
int CList::Lin = 8, CList::iDirView = 0;  bool CList::bFilInf = true;

bool CList::zCur(){		lCur = mia(0, listLen-1, lCur);    rt}
bool CList::zOfs(){		if (lOfs > listLen-Lin)  lOfs = listLen-Lin;  if (lOfs < 0)  lOfs = 0;  rt}

bool CList::Up		(int m){	lCur -= m;  zCur();  int d= lCur-lOfs;        if (d < 0) {  lOfs += d;  zOfs();  }  rt}
bool CList::Dn		(int m){	lCur += m;  zCur();  int d= lCur-lOfs-Lin+1;  if (d > 0) {  lOfs += d;  zOfs();  }  rt}

bool CList::PgOfsUp	(int m){	lOfs -= m;  zOfs();  int d= lCur-lOfs-Lin+1;  if (d > 0) {  lCur -= d;  zCur();  }  rt}
bool CList::PgOfsDn	(int m){	lOfs += m;  zOfs();  int d= lCur-lOfs;        if (d < 0) {  lCur -= d;  zCur();  }  rt}
bool CList::PgUp	(int m){	lCur -= m;  lOfs -= m;  zCur();  zOfs();  rt}
bool CList::PgDn	(int m){	lCur += m;  lOfs += m;  zCur();  zOfs();  rt}


bool CList::Home	(int m)
{	switch(m)
	{
	case 2:  lCur = 0;  lOfs = 0;  rt  //list
	case 1:  lCur = lOfs;  rt  //view

	case 0: 
		if (/*vList[lCur]->prev &&*/ lCur > 1)
		{	if (vList[lCur-1]->isDir() || vList[lCur]->isDir())
			{	do  lCur--;  while (lCur > 0 && vList[lCur]->isDir());  }
			else
			{	while (lCur-1 > 0 && !vList[lCur-1]->isDir())  lCur--;  }
			Up(0);
		}  rt	//srch prev^/next_ dir in list
	}  rf
}

bool CList::End		(int m)
{	switch(m)
	{
	case 2:  lCur = listLen-1;  lOfs = listLen-1;  zCur(); zOfs();  rt  //list
	case 1:  lCur = lOfs+Lin-1;  zCur(); rt  //view

	case 0:
		if (/*vList[lCur]->next &&*/ lCur < listLen-1)
		{	if (vList[lCur+1]->isDir() || vList[lCur]->isDir())
			{	do  lCur++;  while (lCur < listLen && vList[lCur]->isDir());  }
			else
			{	while (lCur+1 < listLen && !vList[lCur+1]->isDir())  lCur++;  }
			Dn(0);
		}  rt
	}  rf
}
