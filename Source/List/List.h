#pragma once
#include <string>
#include <vector>
#include "List_cdef.h"


class CTrk		///  track
{
public:

	CTrk *nx,*pv,*p,  // tree: down,back, right
		*next,*prev;  // list: next,prev

	std::string path, name, ext;
	std::string path2, name_pls;  // subdir-1, name shown in pls
	double time;  INT64 size;
	
	//  extra
	byte type, tab;  //tab--
	byte hide, sel;
	char rate;  // rating
	byte dis;  //err disabled
	byte srch;	//match
	char bokm;  //bookmark

	bool isDir() {  return  type < TY_FILE;  }
	
	std::string getFullPath(){  return path + name + "." + ext;  }
	void updName();

	CTrk::~CTrk();
	CTrk::CTrk(const std::string& Name, const std::string& Path);
};

typedef CTrk* pTrk;



class CList		///  playlist
{
public:
	CList();  ~CList();
	std::string name;  //=filename

	//  adv list
	bool RenameRate(pTrk t);
	bool Load(), Save();
	void Clear(), UnSel(), GotoPlay(bool center), Center();
	void Hide(bool show);  //.. save in dirs

	//  bookmarks
	char bokm;  //bookmark, backgr clr
	void BookmPls(char add);
	void Bookm(char add);  // trk
	void BookmPrev(int lev, bool bSrch), BookmNext(int lev, bool bSrch);

	//  rating
	void DecRatePl(),IncRatePl(), DecRate(),IncRate();
	//  filtering lower,upper
	int iRFilt,iRFilU;
	void DecRFil(bool upr),IncRFil(bool upr);

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
	int listLen;  // length
	void  treeCrt(), listCrt();

	void  tree1File(pTrk* t, pTrk* q, const char* fullpath=0);  bool trGet,trRem;
	void  getNameRating(const char* name, char* pRate, char* pBokm);
	pTrk  tree1Dir(const char* subPath),  tt,ww, ll;  /*1st(root): tt-tree, ww-back, ll-list*/
	pTrk  tree1Dir(pTrk* t, pTrk* q); //drop
	bool  Next(pTrk& cur), NextLev(pTrk& q);  int lev; // move
	//  list
	std::vector<pTrk>  vList, vDirs;  // list +dir,hid
	void  listUpd(bool bUpdCur=true),updTi(),UpdTimes();  // update
	void  destList(), destDirs();  // destroy
	pTrk  getCur();  //ins

	//  playing
	int  idPl, getTkPlIdx();
	
	//  path
	WIN32_FIND_DATAA fd;
	std::string srchPath, ss;
	std::string sPath, pp;
	int InsM;  // top,end,cur
	void  InsertDir(const char* Path);/*+*/

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
