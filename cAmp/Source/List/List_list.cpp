#include "header.h"
#include "List.h"
#include "..\main\App.h"


///  Move
//--------------------------------------------------------------------
//  CopySel ... 
void CList::MoveSel(int m, pTrk npos/*dest*/, CList* pL/*from*/)
{
	bool oth = pL!=NULL;
	if (pL==NULL)  pL = this;

	if (pL->ll==NULL)  return;  //empty
	pTrk first=NULL,last=NULL;

	//  create sel list
	int sel = 0;
	pTrk q = pL->ll, back = NULL;
	while (q)
	{
		pTrk qn = q->next;
		if (!q->isDir() && q->sel > 0)
		{
			if (!first)  first = q;  //1st sel
			last = q;  //last sel

			del(q);  //remove
			if (oth)  {  /*q->sel = 0;*/  sel++;  }
			//^ ..if(!copy) else new Trk
			
			if (back)
			{	q->prev = back;
				back->next = q;  }  //make list
			back = q;
		}
		q = qn;
	}
	if (!first)  return;  //none sel

		pTrk ls = NULL;  //_move top
		if (oth && !first->prev)
			ls = last->next;

	insertList(m, npos, first,last);

		if (ls)  //_upd ll from
		{	ls->prev = NULL;
			pL->ll = ls;	}

	if (oth)  //+-  sel Add
	{	numSel += pL->numSel;
		selSize += pL->selSize;  selTime += pL->selTime;
		pL->unSel0();  }
	
	listUpd(0);
	if (oth)
		pL->listUpd(0);
}

void CList::Move1(int m, pTrk npos/*dest*/)
{
	if (ll==NULL)  return;  //empty

	pTrk cur;//, first,last;
	cur = vList[lCur];  //one
	if (cur == npos)  return;
	if (cur->isDir())  {
		if (m==-1) Up(1);  else  if (m==1) Dn(1);  return;  }

	pTrk n = npos;
	if (n == NULL)  {  // move cur
		if (m < 0 && cur == ll)  return;  //ll up, end down
		n = cur;  cur = NULL;

		if (m==1)  cur = n->next;  else  cur = n->prev;
		if (!cur)  return;//-

		del(n);  // del
		ins(m, cur, n);
	} else {
		del(cur);  // del
		ins(m, n, cur);  }

	if (m==-1) Up(1);  else  if (m==1) Dn(1);
	listUpd(0);
}


///  Insert
//--------------------------------------------------------------------
void CList::Insert1(int m, pTrk nt)
{
	if (ll==NULL)  return;  //empty
	pTrk cur = vList[lCur];  //cur
	if (cur->isDir())  {  Dn(1);  return;  }

	//pTrk prv = cur->prev, nxt = cur->next;
	pTrk n;
	if (nt == 0) {  nt = cur;  //0-dupl
		n = new CTrk(nt->name, nt->path);  // copy
		n->type = nt->type;  n->tab = nt->tab;  n->ext = nt->ext;
		n->time = nt->time;  n->size = nt->size;  n->rate = nt->rate;  }
	else  n = nt;

	ins(m, cur, n);  // ins
	itu++;
	listUpd(0);
}

void CList::ins(int m, pTrk cur/*dest*/, pTrk n/*what*/)
{
	if (ll==NULL)  m = -2;  //empty
	if (m==-1)  {  // above cur
		pTrk prv = cur->prev;
		if (prv)  prv->next = n;  else  ll = n;  //1st
		n->prev = prv;
		n->next = cur;
		cur->prev = n;  //Dn(1);
	} else
	if (m==1)  {  // after cur
		pTrk nxt = cur->next;
		cur->next = n;
		n->prev = cur;
		n->next = nxt;
		if (nxt)  nxt->prev = n;  //last
	} else
	if (m==-2)  {	// at top
		n->prev = NULL;
		n->next = ll;
		if (ll)  ll->prev = n;
		ll = n;  //1st
		Dn(1);
	} else
	if (m==2)  {  // at end
		pTrk end = ll;  while (end->next)  end = end->next;
		if (end)
		{	end->next = n;
			n->prev = end;  }
		n->next = NULL;  //last
	}
}

