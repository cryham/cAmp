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
#include <boost/filesystem.hpp>

#include <vector>
#include <map>
#include <fstream>
#include <tinyxml.h>

#include <bass.h>
#include <d3dx9.h>
#if defined(DEBUG) | defined(_DEBUG)
	#include <crtdbg.h>
#endif


typedef unsigned int   uint;    typedef LPDIRECT3DTEXTURE9 PTex;
typedef unsigned char  byte;    typedef LPDIRECT3DDEVICE9  PDev;

#define rt  return true;
#define rf  return false;

#define sfmt(a)		StringCbPrintfA(a,sizeof(a)-1,
#define scpy(a,b)	StringCbCopyA(a,sizeof(a)-1, b)
#define sadd(a,b)	StringCbCatA(a,sizeof(a)-1, b)

#define mia(a,b,x)  max(a, min(b, x))

#define REL(p)		if(p) {  p->Release();  p = NULL;  }


#if defined(DEBUG) || defined(_DEBUG)
#include <dxerr.h>
	#define Vd(x)    {  hr = (x);  if (FAILED(hr)) {	DXTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true );  }  }
#else
	#define Vd(x)    {  hr = (x);  }
#endif


static int __declspec(naked) __fastcall NextPow2(unsigned n)
{  _asm  {
	dec ecx
	mov eax, 2
	bsr ecx, ecx
	rol eax, cl
	ret
}  }
