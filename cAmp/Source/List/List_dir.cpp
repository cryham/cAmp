#include "header.h"
#include "List.h"


char CList::getNameRating(pTrk qf)  //fd.name, qf not used
{	//  get rating  in file name  find # + ~ ^ ` `- - -- =
	int len = strlen(fd.cFileName);  // no ext !
	if (len == 0/*<= 2*/)  return 0;
	char rate = 0;

	bool dn=true;	int r = 0;
	while (dn && r < chFnAll)  {
		if (fd.cFileName[len-1] == cFnCharRates[r])  {
			rate = cFnNumRates[r];
			if (rate==-1 && len >= 2)
			{	if (fd.cFileName[len-2] == cFnCharRates[r])   rate=-2;  //- to --
				if (fd.cFileName[len-2] == cFnCharRates[r+1])  rate=1;  //`- to `
			}
			dn = false;  }
		r++;  }

	return rate;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 File

void CList::tree1File(/*const char* name,*/ pTrk* tf, pTrk* qf)
{
	//  .Ext
	char* pe = strrchr(/*name*/fd.cFileName,'.');
	if (pe)  // no ext
	if (strlen(pe+1) < ExtLen)	// too long
	{
		char ex[6];  strcpy(ex, pe+1);

		//  ext type find
		BYTE ty = TY_FILE;
		if (ExtFind(ex,iExtAud,ExtAud))  {  ty = TY_AUDIO;  }  //else
		//if (ExtFind(ex,iExtPls,ExtPls))  {  ty = TY_PLS;  } else
		//if (ExtFind(ex,iExtImg,ExtImg))  {  ty = TY_IMG;  allImg++;  }

		if (/*add*/ty == TY_AUDIO)
		{
			//  size
			INT64 si = fd.nFileSizeHigh*smax+ fd.nFileSizeLow;
			allSize += si;	dirSize += si;
			
			allFiles++;  pe[0]='\0';//
			
			pTrk n = new CTrk(fd.cFileName, pp);
			if (*qf)  (*qf)->nx = n;  else  /*1st*/*tf = n;  (*qf) = n;  //list
			(*qf)->pv = ww;   (*qf)->time = 0.0;
			(*qf)->type = ty;  (*qf)->size = si;  (*qf)->tab = lev;  (*qf)->ext = exf;
			(*qf)->rate = getNameRating(*qf);
		}
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 Dir drop
#if 0
pTrk CList::tree1Dir(pTrk* t, pTrk* q)
{
	pTrk  /*t=NULL, q=NULL,*/  tf=NULL, qf=NULL;
	dirSize = 0;  allImg = 0;
	
	/* Path */
	char pp1[MP];
	strcpy(pp,sPath);  strcat(pp,subPath);  scpy(pp1,pp);	strcat(pp1,"*.*");
	hs = FindFirstFileA(pp1,&fd);
	if (hs == INVALID_HANDLE_VALUE)  return t;
	
	bool done = true;
	while (done)
	{
		if (fd.cFileName[0] != '.')
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  //  Dir
			{
				pTrk n = new CTrk(fd.cFileName, pp);
				if (q)  q->nx = n;  else  /*1st*/t = n;  q = n;
				q->pv = ww;  q->type = TY_DIR_unv;
				q->size = 0;  q->tab = lev;
				q->rate = getNameRating(q);  // dir rating
			}
			else  //  File
				tree1File(&tf, &qf);
		}
		if (!FindNextFileA(hs,&fd))  done = false;
	}
	FindClose(hs);
	
	//  add files
	if (tf) if (q)  q->nx = tf;  else  t = tf;
	return t;
}
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 Dir

pTrk CList::tree1Dir(const char* subPath)
{
	pTrk  t=NULL, q=NULL,  tf=NULL, qf=NULL;
	dirSize = 0;
	
	/* Path */
	char pp1[MP];
	strcpy(pp,sPath);  strcat(pp,subPath);  scpy(pp1,pp);	strcat(pp1,"*.*");
	hs = FindFirstFileA(pp1,&fd);
	if (hs == INVALID_HANDLE_VALUE)  return t;
	
	bool done = true;
	while (done)
	{
		if (fd.cFileName[0] != '.')
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  //  Dir
			{
				pTrk n = new CTrk(fd.cFileName, pp);
				if (q)  q->nx = n;  else  /*1st*/t = n;  q = n;
				q->pv = ww;  q->type = TY_DIR_unv;
				q->size = 0;  q->tab = lev;
				q->rate = getNameRating(q);  // dir rating
			}
			else  //  File
				tree1File(&tf, &qf);
		}
		if (!FindNextFileA(hs,&fd))  done = false;
	}
	FindClose(hs);
	
	//  add files
	if (tf) if (q)  q->nx = tf;  else  t = tf;
	return t;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . Tree

void CList::treeCrt()
{
	pTrk q,o;  lev = 0;  ww = NULL;
	if (trGet)  tt = tree1Dir("");
	q = tt;  if (!q)  return;
	int m = 0;	lev = 1;
	
	bool done = true;
	while (done)
	{
		if (q->type == TY_DIR_unv)
		{
			q->type = TY_DIR;	ww = q;
			
			/* path+ */o = q;	srchPath[0] = 0;
			do {  strcpy(ss, o->name);  strcat(ss,"\\");
				strcat(ss,srchPath);  strcpy(srchPath,ss);  o = o->pv;  } while(o);

			q->p = tree1Dir(ss);
			
			/* add allSize */
			o = q;	do {
				o->size += dirSize;  o = o->pv;  } while(o);
			
			allDirs++;	m = 1;
			if (q->p) {  q = q->p;  lev++;  }
		}
		if (m == 1)  m = 0;
		else
		{	if (NextLev(q))  done = false;	}
	}
	
	listCrt();
}


//  tree -> list  1st, add
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CList::listCrt()
{
	if (tt)
	{
		pTrk q = tt, prv = NULL, ll1 = NULL;
		vector<pTrk> delDirs;
		
		bool done = true;
		while (done)
		{
			if (!q->isDir())
			{
				if (!ll1)  {  // new 1st
					ll1 = q;  prv = NULL;  }
				
				q->prev = prv;
				if (prv)
					q->prev->next = q;
				
				prv = q;
			}else
				delDirs.push_back(q);
			
			if (Next(q))  done = false;
		}

		insertList(InsM, getCur(), ll1,prv);  //ins
		
		// dirs from tree
		for (int i=0; i<delDirs.size(); ++i)
			DEL(delDirs[i])
		tt=NULL;  // end tree
		
		listUpd(1);
		UpdTimes();
	}
}


// . . . . . .  get cur util ^-

pTrk CList::getCur()
{
	pTrk cur = NULL;
	if (vList.size() > 0)
	{	cur = vList[lCur];
		if (cur->isDir() && lCur > 0)  cur = vList[lCur-1];
		if (cur->isDir() && lCur < listLen-1)  cur = vList[lCur+1];
		if (cur->isDir())  cur = NULL; //disp err.
	}
	return cur;
}
