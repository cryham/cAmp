#pragma once


enum EVis
{
	viNone=0, viFFT, viOsc, viPrint, viALL
};

const int MaxViews = 8;


class CViewSet
{
public:
	CViewSet() :
		xSize(360),ySize(900), xPos(0),yPos(0),
		iSleep(10), vsync(true), pr_fq(100.f),
		visH(96), fftSize(1), eVis(viFFT), fftMul(69.f),
		bSlDrawR(1), xWplS(14),
		cfP(1),cfA(0),cfT(1),cfH(1),cfG(1),
		xNpt(5),yNpt(2),ofsTab(0)
	{}

	//  window
	int xSize,ySize, xPos,yPos;
	int iSleep;

	//  vis
	int visH, fftSize;  float fftMul;
	/*EVis*/ int eVis;
	bool vsync;
	float pr_fq;

	//  slider
	bool bSlDrawR;  int xWplS;
	//  tabs
	int xNpt, yNpt, ofsTab;
	//  font sizes
	int cfP,cfA,cfT,cfH,cfG;  // pls,amp,tabs,hlp,gui

	// CList::bFilInf  CList::iDirView
};
