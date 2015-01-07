#include "header.h"
#include "Graphics.h"
#include "GuiSys.h"



//  Draw
//------------------------------------------
void GuiSys::DrawRect()
{
	if (!bInit)  return;
	for (size_t c=0; c < controls.size(); c++)
		if (controls[c]->vis)
			controls[c]->DrawRect();
}

void GuiSys::DrawText(float dt)
{
	if (!bInit)  return;
	for (size_t c=0; c < controls.size(); c++)
		//if (controls[c]->vis)
			controls[c]->DrawText(dt);
}

//  ctor
GuiSys::GuiSys() :
	xPos(0),yPos(0), xSize(800),ySize(600),
	tex(0), idTexBut(0), idTexLeRi(0), idTexSld(0),
	pDev(0), cfont(0), bInit(0)
{
	//controls.reserve(10);
}

GuiSys::~GuiSys()
{	Clear();  }

void GuiSys::Clear()
{
	for (size_t c=0; c < controls.size(); ++c)
		delete controls[c];
	controls.clear();
}

//  Init
//------------------------------------------
bool GuiSys::ReInit(int _xPos,int _yPos, int _xSize,int _ySize, 
		PDev _pDev, CFont* _cfont, 
		PTex _tex,
		int _idTexB, int _idTexLR, int _idTexSld)
{
	xPos=_xPos; yPos=_yPos;  xSize=_xSize; ySize=_ySize;
	pDev=_pDev; cfont=_cfont;
	tex=_tex;
	idTexBut=_idTexB;  idTexLeRi=_idTexLR;  idTexSld=_idTexSld;
	bInit = _pDev && _cfont;

	if (controls.size() > 0)  // upd all controls
	for (size_t c=0; c < controls.size(); ++c)
		controls[c]->ReSize();
	return bInit;
}


///  Mouse
///------------------------------------------------------------------------------------
bool GuiSys::MouseMove(int xm, int ym, bool lmb, bool lmbo, bool rmb)
{
	for (size_t i=0; i < controls.size(); ++i)
	{
		GuiCtrl* c = controls[i];
		int xc = c->xp + xPos, yc = c->yp + yPos;
		if (xm >= xc && xm <= xc + c->xs &&
			ym > yc && ym < yc + c->ys)
		{
			c->sel = 1.f;

			if (c->type == GC_But)
			{	GuiBut* b = (GuiBut*)c;
				if (lmb && !lmbo)  b->LDown();
				if (rmb)  b->RDown();
			}
			else if (c->type == GC_Sld)
			{	GuiSld* s = (GuiSld*)c;
				if (lmb)  //  set slider value
				{
					if (xm - xc >= s->xb - s->m)
					s->SetVal( s->vmin + (s->vmax - s->vmin) *
						mia(0.f,1.f, float(xm-xc - s->xb) / s->xbs ));
				}else
				if (rmb)
					s->rst();
			}
		}
	}	rf
}


//  add Text
//------------------------------------------
GuiText* GuiSys::AddText(int x, int y, int xs, const char* name)
{
	GuiText* c = new GuiText();  c->ps = this;
	c->xp = x;  c->yp = y;  c->xs = xs;
	scpy(c->name, name);  c->ReSize();

	controls.push_back((GuiCtrl*)c);
	return c;
}

//  add But
//------------------------------------------
GuiBut* GuiSys::AddBut(int x, int y, int xs, int ys, const char* name)
{
	GuiBut* c = new GuiBut();  c->ps = this;
	c->xp = x;  c->yp = y;  c->xs = xs;  c->ys = ys;
	scpy(c->name, name);  c->ReSize();

	controls.push_back((GuiCtrl*)c);
	return c;
}

//  add Int
//------------------------------------------
GuiInt* GuiSys::AddInt(int x, int y, int xs, const char* name,
	int* val, int vmin, int vmax, bool enu)
{
	GuiInt* c = new GuiInt();  c->ps = this;
	c->xp = x;  c->yp = y;  c->xs = xs;
	scpy(c->name, name);  c->ReSize();
	c->v = val;  c->vmin = vmin;  c->vmax = vmax;  c->enu = enu;

	//  up +
	int by = 4;
	c->bUp = new GuiBut();	GuiBut* b = c->bUp;  b->ps = this;
	b->xp = x + xs + 25;  b->yp = y + by;  b->xs = 18;  b->ys = 18;
	scpy(b->name, "");  b->ReSize();  b->vis = 0;
	controls.push_back((GuiCtrl*)b);
	if (enu)	b->SetLDown(&GuiInt::minc, c);  else
	{	b->SetLDown(&GuiInt::inc, c);	b->SetRDown(&GuiInt::end, c);	}

	//  dn -
	c->bDn = new GuiBut();	b = c->bDn;  b->ps = this;
	b->xp = x + xs;  b->yp = y + by;  b->xs = 18;  b->ys = 18;
	scpy(b->name, "");  b->ReSize();  b->vis = 0;
	controls.push_back((GuiCtrl*)b);
	if (enu)	b->SetLDown(&GuiInt::mdec, c);  else
	{	b->SetLDown(&GuiInt::dec, c);	b->SetRDown(&GuiInt::home, c);	}

	controls.push_back((GuiCtrl*)c);
	return c;
}

//  add Slider
//------------------------------------------
GuiSld* GuiSys::AddSld(int x, int y, int xs, const char* name,
	float* val, float vmin, float vmax,
	int valpos, int barpos, int marg, const char* _sfmt)
{
	GuiSld* c = new GuiSld();  c->ps = this;
	c->xp = x;  c->yp = y;  c->xs = xs;  c->xval = valpos;
	scpy(c->name, name);  c->ReSize();
	c->v = val;  c->vmin = vmin;  c->vmax = vmax;
	
	c->m = 15;  //marg for min max val
	c->xb = barpos + c->m;  c->xbs = xs - barpos - c->m*2;
	if (_sfmt)  scpy(c->sfmt, _sfmt);

	controls.push_back((GuiCtrl*)c);
	return c;
}
