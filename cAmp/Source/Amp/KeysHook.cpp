#include "header.h"

#include "Amp.h"
#include "..\main\App.h"


/*-   -   -   -   -   -   -   -   -   -   -   -   -   Hook Keys  -   -   -   -   -   -   -   -   -   -   -  -*/
bool cAmp::KeysHook(DWORD vk,DWORD sc,DWORD fl)  //BYTE
{
	bool dn = (fl & LLKHF_UP)>0 ? false: true;	//LLKHF_EXTENDED gray

	//  key modifiers
	shift = isKeyDown(VK_SHIFT);  //if (sc==0x38)  alt = dn;
	/*ctrl = isKeyDown(VK_CONTROL);*/  if (sc==0x1D)  ctrl = dn;
	alt = isKeyDown(VK_MENU);  //if (vk==0xA0 || vk==0xA1)  shift = dn;
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
				case FU_BACK:  chPos(-5);  rf	case FU_BACK1: chPos(-1);  rf	case FU_BACK2: chPos(-15);  rf
				case FU_FORW:  chPos( 5);  rf	case FU_FORW1: chPos( 1);  rf	case FU_FORW2: chPos( 15);  rf

				case FU_VOLDN:  chVol(-0.02f);  rf	case FU_VOLDN2: chVol(-0.05f);  rf	//case FU_BACK1: chVol(-0.005f);  rf
				case FU_VOLUP:  chVol( 0.02f);  rf	case FU_VOLUP2: chVol( 0.05f);  rf	//case FU_FORW1: chVol( 0.005f);  rf

				case FU_PREV:  Prev();  rf		case FU_NEXT:  Next();  rf
				case FU_PLAY:  Play(0);  rf		case FU_PAUSE: Pause();  rf		case FU_STOP:  Stop();   rf
				
				case FU_RATINC:	 if (plsPl) plsPl->IncRatePl();  rf
				case FU_RATDEC:  if (plsPl) plsPl->DecRatePl();  rf

				case FU_REPONE:  rep1();  rf

				case FU_PREVPLS:  PlayPrevTab();  rf
				case FU_NEXTPLS:  PlayNextTab();  rf
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
	if (!hKbd)  Info("Can't hook keyboard.","Init")
}
void cAmp::KbdDest()
{	UnhookWindowsHookEx(hKbd);  }
