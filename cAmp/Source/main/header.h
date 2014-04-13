//  Precompiled Header
#pragma once
#define WINVER 0x0510
#define _WIN32_WINNT 0x0510
#define _WIN32_WINDOWS 0x0410
#define _WIN32_IE 0x0600

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>
 #include <CommDlg.h>
 #include <shlobj.h>
 #include <ShellApi.h>

#include <vector>
#include <map>
#include <fstream>
#include <tinyxml.h>
using namespace std;

#include <bass.h>
#include <d3dx9.h>
#if defined(DEBUG) | defined(_DEBUG)
	#include <crtdbg.h>
#endif

#define rt  return true;
#define rf  return false;

#define p(a)		StringCbPrintfA(a,sizeof(a)-1,
#define scpy(a,b)	StringCbCopyA(a,sizeof(a)-1, b)
#define sadd(a,b)	StringCbCatA(a,sizeof(a)-1, b)

#define mia(a,b,x)  max(a, min(b, x))

#define REL(p)		if(p) {  p->Release();  p = NULL;  }
#define DEL(p)		if(p) {  delete p;   p = NULL;  }
#define DELA(p)		if(p) {  delete[]p;  p = NULL;  }

#define Wrng(b,c)					{  MessageBoxA(App::pAmp->hWnd, b,c, MB_OK|MB_ICONWARNING);  rf}
#define Info(b,c)					{  MessageBoxA(App::pAmp->hWnd, b,c, MB_OK|MB_ICONWARNING);  }

#if defined(DEBUG) || defined(_DEBUG)
#include <dxerr.h>
	#define Vd(x)    {  hr = (x);  if (FAILED(hr)) {	DXTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true );  }  }
#else
	#define Vd(x)    {  hr = (x);  }
#endif

#define  PTex  LPDIRECT3DTEXTURE9
#define  PDev  LPDIRECT3DDEVICE9

#pragma warning(disable:4305)  //double to float
#pragma warning(disable:4244)  //float to int
#pragma warning(disable:4018)  //signed/unsigned
#pragma warning(disable:4100)  //w4 unref param
#pragma warning(disable:4706)  //w4 assign in if
#pragma warning(disable:4505)  //unref function
#pragma warning(disable:4309)  //trunc const val
