//  Precompiled Header
#pragma once
#define WINVER 0x0510
#define _WIN32_WINNT 0x0510
#define _WIN32_WINDOWS 0x0410
#define _WIN32_IE 0x0600

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
 #include <CommDlg.h>
 #include <shlobj.h>
 #include <ShellApi.h>

#include <vector>
#include <map>
#include <fstream>

#include <boost/filesystem.hpp>
#include "tinyxml.h"

#include "bass.h"
#include <d3dx9.h>
#if defined(DEBUG) | defined(_DEBUG)
	#include <crtdbg.h>
#endif


typedef unsigned int   uint;    typedef LPDIRECT3DTEXTURE9 PTex;
typedef unsigned char  byte;    typedef LPDIRECT3DDEVICE9  PDev;


#define rt  return true;
#define rf  return false;

#define scpy(a,b)	strcpy_s(a,sizeof(a)-1, b)
#define sadd(a,b)	strcat_s(a,sizeof(a)-1, b)

#define mia(a,b,x)  max(a, min(b, x))

#define REL(p)		if(p) {  p->Release();  p = NULL;  }


#if defined(DEBUG) || defined(_DEBUG)
//#include <dxerr.h>
//	#define Vd(x)    {  hr = (x);  if (FAILED(hr)) {	DXTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true );  }  }
	#define Vd(x)    {  hr = (x);  }
#else
	#define Vd(x)    {  hr = (x);  }
#endif


//  next higher power of 2, 32-bit
static uint NextPow2(uint n)
{
	uint v = n;
	--v;
	v |= v >> 1;  v |= v >> 2;  v |= v >> 4;  v |= v >> 8;  v |= v >> 16;
	++v;
	return v;
}
