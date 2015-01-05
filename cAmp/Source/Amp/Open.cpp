#include "header.h"

#include "Amp.h"
#include "..\main\App.h"


//  Insert Dir dialog
///-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -

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

	// chg icon...  show _
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
			scpy(pls->sPath, sFile);	sadd(pls->sPath,"\\");  //end\ 
			pls->tt = pls->tree1Dir("");  pls->trGet = true;  pls->InsertDir(0);
		}else{  //  File   drop pls-..
			scpy(pls->pp, sFile);  char* pe = strrchr(pls->pp,'\\');  if (pe)  pe[1]=0;  //get path
			pls->tree1File(&tf, &qf);	if (tf)  {
				pls->ins(pls->InsM, pls->getCur(), tf);  //ins
				pls->listUpd(1);  pls->UpdTimes();  }
		}
	}else	//  ---------------- n ----------------
	if (pls->InsM == 2) // || pls->InsM == -1)
	{
		for (i = 0; i < nFiles; i++) {	dragGetName(i);  if (dir)	//  Dirs
			{	scpy(pls->sPath, sFile);	sadd(pls->sPath,"\\");
				pls->trGet = true;  pls->InsertDir(0);	//Sleep(100);
				while (pls->bbThr)  Sleep(10);  // <!>
			}	}
		for (i = 0; i < nFiles; i++) {	dragGetName(i);  if (!dir)	//  Files
			{	scpy(pls->pp, sFile);	char* pe = strrchr(pls->pp,'\\');  if (pe)  pe[1]=0;
				pls->tree1File(&tf, &qf);	files=true;
			}	}
		if (files) {  pls->tt = tf;  pls->trGet = false;  pls->InsertDir(0);  }
	}else{
		for (i = 0; i < nFiles; i++) {	dragGetName(i);  if (!dir)	//  Files
			{	scpy(pls->pp, sFile);	char* pe = strrchr(pls->pp,'\\');  if (pe)  pe[1]=0;
				pls->tree1File(&tf, &qf);	files=true;
			}	}
		if (files) {  pls->tt = tf;  pls->trGet = false;  pls->InsertDir(0);  }

		for (i = nFiles-1; i >= 0; i--) {	dragGetName(i);  if (dir)	//  Dirs
			{	scpy(pls->sPath, sFile);	sadd(pls->sPath,"\\");
				pls->trGet = true;  pls->InsertDir(0);	//Sleep(100);
				while (pls->bbThr)  Sleep(10);  // <!>
			}	}
	}
	DragFinish(hDrop);
	return 0;
}
