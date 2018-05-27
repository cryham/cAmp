#include "header.h"
#include "..\cD3\D3.h"
#include "..\main\resource.h"
#include "..\main\App.h"


//  mouse Move and size window
//--------------------------------------------------------------------------------------
void D3::MoveWnd()
{
	//  mouse pos  upd input
	int xx,yy;  POINT pt;
	xMold=xMpos; yMold=yMpos;  GetCursorPos(&pt);  xMpos=pt.x; yMpos=pt.y;
	//  mouse buttons
	bLs = bL;  bL = HIBYTE(GetKeyState(VK_LBUTTON)) != 0;
	bRs = bR;  bR = HIBYTE(GetKeyState(VK_RBUTTON)) != 0;
	bMs = bM;  bM = HIBYTE(GetKeyState(VK_MBUTTON)) != 0;
	//  key modifiers (if no hook)
	if (!hKbd)  {
		shift = isKeyDown(VK_SHIFT);
		ctrl = isKeyDown(VK_CONTROL);
		alt = isKeyDown(VK_MENU);  }

	if (!act || !shift || alt)  return;
	if (!bRs && bR) {  xWclick = xMpos-view.xPos;  yWclick = yMpos-view.yPos;  }

	//  move
	if (!ctrl && bR && (xMpos!=xMold || yMpos!=yMold))
	{
		view.xPos = xMpos-xWclick;  view.yPos = yMpos-yWclick;
		if (WndCheck())
			SetCursorPos(xMpos,yMpos);
		MoveWindow(hWnd, view.xPos,view.yPos, view.xSize,view.ySize, true);
	}

	//  size
	if (ctrl && bR && (xMpos!=xMold || yMpos!=yMold))
	{
		xx = xMpos-xMold;  yy = yMpos-yMold;
		view.xSize += xx;  view.ySize += yy;
		if (view.xSize < 150)  view.xSize = 150;
		if (view.ySize < 100)  view.ySize = 100;  // min size
		if (view.xPos+view.xSize > xScreen)  view.xSize = xScreen-view.xPos;
		if (view.yPos+view.ySize > yScreen)  view.ySize = yScreen-view.yPos;  // max=screen
		MoveWindow(hWnd, view.xPos,view.yPos, view.xSize,view.ySize, true);
		iSize++;
	}
	if (!bR && bRs && ctrl /*|| iSize>0 /*0,16*/)
	{	iSize = 0;
		App::pAmp->ReInit(true);
	}
}

bool D3::WndCheck()  // wnd in screen
{
	int j = 0;
	if (view.xPos < xScrMin) {  xMpos += xScrMin-view.xPos;  view.xPos = xScrMin;  ++j;  }
	if (view.yPos < yScrMin) {  yMpos += yScrMin-view.yPos;  view.yPos = yScrMin;  ++j;  }
	int xm = xScrMax-view.xSize;  if (view.xPos > xm) {  xMpos -= view.xPos-xm;  view.xPos = xm;  ++j;  }  // max
	int ym = yScrMax-view.ySize;  if (view.yPos > ym) {  yMpos -= view.yPos-ym;  view.yPos = ym;  ++j;  }
	return j > 0;
}


//  Create window
//--------------------------------------------------------------------------------------
bool D3::CreateWnd()
{
	WNDCLASS wc;  wc.style = 0;//CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = MsgProc;	wc.cbClsExtra = 0;  wc.cbWndExtra = 0;
	wc.hInstance = hInst;	wc.lpszMenuName = NULL;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = L"cAmp_WC";

	if (RegisterClass(&wc))
	{
		hWnd = CreateWindow(L"cAmp_WC", L"cAmp", WS_POPUP, view.xPos,view.yPos,view.xSize,view.ySize, 0,0,hInst,0);
		if (hWnd)
		{
			SetFocus(hWnd);  ShowWindow(hWnd,SW_SHOWNORMAL);  UpdateWindow(hWnd);
			return true;
		}
	}
	return false;
}


void D3::ScrSize()
{
	//int mon = GetSystemMetrics(SM_CMONITORS);
	xScrMin = GetSystemMetrics(SM_XVIRTUALSCREEN);  // pos min
	yScrMin = GetSystemMetrics(SM_YVIRTUALSCREEN);
	xScreen = GetSystemMetrics(SM_CXVIRTUALSCREEN);  // size
	yScreen = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	xScrMax = xScrMin + xScreen;  // pos max = pos min + size
	yScrMax = yScrMin + yScreen;
	// 0 0 2560 1024      [2][1]
	// -1280 0 2560 1024  [1][2] // -1280 -1024 2560 2048 6 '.
}
