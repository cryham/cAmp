#include "header.h"
#include "Amp.h"
#include "..\main\App.h"


/*-   -   -   -   -   -   -   -   -   -   -   -   -   Hook Keys  -   -   -   -   -   -   -   -   -   -   -  -*/
bool cAmp::KeysHook(byte vk, byte sc, byte fl)  //byte
{
	bool dn = (fl & LLKHF_UP)>0 ? false: true;	//LLKHF_EXTENDED gray

	//  key modifiers
    if (vk == VK_LSHIFT)  shift = dn;  // R ..sep
    if (vk == VK_LCONTROL)  ctrl = dn;
    if (vk == VK_LMENU)  alt = dn;
	//shift = isKeyDown(VK_SHIFT);  //if (sc==0x38)  alt = dn;
	//*ctrl = isKeyDown(VK_CONTROL);*/  if (sc==0x1D)  ctrl = dn;
	//alt = isKeyDown(VK_MENU);  //if (vk==0xA0 || vk==0xA1)  shift = dn;
	mod = (shift ? 1:0)+(ctrl ? 2:0)+(alt ? 4:0);
	
	//  pick
	if (bPickingKey && dn)
	{	vKeys[yPickKey].vk = vk;  vKeys[yPickKey].sc = sc;  bPickingKey = false;  }

	if (!bHKeys)  rt	if (!dn)  rt

	//  keys
	for (int i=0; i < FU_ALL; ++i)
	{
		CKey* ck= &vKeys[i];
		if (ck->on && ck->mod == mod)
		if (ck->vk == vk && ck->sc == sc)
		{
			ck->vis = 100;
			switch (i)	// Player
			{
				case FU_Back:  chPos(1,0,0);  rf	case FU_Back1: chPos(1,1,0);  rf	case FU_Back2: chPos(1,0,1);  rf
				case FU_Forw:  chPos(0,0,0);  rf	case FU_Forw1: chPos(0,1,0);  rf	case FU_Forw2: chPos(0,0,1);  rf

				case FU_VolDn:  chVol(1,0,0);  rf	case FU_VolDn2: chVol(1,0,1);  rf
				case FU_VolUp:  chVol(0,0,0);  rf	case FU_VolUp2: chVol(0,0,1);  rf

				case FU_Prev:  Prev();  rf		case FU_Next:  Next();  rf
				case FU_Play:  Play(0);  rf		case FU_Pause: Pause();  rf		case FU_Stop:  Stop();   rf
				
				case FU_RatInc:	 if (plsPl) plsPl->IncRatePl();  rf
				case FU_RatDec:  if (plsPl) plsPl->DecRatePl();  rf

				case FU_Rep1:  rep1();  rf

				case FU_PrevPls:  PlayPrevTab();  rf
				case FU_NextPls:  PlayNextTab();  rf
			}
			rt
		}
	}	rt
}


///
LRESULT CALLBACK kpH(int code, WPARAM wp, LPARAM lp)
{
	KBDLLHOOKSTRUCT h = *((KBDLLHOOKSTRUCT*) lp);

	if (App::pAmp->KeysHook( h.vkCode, h.scanCode, h.flags ))
		return CallNextHookEx(App::pAmp->hKbd, code, wp, lp);
	else  return -1;  //no--
}

void cAmp::KbdInit()
{
	hKbd = SetWindowsHookEx(WH_KEYBOARD_LL, kpH, GetModuleHandle(0), 0);
	if (!hKbd)  log("Can't hook keyboard!");
}
void cAmp::KbdDest()
{
	UnhookWindowsHookEx(hKbd);
}
