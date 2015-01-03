#pragma once


enum eGuiCtrlType
{	GC_NONE=0,  GC_But, GC_Int, GC_Sld,  GC_ALL  };

enum eGuiEvent	// for user callback
{
	GE_BtnDnL=0,GE_BtnDnR=0,  // button down Lmb,Rmb
	GE_ValChg,GE_SldDnR,	// slider value changed, rmb down
	GE_ALL
};

typedef void (*FGuiEvent)(void* pInst, class GuiCtrl* ctrl, int IDc, eGuiEvent event);


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
	eGuiCtrlType type;
	D3DXCOLOR tclr;
	
	FGuiEvent callb;  void* inst;  // user callback
	void SetCallback(FGuiEvent callback, void* pInst);
	int idC;  // control's unique id

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

	//  callbacks internal
	FButDown Ldown,Rdown;	void* piL,*piR;
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
	std::map<int, const char*> imap;
	std::map<int, const char*>::iterator imi;

	void iCall();  // inc,dec
	static void dec(void* pInst), inc(void* pInst);
	static void home(void* pInst),end(void* pInst), rst(void* pInst);

	void imdec(),iminc();
	static void mdec(void* pInst),minc(void* pInst);
		
	// find val in map, do after imap adds
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
	void SetVal(float fv), rst();

	GuiSld();
	void DrawRect();
	void DrawText(float dt);
};
