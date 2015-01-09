#include "header.h"
#include "List.h"
#include "..\main\App.h"
using namespace std;


///  Rename file with rating  ------------------------------------------------------------------------

bool CList::RenameRate(pTrk t)
{
	//  empty list or bad file
	if (ll==NULL || t==NULL || t->dis > 0)  rf
	
	char o_rate = 0;  byte o_bokm = 0;
	size_t posB = 0;
	getNameRating(t->name.c_str(), &o_rate, &o_bokm);
	string old = t->getFullPath();
	
	// different
	if (t->rate != o_rate || t->bokm != o_bokm)
	{
		// clear old rating from name
		char s[MP];  scpy(s, t->name.c_str());
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
		
		string name = t->path + s + "." + t->ext;

		// rename
		if (t->isDir())
		{
		
		}
		if (MoveFileA(old.c_str(), name.c_str())==FALSE)
		{
			DWORD e = GetLastError()&0xFFFF;
			string s = old + "\nto:\n" + name + "Error: " + cStr::iToStr(e) + " " +
				(e==ERROR_SHARING_VIOLATION? "file is opened":
				e==ERROR_FILE_NOT_FOUND? "file not found":
				e==ERROR_PATH_NOT_FOUND? "path not found" :"");
			if (e==ERROR_FILE_NOT_FOUND || e==ERROR_PATH_NOT_FOUND)
				t->dis = 1;
			App::pAmp->Err(string("Can't rename file: ")+s);
		}
		else
		{	// change trk name
			t->name = s;
			rt
		}
	}	rf
}


///  list update  ------------------------------------------------------------------------

void CList::listUpd(bool bUpdCur)
{
	int i=0;  char hid = 0;
	pTrk q = ll, qo = NULL;

	//save lcur,lofs, idPl
	bool upd = false,uc=0;	pTrk tkPl=0, tkCur=0; int lOd=0;
	float fCur=0.f, fL= listLen-1; //rel
	
	int si = vList.size();
	if (si > 0)
	{	upd = true;
		fCur = float(lCur)/fL;//rel
		if (idPl > si-1)  idPl = si-1;//
		tkPl = vList[idPl];  tkCur = vList[lCur];
		lOd = lOfs-lCur;
	}
	// move cur,idPl so they'd be in upd--
	// show/hide empty dirs, one file dirs opts...
	
	vList.clear();  listLen = 0;  destDirs();
	allDirs = 0;  allFiles = 0;  allTime = 0;  allSize = 0;
	
	while (q)
	{
		pTrk d = NULL;	//  diffrent path, opt-
		if (q==ll || q->path != qo->path)
		{
			// get last sub dir
			char p[MP];  scpy(p, q->path.c_str());  p[strlen(p)-1]=0;
			char* pe = strrchr(p,'\\');  if (!pe)  {  scpy(p,"no dir");  pe = p;  }
			d = new CTrk(pe+1, q->path);
			d->type = TY_DIR;

			//  subdir-1
			pe[0] = 0;
			char* p2 = strrchr(p,'\\');
			if (p2)  d->path2 = p2+1;

			vDirs.push_back(d);  //for delete
			vList.push_back(d);
			++listLen;  ++i;  /// Add Dir
			++allDirs;  hid = 0;
		}
		if (d)  d->next = q;  //h+
		
		if (q->hide > 0)  hid = q->hide;  //opt for hide/show dir
		if (hid==0 && (q->rate >= iRFilt && q->rate <= iRFilU) || hid==2)  // rating filtering
		{
			vList.push_back(q);

			if (upd)  {
				if (q==tkPl)  idPl = i;  //_ wont work on dirs
				if (q==tkCur && bUpdCur)
				{  lCur = i;  lOfs = lCur+lOd;  uc=1;  }  }

			++listLen;  ++i;  /// Add File
			if (bFilInf){	++allFiles;  allTime += q->time;  allSize += q->size;	}  // filtered
		}
		if (!bFilInf)	{	++allFiles;  allTime += q->time;  allSize += q->size;	}  // all

		qo = q;  q = q->next;
	}
	
	fL = float(listLen-1);
	if (!uc && upd && bUpdCur)  {  lCur = fCur*fL;  lOfs = lCur+lOd;  }
	zCur();  zOfs();  //range
	if (idPl > listLen-1)   idPl = listLen-1;
}
