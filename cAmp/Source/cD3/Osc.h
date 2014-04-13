#pragma once
#include "..\cD3\D3.h"


#define sWAV   4096  // wav data, max screen
#define sFFT  10240  // fft data

#define PrClrs  1024   // print colors
#define PrLin   24     // max new print lines

const static int fftNum = 5;
const static int fts[fftNum] = {
	BASS_DATA_FFT512, BASS_DATA_FFT1024, BASS_DATA_FFT2048, BASS_DATA_FFT4096, BASS_DATA_FFT8192};


class cOsc : public D3
{
public:
	cOsc();
	
  //  Main
	bool InitSnd();  void DestSnd();
	void Vis(IDirect3DDevice9* pDev, float y1,float y2);
	bool bFps, bFInfo,bAllInfo;  // on/off
	
	void Vpr();  int xpr,xpn;
	bool lock;  Timer TimPr;

  //  Osc ~
	int /*time add str*/tmd,tmD,xt_;
	float fft[sFFT],fftB[sFFT];  short wav[sFFT];
 
	//  channels  ----
	HRECORD chRec;
	HSTREAM chPl;
	bool bPlay, bPaus, bRec,bRecSet;  // Play state, Rec

	//  snd  ----
	int nDev, nFreq;
	void bErr(const char *s);
	char* GetErrStr(int err);
	vector<char*> sDevs;
 
  //  Lin =
	float  dt;  //, xL,xR, Ladd,Lsub;
	float  A[sWAV], C[PrLin][sWAV];  //fft,wav clr

	DWORD  pcl[PrClrs];  void UpdPClr();
	void hsv2rgb(float h, float s, float v, float *r,float *g, float *b);
 
	//  str
	void sSize(DWORD), sTime(DWORD, char*);	//old-
	void strTime(char* s, double ti, bool d=false); //d: 9.5s
		
	// str set
	#define chs  const char* str
	static char* strI(int i), *strF(float i), *strB(bool b);  // uses s
	static char* strI6(INT64 i), *strD(double d), *strHex(BYTE b);
	static float toFloat(chs);  static int toInt(chs);  static bool toBool(chs);
	static double toDouble(chs);  static INT64 toInt6(chs);  static BYTE toByteHex(chs);
};
