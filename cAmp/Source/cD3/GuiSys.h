#pragma once
#include "..\cD3\CFont.h"
#include "Gui.h"


//  Gui System  (holds controls)
//-------------------------------------------

class GuiSys
{
//private:
public:
	int xPos,yPos, xSize,ySize;
	PTex tex;
	int idTexBut, idTexLeRi, idTexSld;
	PDev pDev;
	CFont* cfont;
	bool bInit;  // can draw
	
	vector<GuiCtrl*> controls;

public:
	GuiSys();  ~GuiSys();
	bool ReInit(int xPos,int yPos, int xSize,int ySize, 
		PDev pDev, CFont* cfont, PTex tex,
		int idTexB, int idTexLR, int idTexSld);

	void Clear();

	bool MouseMove(int xm, int ym, bool lmb, bool lmbo, bool rmb);

	void DrawRect();
	void DrawText(float dt);

	GuiText* AddText(int x, int y, int xs, const char* name);
	GuiBut* AddBut(int x, int y, int xs, int ys, const char* name);
	GuiInt* AddInt(int x, int y, int xs,  const char* name,
		int* val, int vmin, int vmax,  bool enu = false);
	GuiSld* AddSld(int x, int y, int xs,  const char* name,
		float* val, float vmin, float vmax,
		int valpos, int barpos, int marg=15, const char* _sfmt=0);
	//..
};
