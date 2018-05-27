#include "header.h"
#include "CFont.h"
#include "..\main\App.h"
using namespace std;


#define Log(s)  App::pAmp->log(s)
#define Err(s)  App::pAmp->Err(s)

bool CFont::begin = false;
LPD3DXSPRITE CFont::Fspr = NULL;
LPD3DXBUFFER CFont::buf = NULL;


bool CFont::LoadFX( PDev dev, const string& path, LPD3DXEFFECT& fx )
{
	if (FAILED( D3DXCreateEffectFromFileA(dev, path.c_str(), 0, 0, 0, 0, &fx, &buf) ))
		if (buf)
		{
			char *p = strstr( (char*) buf->GetBufferPointer(), "error" );
			if (p)
				Log("!Load .fx " + path + "\n  " + p + "\n"+path);
			else
				Log(string("!Load .fx " + path + "\n  " + string((char*)buf->GetBufferPointer()) ) + "\n"+path);
			REL(buf)
			rf
		}
		else
			Log("!Load .fx error \n  " + path);
	rt
}


//--- --	--		--				init				--		--	-- ---

bool CFont::Init( PDev dev )
{
	string Fcf, Ftx;
	Fcf = Fname + ".cf";
	Ftx = Fname + Fext;
	
	//  read CFont data
	ifstream fi;	char s[96];
	fi.open(Fcf.c_str());
	if (!fi.good())
		rf

	fi.getline(s,90);  //CrystalFont 4.45
	fi.getline(s,90);		Fy = atoi(s);
	fi.getline(s,90,' ');	z1 = atoi(s);
	fi.getline(s,90);		z2 = atoi(s);  iz = z2-z1;
	for (int i=z1; i<=z2; i++)
	{
		fi.getline(s,90,' ');	xb[i] = atoi(s);
		fi.getline(s,90,' ');	yb[i] = atoi(s);
		fi.getline(s,90);		Fs[i] = atoi(s);
	}
	fi.close();

	Fx = Fs[32];  //spc

	//  get dim
	D3DXIMAGE_INFO ii;
	D3DXGetImageInfoFromFileA(Ftx.c_str(), &ii);
	Fxw = ii.Width;  Fyw = ii.Height;

	/*Font sprite & texture*/
	Fclr = D3DXCOLOR(1.f,1.f,1.f, 1.f);
	if (Fspr == NULL)
		if (FAILED( D3DXCreateSprite(dev, &Fspr) ))
			Err("!CFont: Can't create sprite");

	if (FAILED( D3DXCreateTextureFromFileExA(dev, Ftx.c_str(), Fxw, Fyw, 1, 
			D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
			/*alpha*/RGB( 255, 33, 255 ) , 0, 0, &Ftex) ))
		Err("!CFont: Can't load texture from: " + Ftx);
	pTexCur = Ftex;
	
	//  white texture copy  --------------
	if (FAILED( D3DXCreateTexture(dev, Fxw, /*Fyw*/Fxw, 1, 
			D3DPOOL_DEFAULT, /*D3DFMT_UNKNOWN*/D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &FtexW) ))
		Err("!CFont: Can't create white texture" );
		
	if (Ftex && FtexW)
	{	D3DLOCKED_RECT lr,lrW;
		if (FAILED( Ftex->LockRect(0, &lr, 0, D3DLOCK_READONLY) ))  Err("CFont copy can't lock");
		if (FAILED( FtexW->LockRect(0, &lrW, 0, D3DLOCK_DISCARD|D3DLOCK_NOOVERWRITE) ))  Err("font copy W can't lock");

		UINT* o = static_cast<UINT*>(lr.pBits);  //, yO = lr.Pitch>>2;
		UINT* oW= static_cast<UINT*>(lrW.pBits);

		UINT s = Fxw * Fyw;
		for (UINT a=0; a < s; ++a)
		{
			UINT rgb = o[a], r = rgb & 0xFF, g = (rgb >> 8) & 0xFF, b = (rgb >> 16) & 0xFF;
			UINT w = (r+g+b)*45/90;  if (w > 255)  w = 255;
			oW[a] = 0xFF000000+ w*0x010101;
		}

		if (FAILED( Ftex->UnlockRect(0) ))  Err("CFont copy can't unlock");
		if (FAILED( FtexW->UnlockRect(0) ))  Err("CFont copy W can't unlock");
		//if (strstr(Fname, "Tac16n"))  // test
		//	D3DXSaveTextureToFileA("white.png", D3DXIFF_PNG, FtexW, 0);
	}

	return true;
}


