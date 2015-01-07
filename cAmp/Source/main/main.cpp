#include "header.h"
#include "..\main\App.h"
using namespace std;


//  Entry  Win Main
//-------------------------------------------------------------------------------------------

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpCmdLine, int nCmdShow)
{
	//  Enable run-time memory check for debug builds
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	//  Init
	cAmp* pA = App::pAmp = new cAmp();
	if (pA == NULL)  {  MessageBoxA(0, "New cAmp !","main Init", MB_OK|MB_ICONSTOP);  return -3;  }
	
	//  app
	char s[300];
	GetModuleFileNameA(0,s,sizeof(s));
	char* pe = strrchr(s,'\\')+1;  pe[0]=0;  //ends with\ 
	scpy(pA->appPath, s);

	//  log
	sadd(s,"cAmp.log");
	ofstream of(s);
	if (!of)
	{
		string s = "Can't open log file: " + s;
		MessageBoxA(0, s.c_str(), "Init main", MB_OK|MB_ICONSTOP);
	}
	pA->flog = &of;

	pA->hInst = hInst;
	if (pA->Begin())
	{
		//  message loop
		bool run = true;
		while (run)
		{
			MSG msg;
			if (PeekMessage(&msg,0,0,0,PM_REMOVE))
			{
				if (msg.message==WM_QUIT)  run = false;
				TranslateMessage(&msg);  DispatchMessage(&msg);
			}else
				pA->Graph();
		}
	}	
	//  destroy
	pA->End();
	delete pA;
	return 0;
}


//  Processes messages
//-------------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_SYSKEYDOWN:
		App::pAmp->Keys(wp);
		return DefWindowProc(hWnd, msg, wp, lp);
	case WM_KEYDOWN:
		//if (wp != VK_ESCAPE)
		{  App::pAmp->Keys(wp);  break;  }

	case WM_DESTROY:
		PostQuitMessage(0);  rf

	case WM_MOUSEWHEEL:
		App::pAmp->Wheel(GET_WHEEL_DELTA_WPARAM(wp)/WHEEL_DELTA);  rf

	case WM_ACTIVATE:  App::pAmp->act = (LOWORD(wp) != WA_INACTIVE) && !(HIWORD(wp));  rf
	//	ShowCursor(0); rf
	
	case WM_DROPFILES:
		return  App::pAmp->OnDropFiles(wp,lp);

 	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}