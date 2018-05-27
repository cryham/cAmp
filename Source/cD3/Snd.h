#pragma once
#include "..\cD3\D3.h"
#include "..\main\str.h"


const static int
	sWAV = 4096,   // wav data, max screen width
	sFFT = 10240,  // fft data, max fft size
	PrClrs = 8192,  // print colors
	PrLin  = 24;    // max new print lines

const static int FFTNum = 5;
const static int fts[FFTNum] = {
	BASS_DATA_FFT512, BASS_DATA_FFT1024, BASS_DATA_FFT2048, BASS_DATA_FFT4096, BASS_DATA_FFT8192};


class cSnd : public D3, public cStr
{
public:
	cSnd();
	
	//  Main
	bool InitSnd();
	void DestSnd();
	void Vis(IDirect3DDevice9* pDev, float y1,float y2);

	bool bFps, bFInfo,bAllInfo;  // on/off
	int iTimeTest;  // test time colors
	
	//  spectrogram (voice print)
	void Vpr();
	int xpr, xpn;
	bool lock;  Timer TimPr;

	//  Osc/FFT ~
	int tmd,tmD,xt_;  // time add str
	float fft[sFFT],fftB[sFFT];
	short wav[sFFT];
 
	//  channels  ----
	HRECORD chRec;
	HSTREAM chPl;
	HMUSIC chMod;
	DWORD ch() {  return chMod ? chMod : chPl;  }
	HSYNC chSync;
	bool bPlay, bPaused, bRec,bRecSet;  // Play state, Rec

	//  sound  ----
	int nDev, nFreq, maxFreq;
	void bErr(const char *s);
	const char* GetErrStr(int err);
	std::vector<std::string> sDevs;
 
	//  wav data =
	float dt;  //, xL,xR, Ladd,Lsub;
	float visA[sWAV], visC[PrLin][sWAV];  //fft,wav clr

	DWORD pcl[PrClrs];
	void UpdPClr();
};