void CFont::OnLost()
{
	if (Fspr)
		Fspr->OnLostDevice();
}

void CFont::OnReset()
{
	if (Fspr)
		Fspr->OnResetDevice();
}


void CFont::Destroy()
{	
	REL(FtexW)
	REL(Ftex)
	REL(Fspr)
}



//  ----    font    ----

void CFont::Begin( int a )
{
	HRESULT hr;
	if (begin)	Vd( Fspr->End());
	Vd( Fspr->Begin( a != 0 ? D3DXSPRITE_ALPHABLEND : D3DXSPRITE_SORT_TEXTURE ));
	begin = true;
}

void CFont::End()
{
	if (begin)	Fspr->End();
	begin = false;
}


///  ---------  Format  ---------

void CFont::Format(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf_s(str, strSize, format, args);
	str[strSize-1] = 0;
	va_end(args);
}

void CFont::StrCopy(const char* from)
{
	strcpy_s(str, strSize, from);
}

void CFont::StrAdd(const char* from)
{
	strcat_s(str, strSize, from);
}

void CFont::StrWr(const char* from, int x, int y)
{
	StrCopy(from);  Write(x,y);
}


///  ---------  Write  ---------

void CFont::Write(int x, int y)
{
	if (!pTexCur || !Fspr)  return;

	int i = 0;	int c, w;
	D3DXVECTOR3 p = D3DXVECTOR3(x, y, 0);

	while ((c = BYTE(str[i++]) ) >= z1)
	{
		/*if (i>0 && s[i-1] == '`')  // cmd
		{
			//if (s[i] >= '0' && s[i] < '0'+Fnw)  Fn = s[i]-'0';  else  // clr chng
			if (s[i] == '+')  p.x += Fs[0]*2;  i++;		// spc
		}
		else/**/
		if (c <= z2)
		{
			w = Fs[c];

			if (dir < 0)  p.x -= w;

			if (p.x +w > xmax)  // dir > 0
				if (bSl)  return;  // next line
				else  {  p.x = x;  p.y += Fy;  }
			
			RECT r = { xb[c], yb[c], xb[c]+w, yb[c]+Fy };
			Fspr-> Draw( /*Ftex*/pTexCur, &r, 0, &p,  Fclr );
			
			if (dir > 0)  p.x += w;
		}
	}	xwr = p.x;
}

int CFont::GetWidth()
{
	int i = 0, w = 0, c;
	while ((c = BYTE(str[i++]) ) >= z1)
		if (c <= z2)
			w += Fs[c];
	return w;
}

void CFont::SetTex(bool norm)
{
	pTexCur = norm ? Ftex : FtexW;
}

CFont::CFont()
{
	dir = 1;  xmax = 3000;  bSl = true;
	Ftex = FtexW = pTexCur = NULL;  iz = 0;
}



//  ___ ___ ___ ___  Timer class  ___ ___ ___ ___

Timer::Timer()
{
	iv = 0.;	iv1 = 0.5;
	
	LARGE_INTEGER FQ;
	if (QueryPerformanceFrequency( &FQ ))
		fq = double( FQ.QuadPart );
}

bool Timer::update()
{
	QueryPerformanceCounter( &CC );
	cc = double( CC.QuadPart );
	t = cc / fq;
	
	dt = t - st;  // delta time
	if (dt < iv)  // interval
		return false;
	
	st = t;  // old time
	iFR++;  // frames count
	
	//  framerate update
	dt1 = t - st1;
	if (dt1 >= iv1)
	{
		FR = iFR / dt1;
		if (FR < 1.)  FR = 0.;
		iFR = 0.;
		st1 = t;
	}
	return true;
}
