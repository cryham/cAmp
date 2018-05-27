#include "header.h"
#include "Amp.h"
#include "..\main\App.h"
using namespace std;


//  Insert Dir dialog
///-   -   -   -   -   -   -   -   -   -   -   -   -   -   -    -

static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{	case BFFM_INITIALIZED:
			if (lpData)  SendMessageA(hwnd, BFFM_SETSELECTIONA, TRUE, lpData);
	}
	return 0;
}

BOOL cAmp::BrowseDir(LPCSTR sCurrent, LPSTR sPath)
{
	BROWSEINFOA  bi = {0};  BOOL b;
	LPITEMIDLIST pidl;	CHAR sDisplay[MAX_PATH];

	CoInitialize(NULL);
	bi.hwndOwner = hWnd;
	bi.pszDisplayName = sDisplay;
	bi.lpszTitle = "Insert Directory";
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_USENEWUI|BIF_NONEWFOLDERBUTTON|BIF_VALIDATE;
	bi.lpfn    = BrowseCallbackProc;
	bi.lParam  = (LPARAM)sCurrent;

	pidl = SHBrowseForFolderA(&bi);
	if (pidl != NULL)
	{	b = SHGetPathFromIDListA(pidl, sPath);
		CoTaskMemFree(pidl);
	} else	b = FALSE;

	CoUninitialize();
	return b;
}


///  Drop files   -   -   -   -   -   -   -   -   -   -   -   -

LRESULT cAmp::OnDropFiles(WPARAM wParam, LPARAM lParam)
{
	if (!pls)  return 0;
	char sFile[MAX_PATH];
	HDROP hDrop = (HDROP)wParam;

	// change icon...  show _
	// none- after cur,  shift- top, ctrl- end
	if (!pls->ll) pls->InsM = 2; else
	if (shift)	pls->InsM = -2; else	if (ctrl)	pls->InsM = 2; else
	if (alt)	pls->InsM = -1; else	pls->InsM = (pls->lCur == 0) ? -1 :1;
	
	int nFiles = DragQueryFileA(hDrop, 0xFFFFFFFF, sFile, MAX_PATH);	
	pTrk tf=NULL,qf=NULL;  pls->ww = NULL;
	bool files=false, dir;  int i;

	#define dragGetName(i)	\
		DragQueryFileA(hDrop, i, sFile, MAX_PATH); \
		pls->hs = FindFirstFileA(sFile, &pls->fd); \
		if (pls->hs == INVALID_HANDLE_VALUE)  return 0; \
		FindClose(pls->hs); \
		dir = pls->fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

	if (nFiles==1)	// --------- 1 ---------
	{
		dragGetName(0);  if (dir) {  //  Dir
			pls->sPath = string(sFile) + "\\";  //end\ 
			pls->tt = pls->tree1Dir("");  pls->trGet = true;  pls->InsertDir(0);
		}else{  //  File
			pls->tree1File(&tf, &qf, sFile);	if (tf)  {
				pls->ins(pls->InsM, pls->getCur(), tf);  //ins
				pls->listUpd(1);  pls->UpdTimes();  }
		}
	}else	//  ---------------- n ----------------
	if (pls->InsM == 2) // || pls->InsM == -1)
	{
		for (i = 0; i < nFiles; i++) {	dragGetName(i);  if (dir)	//  Dirs
			{	pls->sPath = string(sFile) + "\\";
				pls->trGet = true;  pls->InsertDir(0);	//Sleep(100);
				while (pls->bbThr)  Sleep(10);  // <!>
			}	}
		for (i = 0; i < nFiles; i++) {	dragGetName(i);  if (!dir)	//  Files
			{
				pls->tree1File(&tf, &qf, sFile);	files=true;
			}	}
		if (files) {  pls->tt = tf;  pls->trGet = false;  pls->InsertDir(0);  }
	}else{
		for (i = 0; i < nFiles; i++) {	dragGetName(i);  if (!dir)	//  Files
			{
				pls->tree1File(&tf, &qf, sFile);	files=true;
			}	}
		if (files) {  pls->tt = tf;  pls->trGet = false;  pls->InsertDir(0);  }

		for (i = nFiles-1; i >= 0; i--) {	dragGetName(i);  if (dir)	//  Dirs
			{	pls->sPath = string(sFile) + "\\";
				pls->trGet = true;  pls->InsertDir(0);	//Sleep(100);
				while (pls->bbThr)  Sleep(10);  // <!>
			}	}
	}
	DragFinish(hDrop);
	return 0;
}


///  Copy files to other path  -   -   -   -   -   -   -   -   -
using namespace boost::filesystem;

void cAmp::CopyPls(bool b, CList* l)
{
	if (!l)  return;
	//note: dont do anything on pls during copy
	for (int i=0; i < l->vList.size(); ++i)
	{
		pTrk q = l->vList[i];
		if (!q->isDir())
		if (copyType > 0 || q->sel)  // 0 only selected
		{
			string pa = q->getFullPath();
			string to = pa.substr(2);  //par? rem drive d: etc
			to = copyPath + to;  // replace
			//  copy
			string tp,tf;  cExt::splitPath(to, tp, tf);

			if (!b)  // sum size
			{	if (!exists(to))
					copyMBAll += float(q->size)/1000000.f;
			}else
			try
			{	create_directories(tp);
				if (!exists(to))
				{
					copy_file(pa, to);  //copy_option::overwrite_if_exists
					//Sleep(10); //test
					copyMBCur += float(q->size)/1000000.f;
				}
			}catch (const filesystem_error & ex)
			{	if (b)
				log(string("copy fail: ") + ex.what());
			}
		}
		if (b && bCopyAbort)
		{	bCopyAbort = false;  q = 0;  }
	}
}

void cAmp::CopyAll()
{
	copyMBCur=0.f;  copyMBAll=0.f;
	bCopyAbort = false;
	
	for (int b=0; b<2; ++b)  // 0 sum size, 1 copy
	{
		if (copyType < 2)  // 0,1 cur playlist (visible)
			CopyPls(b>0, pls);
		else	// 2 all playlists
		for (size_t i=0; i < vPlst.size(); ++i)
			CopyPls(b>0, vPlst[i]);
	}
	bThrCopy = false;
}


DWORD WINAPI CopyThr(LPVOID lpParam)
{ 
	cAmp* a = (cAmp*)lpParam;
	a->CopyAll();
	return 0;
}

void cAmp::CopyThread()
{
	destCopyThr();
	thrCopy = CreateThread(NULL,0,CopyThr,(LPVOID)this,0,NULL);
	bThrCopy = true;
}

void cAmp::destCopyThr()
{
	if (bThrCopy)
	{	bThrCopy = false;
		TerminateThread(thrCopy, 1);  }
}
