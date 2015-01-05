#pragma once
#include <string>
#include <vector>
#include "List_cdef.h"


class CTrk		///  track
{
public:

	CTrk *nx,*pv,*p,  // tree: down,back, right
		*next,*prev;  // list: next,prev

	char* name, *path, *path2;  //sub-1
	std::string name_pls;
	double time;  INT64 size;
	
	//  extra
	BYTE type, tab, ext;  //ext-ExtAud   tab--
	BYTE hide, sel;  char rate;  // rating
	BYTE dis;  //err disabled
	BYTE srch;	//match
	BYTE bokm;  //bookmark

	bool isDir() {  return  type < TY_FILE;  }
	
	void getFullName(char* str)  {
		StringCbPrintfA(str,MP-1, "%s%s.%s", path, name, ExtAudM[ext]);  }
	void updName();

	CTrk::~CTrk();
	CTrk::CTrk(const char* Name, const char* Path);
	void AddPath2(const char* Path2);
};

typedef CTrk* pTrk;



class CList		///  playlist
{
public:
	CList();  ~CList();
	std::string name;  //=filename

	//  adv oper
	bool RenameRate(pTrk t),
		Load(), Save();  void  Clear(),
		Hide(bool show), UnSel(), GotoPlay(), Center(),
		Bookm(int lev), BookmPrev(int lev, bool bSrch),BookmNext(int lev, bool bSrch),
		DecRatePl(),IncRatePl(),DecRate(),IncRate(),
		DecRFil(bool upr),IncRFil(bool upr);  int iRFilt,iRFilU;

	//  select
	void  Pick(int cr), Select1(int cr),SelRange(int cr, bool un), SelDir(int cr),
		sel(int ivL, bool sel), unSel0();
	
	//  operate
	void  Move1(int m, pTrk npos), MoveSel(int m, pTrk npos, CList* pL=NULL/*from*/),  // npos,cur = destin
		Del(bool disk=false), DelSel(bool disk=false), del(pTrk q,bool disk=false),  //from disk
		Insert1(int m, pTrk nt), ins(int m, pTrk cur, pTrk n/*what*/),
		insertList(int m, pTrk npos, pTrk first,pTrk last),
		CopySelFiles();
	
	//  tree, create
	void  treeCrt(), listCrt();  int listLen;  // length
	void  tree1File(pTrk* t, pTrk* q);  bool trGet,trRem;
	void  getNameRating(const char* name, char* pRate, BYTE* pBokm);
	pTrk  tree1Dir(const char* subPath),  tt,ww, ll;  /*1st(root): tt-tree, ww-back, ll-list*/
	pTrk  tree1Dir(pTrk* t, pTrk* q); //drop
	bool  Next(pTrk& cur), NextLev(pTrk& q);  int lev; // move
	//  list
	std::vector<pTrk>  vList, vDirs;  // list +dir,hid
	void  listUpd(bool bUpdCur=true),updTi(),UpdTimes();  // update
	void  destList(), destDirs();  // destroy
	pTrk  getCur();  //ins

	int  idPl, getTkPlIdx();  //  playing
	
	//  path
	WIN32_FIND_DATAA fd;
	char  sPath[MP],pp[MP], srchPath[MP],ss[MP];
	void  InsertDir(char* Path);/*+*/  int InsM;  // top,end,cur
	//  ext
	bool  ExtFind(const char* ex, int iExtCnt, const ExtTab extArr);
	int  exf;  // found

	//  therads
	HANDLE hs, thr,thrTi; // dir,timeupd
	void  destroyThr(),destThrTi();  bool bThr,bThrTi, bbThr;
	//void  WaitThr();
	
	//  list (keys)
	bool  PgUp(int),PgDn(int), PgOfsUp(int),PgOfsDn(int),  // (int mul)
		  Up(int),Dn(int), Home(int),End(int),
		  zCur(),zOfs();
	
	//  stats
	INT64  allSize, dirSize;  // sizes
	double  allTime;
	int  allDirs, allFiles;  // all cnt
	//  stats selected
	double selTime;  INT64 selSize;  //int selDirs;

	//  cur,sel
	int	 lCur, lOfs,  lInsPos, lInsM,  // list pos, ins
		ylastSel, numSel,  // select pos, count
		itu;  // tim upd indx
	int  iSrch;  // search hits
	
	static int  Lin;  // move,list lines
	static bool bFilInf;  // stats inf all/filt
	static int  iDirView;  // dirs(path) display
};
