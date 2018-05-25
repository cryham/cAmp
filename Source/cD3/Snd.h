#pragma once
#include "..\cD3\D3.h"
#include "..\main\str.h"


#define sWAV   4096  // wav data, max screen
#define sFFT  10240  // fft data

#define PrClrs  8192  // print colors
#define PrLin   24     // max new print lines

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
	HSYNC chSync;
	bool bPlay, bPaused, bRec,bRecSet;  // Play state, Rec

	//  sound  ----
	int nDev, nFreq, maxFreq;
	void bErr(const char *s);
	char* GetErrStr(int err);
	std::vector<std::string> sDevs;
 
    //  wav data =
	float dt;  //, xL,xR, Ladd,Lsub;
	float A[sWAV], C[PrLin][sWAV];  //fft,wav clr

	DWORD pcl[PrClrs];
	void UpdPClr();
};
