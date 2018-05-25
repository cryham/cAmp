#pragma once
class TiXmlElement;  class D3;


enum EVis
{
	viNone=0, viFFT, viOsc, viPrint, viALL
};

const int MaxViews = 8;


class CViewSet
{
public:
	CViewSet();
	void Defaults();

	void LoadXml(const TiXmlElement* m, const D3* ap);
	void SaveXml(TiXmlElement* m);


	//  window
	int xSize,ySize, xPos,yPos;

	int iSleep;  // in ms
	bool vsync;

	//  vis
	int visH;  // draw height
	int fftSize;  // samples (fft quality)
	float fftMul;  // multiply y (scale)
	/*EVis*/ int eVis;
	float pr_fq;  // voice print/spectrogram freq (speed)

	//  slider
	bool bSlDrawR;  int xW_plS;
	//  tabs count
	int xNpt, yNpt, ofsTab;

	//  font sizes
	int cfP,cfA,cfT,cfH,cfG;  // pls,amp,tabs,hlp,gui

	// CList::bFilInf  CList::iDirView
};
