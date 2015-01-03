#include "header.h"

#include "Amp.h"
#include "..\main\App.h"
#include "..\cD3\Graphics.h"


//* - -  -   -                                -  - -- gui init -- -  -                                 -  -  - - */
void cAmp::GuiCreate()
{
	GuiSys* g;  GuiInt* gi;  GuiText* tx;  GuiBut* bt;  GuiSld* sl;
	int y, ya, xs;
	#define  GuiPg(i)	g = &gui[i];  g->Clear();  \
		y = 0, ya = 24, xs = 120;  \
		gi = g->AddInt(150,0,60, "Page", &gpg, 0, GuiPages-1);  y+=ya;
	#define  CallId(c, id)	c->idC = id;  c->SetCallback(&cAmp::GuiEvCall, this);

GuiPg(0);
	tx = g->AddText(0,y,xs, "Player Settings");  y+=ya*3/2;  tx->tclr = D3DXCOLOR(1,1,1,1);

	gi = g->AddInt(0,y,xs, "Sound Device", &nDev, -1, sDevs.size()-1);  y+=ya;
	gi->imap[-1] = "Default";
	for (size_t i=0; i < sDevs.size(); i++)
		gi->imap[i] = sDevs[i];

	gi = g->AddInt(0,y,xs, "Frequency [kHz]", &nFreq, 0, 2, true);  //y+=ya;
	gi->imap[44100]="44.1";  //..get from snd
	gi->imap[48000]="48";
	gi->imap[96000]="96";
	/*gi->imap[192000]="192";*/	gi->done();
	bt = g->AddBut(240,y,60, ya, "Apply");  y+=ya;
	CallId(bt, 1);

	gi = g->AddInt(0,y,xs, "App priority", &iPriority, 0, 2);  y+=ya;
	gi->imap[0]="Normal";	gi->imap[1]="High";  gi->imap[2]="Realtime";

GuiPg(1);  // vis
	tx = g->AddText(0,y,xs, "Visualization");  y+=ya*3/2;  tx->tclr = D3DXCOLOR(1,1,1,1);
	g->AddInt(0,y,xs, "Sleep interval [ms]", &view.iSleep, -1, 200);  y+=ya*3/2;
	gi = g->AddInt(0,y,xs, "Visualization", &view.eVis, 0, viALL-1);  y+=ya;
	gi->imap[0]="Off";	gi->imap[1]="Frequency FFT";  gi->imap[2]="Wave Oscilloscope";  gi->imap[3]="Voice Print FFT";

	gi = g->AddInt(0,y,xs, "FFT vis size", &view.fftSize, 0, fftNum-1);  y+=ya;
	gi->imap[0]="512";	gi->imap[1]="1024";  gi->imap[2]="2048";  gi->imap[3]="4096";  gi->imap[4]="8192";
				  //_
	sl = g->AddSld(0,y,500, "Print move freq", &view.pr_fq, 10.f, 300.f, xs-10, 160, 15, "%6.1f");  y+=ya;
	CallId(sl, 2);
	g->AddSld(0,y,500, "FFT mult",		&view.fftMul, 40.f, 100.f, xs-10, 160, 15, "%6.1f");  y+=ya;
	
GuiPg(2);	//  pls
	tx = g->AddText(0,y,xs, "Playlist");  y+=ya*2/2;  tx->tclr = D3DXCOLOR(1,1,1,1);
	g->AddInt(0,y,xs, "Tabs X columns",	&view.xNpt, 1, 100);  y+=ya;
	g->AddInt(0,y,xs, "Tabs Y row",		&view.yNpt, 0, 100);  y+=ya;
	g->AddInt(0,y,xs, "Slider width",	&view.xWplS, 1, 100);  y+=ya*3/2;
	
	//  fonts
	tx = g->AddText(0,y,xs, "Font sizes");  y+=ya*2/2;  tx->tclr = D3DXCOLOR(1,1,1,1);
	g->AddInt(0,y,xs, "Playlist",&view.cfP, 0, NumFnt-1);  y+=ya;
	g->AddInt(0,y,xs, "Amp info",&view.cfA, 0, NumFnt-1);  y+=ya;
	g->AddInt(0,y,xs, "Tabs",	&view.cfT, 0, NumFnt-1);  y+=ya;
	g->AddInt(0,y,xs, "Help",	&view.cfH, 0, NumFnt-1);  y+=ya;
	g->AddInt(0,y,xs, "Gui",	&view.cfG, 0, NumFnt-1);  y+=ya;
}

///  gui events
// - -  -   -                                -  - -- 
void cAmp::GuiEvent(GuiCtrl* ctrl, int IDc, eGuiEvent evt)
{
	switch (IDc)
	{
	case 1:
		//if (evt == GE_BtnDnLn)
		break;
	
	case 2:
		//if (evt == GE_ValChg)
		GuiSld* sl = (GuiSld*)ctrl;
		break;
	}
};

//  reinit
void cAmp::GuiReInit()
{
	for (int i=0; i < GuiPages; ++i)
	gui[i].ReInit(10, yB_pt+cfont[view.cfG]->Fy, view.xSize,view.ySize,
		pDev, cfont[view.cfG], Tex, TX_TabC,TX_LeRi,TX_PosDk);
}

//  callback
void cAmp::GuiEvCall(void* pInst, GuiCtrl* ctrl, int IDc, eGuiEvent evt)
{
	cAmp* pA = (cAmp*)pInst;
	if (/*pA &&*/ IDc >= 0)
		pA->GuiEvent(ctrl, IDc, evt);
}
