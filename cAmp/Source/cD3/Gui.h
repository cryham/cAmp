#pragma once


enum GuiCtrlType
{	GC_NONE=0,  GC_But, GC_Int, GC_Sld,  GC_ALL  };


//  Control base
//-------------------------------------------
class GuiCtrl
{
public:
	int xp,yp, xs,ys;  // pos,size
	float x1,x2,y1,y2;  void ReSize();
	char name[30];   // display text
	bool vis;  // visible

	float sel;  DWORD rgb;  //clr over

	class GuiSys* ps;
	GuiCtrlType type;
	D3DXCOLOR tclr;

	virtual ~GuiCtrl();  GuiCtrl();
	virtual void DrawRect()=0;
	virtual void DrawText(float dt)=0;
};


//  Text
//-------------------------------------------
class GuiText : public GuiCtrl
{
public:
	GuiText();  ~GuiText();
	void DrawRect();
	void DrawText(float dt);
};


//  Button
//-------------------------------------------
typedef void (*FButDown)(void* pInst);
class GuiBut : public GuiCtrl
{
public:

	//  callbacks
	FButDown Ldown,Rdown;
	void* piL,*piR;
	void LDown(), RDown();
	void SetLDown(FButDown butdown, void* pInst);
	void SetRDown(FButDown butdown, void* pInst);
	
	GuiBut();  ~GuiBut();
	void DrawRect();
	void DrawText(float dt);
};


//  Int    | text  <dec inc>  value/str |
//-------------------------------------------
class GuiInt : public GuiCtrl
{
public:
	int* v,pdef;  // value,range
	int vmin,vdef,vmax;  bool enu;
	map<int, const char*> imap;
	map<int, const char*>::iterator imi;

	// dec,inc, home,end
	#define  giv  GuiInt* gi = (GuiInt*)pInst;  int* v = gi->v;
	static void dec(void* pInst) {	giv  if (*v > gi->vmin)  (*v)--;  }
	static void inc(void* pInst) {	giv  if (*v < gi->vmax)  (*v)++;  }
	static void home(void* pInst){	giv  *v = gi->vmin;  }
	static void end(void* pInst) {	giv  *v = gi->vmax;  }
	static void rst(void* pInst) {	giv  *v = gi->vdef;  }

	void mdec_(){			if (imi != imap.begin())  --imi;  (*v) = (*imi).first;  }
	void minc_(){	++imi;	if (imi == imap.end())    --imi;  (*v) = (*imi).first;  }
	static void mdec(void* pInst){	giv  gi->mdec_();  }
	static void minc(void* pInst){	giv  gi->minc_();  }
		
	// find *v in map, do after imap adds
	void done();

	GuiBut* bDn,*bUp;
	GuiInt();
	void DrawRect();
	void DrawText(float dt);
};


//  Slider  | text  value  [bar  ]  |
//-------------------------------------------
class GuiSld : public GuiCtrl
{
public:
	float* v,pdef;  // value,range
	float vmin,vdef,vmax;
	int xval, xb,xbs, m;  // val pos, bar pos,size, marg
	char sfmt[16];

	// set val
	#define  giv  GuiSld* gi = (GuiSld*)pInst;  float* v = gi->v;
	static void rst(void* pInst) {	giv  *v = gi->vdef;  }
	void SetVal(float fv) {  *v = fv;  /*callback*/  }

	GuiSld();
	void DrawRect();
	void DrawText(float dt);
};
