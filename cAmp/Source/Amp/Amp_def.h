#pragma once
#include "../main/def.h"


//  application edit modes, focus
enum eEdit
{
	ED_Pls=0/*none*/, ED_Help, ED_nTab, ED_nFind, ED_Keys, ED_Set1, ED_ALL
};


//  Gui pages
const int GuiPages = 3;

//  Help pages
const int HelpPages = 6;
const static char HPnames[HelpPages][20]=
{
	"Player", "Mouse", "Playlist basic", "Playlist advanced", "Tabs, visual", "Player functions"
};


//  player functions
enum ePlrFuns
{
	FU_Play=0, FU_Pause, FU_Stop,
	FU_Prev, FU_Next,
	FU_PrevPls, FU_NextPls, 

	FU_Back, FU_Forw,  FU_Back1, FU_Forw1,  FU_Back2, FU_Forw2,
	FU_VolDn, FU_VolUp, FU_VolDn2, FU_VolUp2,
	FU_RatInc, FU_RatDec, FU_Rep1, FU_ALL
};

const char cswFuns[FU_ALL][24] =
{
	"Play  |>", "Pause ||", "Stop  []",
	"Prev  |<", "Next  >|",
	"Prev < playlist", "Next > playlist",

	"Backward <<", "Forward  >>",  "Bck  << small", "Frw >> small",  "Bck  << big", "Frw >> big",
	"Volume Down", "Volume Up ^",  "Vol Dn big", "Vol Up big", 
	"Inc + Rating", "Dec - Rating", "Repeat 1 One"
};

const int cyFadd[FU_ALL] = {0,0,2, 0,1, 0,2, 0,1,0,1,0,1, 0,1,0,2, 0,1,0};  // inc y for draw


//  global key data
class CKey
{
public:
	byte vk,sc, mod;
	bool on;  int vis;
	
	CKey() :
		vk(0),sc(0),mod(0), on(false),vis(0)
	{	}
};
