#include "header.h"
#include "Graphics.h"
#include "Gui.h"
#include "GuiSys.h"



//	Control base
//--------------------------------------------------------------------------------------

GuiCtrl::GuiCtrl() :
	xp(0),yp(0), xs(20),ys(20), sel(0.f),rgb(0), ps(0),
	x1(0.f),y1(0.f), x2(0.1f),y2(0.1f), type(GC_NONE),
	vis(1), tclr(0.85f,0.85f,0.85f, 1.f),
	callb(0),idC(-1),inst(0)
{
	name[0]=0;
}
GuiCtrl::~GuiCtrl() {  }

void GuiCtrl::ReSize()
{
	if (!ps)  return;
	//float xfs = ps->xSize, yfs = ps->ySize;
	x1 = xp + ps->xPos;			y1 = yp + ps->yPos;
	x2 = xp + ps->xPos + xs;	y2 = yp + ps->yPos + ys;
}

void GuiCtrl::SetCallback(FGuiEvent callback, void* pInst)
{
	callb = callback;  inst = pInst;
}


//  Text
//--------------------------------------------------------------------------------------
GuiText::GuiText()  {}
GuiText::~GuiText() {}

void GuiText::DrawRect()
{
	//Rtex(ps->pDev, ps->pTexBut, x1,y1, x2,y2);
}
void GuiText::DrawText(float dt)
{
	ps->cfont->Fclr = tclr;
	ps->cfont->StrWr(name,
		xp + ps->xPos + 5,  //_par
		yp + ps->yPos + 4);
}


//	Button
//--------------------------------------------------------------------------------------

GuiBut::GuiBut() :
	Ldown(0),Rdown(0), piL(0),piR(0)
{	type = GC_But;  }
GuiBut::~GuiBut() {  }
	
void GuiBut::LDown(){	if (Ldown)	(*Ldown)(piL);	if (callb) (*callb)(inst,this,idC,GE_BtnDnL);	}
void GuiBut::RDown(){	if (Rdown)	(*Rdown)(piR);	if (callb) (*callb)(inst,this,idC,GE_BtnDnR);	}

void GuiBut::SetLDown(FButDown _butdown, void* _pInst){  Ldown = _butdown;	piL = _pInst;  }
void GuiBut::SetRDown(FButDown _butdown, void* _pInst){  Rdown = _butdown;	piR = _pInst;  }


void GuiBut::DrawRect()
{
	Rtex(ps->pDev, ps->tex, ps->idTexBut, x1,y1, x2,y2);
	if (sel > 0.f)
	RtxC(ps->pDev, ps->tex, ps->idTexBut, x1,y1, x2,y2, rgb);
}

void GuiBut::DrawText(float dt)
{
	ps->cfont->Fclr = tclr;
	ps->cfont->StrWr(name,
		xp + ps->xPos + 5,  //_par
		yp + ps->yPos + 4);

	if (sel > 0.f)
	{
		sel -= dt * 4.f;/**/
		rgb = RGB(sel*255.f, sel*255.f, sel*255.f);
	}
}


//	Int
//--------------------------------------------------------------------------------------

GuiInt::GuiInt() :
	pdef(0), vmin(0),vdef(1),vmax(3), bUp(0), bDn(0)
{  v = &vdef;  type = GC_Int;  }

void GuiInt::done()
{	imi = imap.begin();
	while (imi != imap.end())
	{
		if ((*imi).first == *v)  return;
		++imi;
	}	imi = imap.begin();/*not found*/
}

