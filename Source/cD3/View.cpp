#include "header.h"
#include "../main/str.h"
#include "View.h"
#include "D3.h"   // range check
#include "Snd.h"  // const


CViewSet::CViewSet()
{
	Defaults();
}

void CViewSet::Defaults()
{
	xSize = 360;  ySize = 900;  xPos = 0;  yPos = 0;
	iSleep = 10;  vsync = true;  pr_fq = 100.f;
	visH = 96;  fftSize = 1;  eVis = viFFT;  fftMul = 69.f;
	bSlDrawR = 1;  xW_plS = 14;
	xNpt = 5;  yNpt = 2;  ofsTab = 0;
	cfP = 1;  cfA = 0;  cfT = 1;  cfH = 1;  cfG = 1;
}


///  . . .             Load             . . .
void CViewSet::LoadXml(const TiXmlElement* m, const D3* ap)
{
	Defaults();
	const char* a;
	a = m->Attribute("sizeX");	if (a)  xSize = mia(100, ap->xScreen, cStr::toInt(a));
	a = m->Attribute("sizeY");	if (a)	ySize = mia(30,  ap->yScreen, cStr::toInt(a));
	a = m->Attribute("posX");	if (a)  xPos = cStr::toInt(a);
	a = m->Attribute("posY");	if (a)  yPos = cStr::toInt(a);

	a = m->Attribute("eVis");	if (a)  eVis = (EVis)mia(0,viALL, cStr::toInt(a));
	a = m->Attribute("visH");	if (a)  visH = mia(0, ap->yScreen, cStr::toInt(a));
	a = m->Attribute("ftMul");	if (a)  fftMul = cStr::toInt(a);//toFloat
	a = m->Attribute("fft");	if (a)  fftSize = mia(0,FFTNum-1, cStr::toInt(a));
	a = m->Attribute("vpFq");	if (a)  pr_fq = cStr::toInt(a);//toFloat

	a = m->Attribute("sleep");	if (a)  iSleep = cStr::toInt(a);
	a = m->Attribute("vsync");	if (a)  vsync = cStr::toBool(a);
	
	a = m->Attribute("sldr");	if (a)  xW_plS = max(0, cStr::toInt(a));
	a = m->Attribute("slR");	if (a)  bSlDrawR = cStr::toBool(a);

	int nf = NumFnt-1;
	a = m->Attribute("Fp");		if (a)  cfP = mia(0,nf, cStr::toInt(a));
	a = m->Attribute("Fa");		if (a)  cfA = mia(0,nf, cStr::toInt(a));
	a = m->Attribute("Ft");		if (a)  cfT = mia(0,nf, cStr::toInt(a));
	a = m->Attribute("Fh");		if (a)  cfH = mia(0,nf, cStr::toInt(a));
	a = m->Attribute("Fg");		if (a)  cfG = mia(0,nf, cStr::toInt(a));

	a = m->Attribute("tbX");	if (a)  xNpt = max(1, cStr::toInt(a));
	a = m->Attribute("tbY");	if (a)  yNpt = max(0, cStr::toInt(a));
	a = m->Attribute("tbO");	if (a)  ofsTab = cStr::toInt(a);
}

///  . . .             Save             . . .
void CViewSet::SaveXml(TiXmlElement* m)
{
	m->SetAttribute("sizeX",	cStr::iToStr(xSize,4));
	m->SetAttribute("sizeY",	cStr::iToStr(ySize,4));
	m->SetAttribute("posX",		cStr::iToStr(xPos,4));
	m->SetAttribute("posY",		cStr::iToStr(yPos,4));

	m->SetAttribute("eVis",		cStr::strI(eVis));
	m->SetAttribute("visH",		cStr::iToStr(visH,4));
	m->SetAttribute("ftMul",	cStr::strF(fftMul));
	m->SetAttribute("fft",		cStr::strI(fftSize));
	m->SetAttribute("vpFq",		cStr::strI(pr_fq));

	m->SetAttribute("sleep",	cStr::iToStr(iSleep,2));
	m->SetAttribute("vsync",	cStr::strB(vsync));

	m->SetAttribute("sldr",		cStr::iToStr(xW_plS,2));
	m->SetAttribute("slR",		cStr::strB(bSlDrawR));

	m->SetAttribute("Fp",	cStr::strI(cfP));
	m->SetAttribute("Fa",	cStr::strI(cfA));
	m->SetAttribute("Ft",	cStr::strI(cfT));
	m->SetAttribute("Fh",	cStr::strI(cfH));
	m->SetAttribute("Fg",	cStr::strI(cfG));

	m->SetAttribute("tbX",	cStr::iToStr(xNpt,2));
	m->SetAttribute("tbY",	cStr::strI(yNpt));
	m->SetAttribute("tbO",	cStr::strI(ofsTab));
}
