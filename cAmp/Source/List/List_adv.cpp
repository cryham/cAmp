#include "header.h"
#include "List.h"
#include "..\main\App.h"


//  bookmarks
//-----------------------------------------------------------------
void CList::Bookm(int lev)  // toggle
{
	if (ll==NULL)  return;
	if (vList[lCur]->bokm == lev)  vList[lCur]->bokm = 0;
	else  vList[lCur]->bokm = lev;
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
	if (!dn)  lCur == old;
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
	if (!dn)  lCur == old;
	Center();
}

///  Rename file with rating  ------------------------------------------------------------------------

bool CList::RenameRate(pTrk t)
{
	if (ll==NULL || t==NULL || t->dis > 0)  rf
	
	char o_rate = 0;  BYTE o_bokm = 0;
	size_t posB = 0;
	getNameRating(t->name, &o_rate, &o_bokm);
	char old[MP];  t->getFullName(old);
	
	// different
	if (t->rate != o_rate || t->bokm != o_bokm)
	{
		// clear old rating from name
		char s[MP];  scpy(s, t->name);
		int l = strlen(s);  // no ext
		
		if (o_rate != 0)  // are chR's in name
		{	bool dn=true;
			int p = l-1;
			while (dn && p >= 0)
			{
				bool fn = false;
				for (int i=0; i < chFnAll; ++i)
				if (s[p] == cFnCharRates[i])
					fn = true;
				if (fn)  p--;  else  dn=false;  //back
			}
			s[p+1]=0;  //end
		}
		if (o_bokm > 0)
		{
			l = strlen(s);
			if (l >= 2 && s[l-2]=='%' /*&& s[]*/)
				s[l-2]=0;  //end
		}
		
		// bookmark
		if (t->bokm > 0)
		{	char b[3] = {'%', t->bokm+'0', 0};
			sadd(s, b);  }

		// add rating
		if (t->rate != 0)
			sadd(s, chFRate[mia(0,chRall, t->rate+cR0)]);
		
		char name[MP+1];  //t->getFullName(name);
		StringCbPrintfA(name,MP, "%s%s.%s", t->path, s, ExtAudM[t->ext]);

		// rename
		if (t->isDir())
		{
		
		}
		if (MoveFileA(old, name)==FALSE)
		{
			DWORD e = GetLastError()&0xFFFF;
			p(s)"%s\nto:\n%s\nError: %d %s", old, name, e,
				e==ERROR_SHARING_VIOLATION? "file is opened":
				e==ERROR_FILE_NOT_FOUND? "file not found":
				e==ERROR_PATH_NOT_FOUND? "path not found" :"");
			if (e==ERROR_FILE_NOT_FOUND || e==ERROR_PATH_NOT_FOUND)
				t->dis = 1;
			Info(s,"Can't rename file");
		}
		else
		{	// change trk name
			l = strlen(s)+1;  DELA(t->name)
			t->name = new char[l];  strcpy(t->name, s);
			rt
		}
	}	rf
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
{	numSel = 0;  selSize = 0;  selTime = 0;  }


namespace bfs = boost::filesystem;

void CList::CopySelFiles()
{
	pTrk q = ll;
	while (q)
	{
		if (q->sel)
		{
			char pa[MP],to[MP],tp[MP];  q->getFullName(pa);
			scpy(to,pa);  to[0] = 'd';  // drive letter
			scpy(tp,q->path);  tp[0] = 'd';
			///TODO from set.xml ..
			//int i = strlen(tp);
			//if (i > 0)  tp[i-1] = 0;

			try
			{
				bfs::create_directories(tp);

				if (!bfs::exists(to))
					bfs::copy_file(pa, to);  //bfs::copy_option::overwrite_if_exists
			}
			catch (const bfs::filesystem_error & ex)
			{
				MessageBoxA(0, ex.what(), "copy fail", 0);
			}
		}
		q = q->next;
	}
}



///  list update  ------------------------------------------------------------------------