void GuiInt::DrawRect()
{
	DWORD c1 = enu || (*v < vmax) ? 0xFFFFFFFF : 0x60606060;
	DWORD c2 = enu || (*v > vmin) ? 0xFFFFFFFF : 0x60606060;
	RtxC(ps->pDev, ps->tex, ps->idTexLeRi, bUp->x1,bUp->y1, bUp->x2,bUp->y2, c1,		4.f*0.125f, 5.f*0.125f);  if (bUp->sel > 0.f)
	RtxC(ps->pDev, ps->tex, ps->idTexLeRi, bUp->x1,bUp->y1, bUp->x2,bUp->y2, bUp->rgb,	6.f*0.125f, 7.f*0.125f);
	RtxC(ps->pDev, ps->tex, ps->idTexLeRi, bDn->x1,bDn->y1, bDn->x2,bDn->y2, c2,		0.f*0.125f, 1.f*0.125f);  if (bDn->sel > 0.f)
	RtxC(ps->pDev, ps->tex, ps->idTexLeRi, bDn->x1,bDn->y1, bDn->x2,bDn->y2, bDn->rgb,	2.f*0.125f, 3.f*0.125f);
}

void GuiInt::DrawText(float dt)
{
	ps->cfont->Fclr = tclr;
	ps->cfont->StrWr(name,
		xp + ps->xPos + 5,  //_par
		yp + ps->yPos + 4);

	if (imap.empty())
		ps->cfont->Format("%d", *v);  // int
	else
		ps->cfont->Format("%s", imap[*v].c_str());  // enum

	ps->cfont->Write(
		xp + ps->xPos + 5 + xs + 60,
		yp + ps->yPos + 4);  //_par
}

//  dec,inc, home,end
#define  giv  GuiInt* gi = (GuiInt*)pInst;  int* v = gi->v;
#define  clb  gi->iCall();
void GuiInt::iCall()  {  if (callb)  (*callb)(inst, this, idC, GE_ValChg);  }

void GuiInt::dec(void* pInst) {	giv  if (*v > gi->vmin)  (*v)--;  clb  }
void GuiInt::inc(void* pInst) {	giv  if (*v < gi->vmax)  (*v)++;  clb  }
void GuiInt::home(void* pInst){	giv  *v = gi->vmin;  clb  }
void GuiInt::end(void* pInst) {	giv  *v = gi->vmax;  clb  }
void GuiInt::rst(void* pInst) {	giv  *v = gi->vdef;  clb  }

void GuiInt::imdec(){			if (imi != imap.begin())  --imi;  (*v) = (*imi).first;  }
void GuiInt::iminc(){	++imi;	if (imi == imap.end())    --imi;  (*v) = (*imi).first;  }
void GuiInt::mdec(void* pInst){	giv  gi->imdec();  clb  }
void GuiInt::minc(void* pInst){	giv  gi->iminc();  clb  }


//	Float slider
//--------------------------------------------------------------------------------------

GuiSld::GuiSld() :
	pdef(0), vmin(0),vdef(1.f),vmax(3.f)
{
	v = &vdef;  type = GC_Sld;  scpy(sfmt, "%6.3f");
}

void GuiSld::DrawRect()
{
	float f = (*v - vmin)/(vmax-vmin);
	float xv = x1+xb + xbs*f;           //_par
	Rtex(ps->pDev, ps->tex, ps->idTexSld, x1+xb,y1+4, x2-m,y2, 	0.f, 1.f);
	Rtex(ps->pDev, ps->tex, ps->idTexSld, x1+xb,y1+4, xv,y2, 	0.f, f);
}

void GuiSld::DrawText(float dt)
{
	ps->cfont->Fclr = tclr;
	ps->cfont->StrWr(name,
		xp + ps->xPos + 5,  //_par
		yp + ps->yPos + 4);

	ps->cfont->Format(sfmt, *v);  // sfmt
	ps->cfont->Write(xp + ps->xPos + 5 + xval,
					 yp + ps->yPos + 4);  //_par
}

void GuiSld::rst()			  {  *v = vdef;  if (callb)  (*callb)(inst, this, idC, GE_SldDnR/*_*/);  }
void GuiSld::SetVal(float fv) {  *v = fv;    if (callb)  (*callb)(inst, this, idC, GE_ValChg);  }