void CList::insertList(int m, pTrk npos/*dest*/, pTrk first,pTrk last)
{
	if (ll==NULL)  m = -2;  //empty
	if (m==-1)  {  // above npos
		if (!npos)  return;
		pTrk prv = npos->prev;

		if (prv)  prv->next = first;  else  ll = first;  //1st
		first->prev = prv;
		last->next = npos;
		npos->prev = last;
	} else
	if (m==1)  {  // after npos
		if (!npos)  return;
		pTrk nxt = npos->next;

		npos->next = first;
		first->prev = npos;
		last->next = nxt;
		if (nxt)  nxt->prev = last;  //last
	} else
	if (m==-2)  {	// at top																										
		first->prev = NULL;
		last->next = ll;
		if (ll)  ll->prev = last;
		ll = first;  //1st
	} else
	if (m==2)  {  // at end
		pTrk end = ll;  while (end->next)  end = end->next;
		if (end)
		{	end->next = first;
			first->prev = end;  }
		last->next = NULL;  //last
	}
}


///  Delete
//--------------------------------------------------------------------
void CList::Del(bool disk)	// one
{
	if (ll==NULL)  return;  //empty
	pTrk cur = vList[lCur];  //cur
	if (cur->isDir())  {  Dn(1);  return;  }
	
	// ... disk del plTrk
	del(cur, disk);  DEL(cur)  //  del trk
	listUpd(0);
}

void CList::del(pTrk q, bool disk)
{
	pTrk prv = q->prev, nxt = q->next;
	
	if (q && disk)
	{
		char s[MP];  q->getFullName(s);
		if (DeleteFileA(s)==FALSE)  {	//FormatMessageA()
			DWORD er = GetLastError();
			if (er == ERROR_FILE_NOT_FOUND || er == ERROR_PATH_NOT_FOUND)
				Info(s,"Can't delete file (File/path not found")
			else if (er == ERROR_ACCESS_DENIED)
				Info(s,"Can't delete file (Access denied)")
			else
				Info(s,"Can't delete file")  }
	}

	if (q == ll) {  ll = nxt;	if (nxt) nxt->prev = NULL;  }  //1st
	else  {
		if (prv) {  prv->next = nxt;  }
		if (nxt) {  nxt->prev = prv;  }  }
}

void CList::DelSel(bool disk)  // selected
{
	if (ll==NULL)  return;  //empty

	pTrk q = ll;
	while (q)
	{
		pTrk qn = q->next;  //cur
		if (!q->isDir() && q->sel > 0)
		{
			del(q, disk);  DEL(q)  // del trk
		}
		q = qn;
	}
	unSel0();
	listUpd(0);
	
	//if (trRem)	
	//	UpdTimes();/* * * * */
	//trRem = false;
}


///  Insert Dir  thread  - - - - - - - - - - - - - - - - - - 

DWORD WINAPI DirThread(LPVOID lpParam)
{ 
	CList* pL = (CList*)lpParam;
	pL->treeCrt();
	pL->bThr = false;
	pL->bbThr = false;

	//if (pL->trRem)
	//	pL->DelSel();/* * * * */

	App::pAmp->bDrawPlst2 = true;
    return 0;
}

void CList::destThrTi()
{
	if (bThrTi)
	{	bThrTi = false;
		TerminateThread(thrTi, 1);  }
}

void CList::InsertDir(char* Path) /*+*/
{
	//if (!Path)  return;
	if (Path)  {
		scpy(sPath, Path);  // end with \ 
		if (sPath[strlen(sPath)-1]!='\\')  sadd(sPath,"\\");	}

	if (bThr)  {	//WaitForSingleObject(thr, 100);
		if (bThr)  return;  }
	destThrTi();
	
	thr = CreateThread(NULL,0,DirThread,(LPVOID)this,0,NULL);
	bbThr = true;
	bThr = true;
}

//void CList::WaitThr()
//{
//	if (bThr)
//		WaitForSingleObject(thr, 100);
//}

void CList::destroyThr()
{
	if (bThr)
	{	bThr = false;
		TerminateThread(thr, 1);  // kill on app exit
		bbThr = false;
		listCrt();	// for deleteList
	}
	destThrTi();	//^listCrt(0)..
}