#include "header.h"
#include "List.h"
#include "..\cD3\Osc.h"
#include "..\main\App.h"

											///\\\  List  \\\///

///------------------------------------------------  Load  ------------------------------------------------
const static char* slp = "Pls Load error";
bool CList::Load()
{
	//**/static Timer ti;  ti.update();

	ifstream fi;  char s[MP*2],ss[MP*2],n[MP*2],e[20];
	scpy(s,cOsc::appPath);  sadd(s,"playlists\\");  sadd(s,name);  sadd(s,".cp");
	fi.open(s,ios_base::in|ios_base::binary);
	if (fi.fail())  {  p(n)"Can't open file\n%s",s);  Wrng(n,slp)  }

	fi.getline(s,20);  if (strcmp(s,"cAmpPls")!=0)  Wrng("Not cAmpPls",slp)
	fi.getline(s,60);
	sscanf(s,"%d|%d|%d|%d|%d", &iRFilt, &lCur, &lOfs, &idPl, &iRFilU);
	
	pTrk prv = NULL, ll1 = NULL, qq = NULL;
	while (!fi.eof())
	{	// p,n,e
		fi.getline(s,MP,'|');  fi.getline(n,MP,'|');  fi.getline(e,MP,'|');
		if (strlen(s) > 0 && strlen(n) > 0 && strlen(e) > 0)
		{
			if (!qq || s[0] != '<')  scpy(ss, s);  // new path
			pTrk q = new CTrk(n,ss);
				
			if (ExtFind(e,iExtAud,ExtAud))  q->ext = exf;
			q->type = TY_AUDIO;
			// t,s
			fi.getline(s,MP,'|');  q->time = cOsc::toDouble(s);
			fi.getline(s,MP,'|');  q->size = cOsc::toInt6(s);

			// c,h,r,b
			fi.getline(s,80);	int c=0,h=0,r=0, b=0;
			sscanf(s,"%d|%d|%d|%d", &c, &h, &r, &b);
			q->tab=c;  q->hide=h;  q->rate=r;  q->bokm=b;
			
			//  list
			if (!ll1)  {  // new 1st
				ll1 = q;  prv = NULL;  }
			
			qq = q;  // prev
			q->prev = prv;
			if (prv)  q->prev->next = q;
			prv = q;
		}
	}	fi.close();
	
	if (ll1)
	{	insertList(/*InsM*/-2, getCur(), ll1,prv);  //ins
		listUpd(1/*0*/);  }

	//**/ti.update();   App::pAmp->time = ti.dt;
	rt
}

///------------------------------------------------  save  ------------------------------------------------
bool CList::Save()
{
	//**/static Timer ti;  ti.update();
	if (name[0]==0)  rt  // empty

	ofstream of;  char s[MP*2],n[MP*2];
	scpy(s,cOsc::appPath);  sadd(s,"playlists\\");  sadd(s,name);  sadd(s,".cp");
	of.open(s, ios_base::out|ios_base::binary|ios_base::trunc);
	if (of.fail())  {  p(n) "Can't open file\n%s",s);  Wrng(n,"Pls Save error")  }
	of << "cAmpPls\n";
	of << iRFilt <<'|'<< lCur <<'|'<< lOfs <<'|'<< idPl <<'|'<< iRFilU <<"\n";
	
	pTrk q = ll, qq = q; //prev
	while (q)
	{
		if (q != ll && strcmp(q->path, qq->path)==0)
			of << '<';  // same path
		else
			of << q->path;
		of <<'|'<< q->name <<'|'<< &ExtAud[q->ext][0] <<'|'<< q->time <<'|'<< q->size <<'|';
		of << (int)q->tab <<'|'<< (int)q->hide <<'|'<< (int)q->rate <<'|'<< (int)q->bokm;

		qq = q;  q = q->next;
		if (q)  of << "\n";
	}
	of.close();

	//**/ti.update();   App::pAmp->time = ti.dt;
	rt
}


//------------------------------------------------  clear
void CList::Clear()
{
	destroyThr();
	vList.clear();  listLen=0;  srchPath[0]=0;  //? numSel = 0;
	allDirs = allFiles = 0;	allSize = 0; allTime = 0;
	destList();
}
