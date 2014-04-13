#pragma once


class Timer
{
	private:
		double cc,fq, t,st, dt1,st1, iFR;
		LARGE_INTEGER CC;

	public:
		double dt, FR, iv, iv1;
		// delta time, FrameRate, interval, intervalFR

		Timer();
		bool update();
};


class CFont
{
public:
	CFont();
	bool Init( PDev dev );
	void Destroy();
	

	/* effect: */
	static LPD3DXBUFFER buf;
	static bool LoadFX( PDev dev, LPCSTR na, LPD3DXEFFECT& fx );
	

	//  CFont
	WORD  xb[256],yb[256];	// x,y begins
	BYTE  Fs[256];			// x sizes
	BYTE  z1, z2, iz;  // chars range, count
	int  Fx, Fxw, Fy, Fyw, xwr;  // sizes, last x
	
	// string
	static const int strSize = 300;
	char str[strSize+1];
	char Fname[280], Fext[6];  // font file name

	//  init
	static bool begin;
	static void Begin(int a), End();
	static void OnReset(), OnLost();

	//  Main str
	void Format(const char* format, ...);
	void StrCopy(const char* from);
	void StrAdd(const char* from);
	void StrWr(const char* from, int x, int y);

	void Write(int x, int y);
		int GetWidth(), dir, xmax;  //add
		bool bSl;  //single line

	//  font d3d
	static LPD3DXSPRITE Fspr;
	PTex Ftex,FtexW, pTexCur;
	D3DXCOLOR Fclr;
	void SetTex(bool norm);
};