void CList::listUpd(bool bUpdCur)
{
	int i=0;  char hid = 0;
	pTrk q = ll, qo = NULL;

	//save lcur,lofs, idPl
	bool upd = false,uc=0;	pTrk tkPl=0, tkCur=0; int lOd=0;
	float fCur=0.f, fL= listLen-1; //rel
	
	if (vList.size() > 0)  {  upd = true;
		fCur = float(lCur)/fL;//rel
		if (idPl > vList.size()-1)  idPl = vList.size()-1;//
		tkPl = vList[idPl];  tkCur = vList[lCur];  lOd = lOfs-lCur;  }
	// move cur,idPl so they'd be in upd--
	// show/hide empty dirs, one file dirs opts...
	
	vList.clear();  listLen = 0;  destDirs();
	allDirs = 0;  allFiles = 0;  allTime = 0;  allSize = 0;
	
	while (q)
	{
		pTrk d = NULL;	//  diffrent path, opt-
		if (q==ll || strcmp(q->path,qo->path) != 0)
		{
			// get last sub dir
			char p[MP];  scpy(p, q->path);  p[strlen(p)-1]=0;
			char* pe = strrchr(p,'\\');  if (!pe)  {  scpy(p,"no dir");  pe = p;  }
			d = new CTrk(pe+1, q->path);

			//  subdir-1
			pe[0] = 0;
			char* p2 = strrchr(p,'\\');
			if (p2)  d->AddPath2(p2+1);
			
			vDirs.push_back(d);  //for delete
			
			vList.push_back(d);  listLen++;  i++;  /// Add Dir
			allDirs++;	hid = 0;
		}
		if (d)  d->next = q;  //h+
		
		if (q->hide > 0)  hid = q->hide;  //opt for hide/show dir
		if (hid==0 && (q->rate >= iRFilt && q->rate <= iRFilU) || hid==2)  // rating filtering
		{	vList.push_back(q);

			if (upd)  {
				if (q==tkPl)  idPl = i;  //_ wont work on dirs
				if (q==tkCur && bUpdCur)
				{  lCur = i;  lOfs = lCur+lOd;  uc=1;  }  }

			listLen++;  i++;  /// Add File
			if (bFilInf)	{	allFiles++;  allTime += q->time;  allSize += q->size;	}  // filtered
		}
		if (!bFilInf)	{	allFiles++;  allTime += q->time;  allSize += q->size;	}  // all

		qo = q;  q = q->next;
	}
	
	fL = float(listLen-1);
	if (!uc && upd && bUpdCur)  {  lCur = fCur*fL;  lOfs = lCur+lOd;  }
	zCur();  zOfs();  //range
	if (idPl > listLen-1)   idPl = listLen-1;
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


bool CList::Home	(int m){	switch(m) {
	case 2:  lCur = 0;  lOfs = 0;  rt  //list
	case 1:  lCur = lOfs;  rt  //view
	case 0:  if (/*vList[lCur]->prev &&*/ lCur > 1)  {
		if (vList[lCur-1]->isDir() || vList[lCur]->isDir())
		{	do  lCur--;  while (lCur > 0 && vList[lCur]->isDir());  }
		else
		{	while (lCur-1 > 0 && !vList[lCur-1]->isDir())  lCur--;  }
		Up(0);	} rt	//srch prev^/next_ dir in list
}  rf}

bool CList::End		(int m){	switch(m) {
	case 2:  lCur = listLen-1;  lOfs = listLen-1;  zCur(); zOfs();  rt  //list
	case 1:  lCur = lOfs+Lin-1;  zCur(); rt  //view
	case 0:  if (/*vList[lCur]->next &&*/ lCur < listLen-1)  {
		if (vList[lCur+1]->isDir() || vList[lCur]->isDir())
		{	do  lCur++;  while (lCur < listLen && vList[lCur]->isDir());  }
		else
		{	while (lCur+1 < listLen && !vList[lCur+1]->isDir())  lCur++;  }
		Dn(0);	} rt
}  rf}
//--------------------------------------------------------------------
