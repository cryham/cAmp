#include "header.h"
#include "Amp.h"
#include "..\main\App.h"
#include "..\cD3\Graphics.h"
using namespace std;


void CALLBACK EndSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	cAmp* pAmp = (cAmp*)user;
	if (!pAmp->bRep1)  pAmp->Next();
}

/*.   .   .   .   .   .   .   .   .   .   .   .   .  Player  .   .   .   .   .   .   .   .   .   .   .   .  .*/
///  Play

bool cAmp::Play(bool get,bool fget)	//  |>
{
	//  list
	if (get && bPlay || fget)
	{	if (!pls || !pls->ll)  rf
		if (pls->vList[pls->lCur]->isDir())  rf  // dont get dir
		plsPlChg(plsId);  //plsPl id
		plsPl->idPl = plsPl->lCur;  }  //trk id
	if (!plsPl || !plsPl->ll)  rf  //empty
	
	if (plsPl->idPl < 0 || plsPl->idPl >= plsPl->vList.size())  rf
	
	pTrk tkPl = plsPl->vList[plsPl->idPl];
	if (tkPl->isDir())  rf  //-
	//bDrawPlst = true;  // Z,X diffr--

	//. rem old
	if (chPl || chMod)
	{
		BASS_ChannelStop(chPl);
		if (chSync)
			BASS_ChannelRemoveSync(ch(), chSync);
		if (chPl)  BASS_StreamFree(chPl);
		if (chMod) BASS_MusicFree(chMod);
		chPl = 0;  chMod = 0;
	}

	//  get name
	string fname = tkPl->getFullPath();
	//if (tkPl->mod)
	if (cExt::Find(tkPl->ext) < 0)
	{   // load mod
		chMod = BASS_MusicLoad(FALSE, fname.c_str(), 0,0,
			BASS_MUSIC_STOPBACK | BASS_MUSIC_AUTOFREE | //?
			BASS_MUSIC_RAMPS | BASS_MUSIC_SINCINTER |  // quality todo opt
			BASS_MUSIC_FT2PAN | BASS_MUSIC_FT2MOD | //BASS_MUSIC_PT1MOD | //MODBASS_MUSIC_SURROUND | 
			(bRep1? BASS_SAMPLE_LOOP: 0) | BASS_MUSIC_PRESCAN, 1);
	}else
	{   // create stream
		chPl = BASS_StreamCreateFile(FALSE, fname.c_str(), 0,0,
			(bRep1? BASS_SAMPLE_LOOP: 0) | BASS_STREAM_AUTOFREE);
	}
	
	if (!chPl && !chMod)
	switch (BASS_ErrorGetCode())
	{
		case BASS_ERROR_FILEOPEN:
		case BASS_ERROR_FILEFORM:  // unsup format
		{	//  cant open, not found
			tkPl->dis = 1;
			if (bNextPrev)	Next();  else  Prev();
		}	rf
		default:  // other
		{	int er = BASS_ErrorGetCode();
			string s = "Can't play file: " + fname + "\n  error code: " + iToStr(er) + ": " + GetErrStr(er);
			log(s);
		}	rf
	}
	else  tkPl->dis = 0;

	//. sync reaching end - for play next
	chSync = BASS_ChannelSetSync(ch(), BASS_SYNC_END/*or BASS_SYNC_FREE*/, 0, EndSync, this); 
		
	//  get file info
	int bitr=0;
	QWORD bytes = BASS_ChannelGetLength(ch(), BASS_POS_BYTE);
	if (bytes > 0)
	{	tmTot = BASS_ChannelBytes2Seconds(ch(), bytes);
		// time upd again // todo when needed
		plsPl->allTime -= tkPl->time;  tkPl->time = tmTot;
		plsPl->allTime += tkPl->time;
		bitr = 0.008*double(tkPl->size)/tmTot;	}
	else  tmTot = 0.0;
	
	//  ext  kbps  freq  size
	BASS_CHANNELINFO info;	BASS_ChannelGetInfo(ch(), &info);
	sprintf_s(sPlInf, sizeof(sPlInf)-1,
		"%4d %2d  %3.1f", bitr/*info.origres*/, info.freq/1000, double(tkPl->size-44)/1000000.0 );
	
	//  play
	BASS_ChannelSetAttribute(ch(), BASS_ATTRIB_VOL, fVol);  //,bal
	BASS_ChannelPlay(ch(), TRUE);
	bPlay = true;	bPaused = false;  rt
}


///  change pos,vol  - - - - 

void cAmp::chPos(bool back, bool slow, bool fast)  //  <<  >>
{
	if (!bPlay)  return;
	double add = vSpdSeek[iSpdSeek].s[slow ? 0 : fast ? 2 : 1].add;
	double pos = BASS_ChannelBytes2Seconds(ch(), BASS_ChannelGetPosition(ch(), BASS_POS_BYTE));
	pos += back ? -add : add;
	if (pos < 0.0)  {  if (!bRep1) Prev();  pos += tmTot;  }  //out exact
	if (pos >tmTot) {  pos -= tmTot;  if (!bRep1) Next();  }
	BASS_ChannelSetPosition(ch(), BASS_ChannelSeconds2Bytes(ch(), pos), BASS_POS_BYTE);
}

void cAmp::chPosAbs(double pos)	//  <<| >>
{
	if (!bPlay)  return;
	if (pos > 0.999)  pos = 0.999;//
	BASS_ChannelSetPosition(ch(), BASS_ChannelSeconds2Bytes(ch(), pos*tmTot), BASS_POS_BYTE);
}


void cAmp::chVol(bool back, bool slow, bool fast)  //  ^ v
{
	float add = vSpdVol[iSpdVol].v[slow ? 0 : fast ? 2 : 1] * 0.01f;
	fVol += back ? -add : add;
	fVol = mia(0.f,1.f, fVol);	tmd = tmD;
	if (bPlay)
		BASS_ChannelSetAttribute(ch(), BASS_ATTRIB_VOL, fVol);
}


///  repeat  - - - - 

void cAmp::repAll()  // @A
{
	bRepAll = !bRepAll;  tmd = tmD;
}
void cAmp::rep1()    // @1
{
	bRep1 = !bRep1;  tmd = tmD;
	if (bPlay)
		BASS_ChannelFlags(ch(), bRep1? BASS_SAMPLE_LOOP :0, BASS_SAMPLE_LOOP);
}


///  controls  - - - - 

void cAmp::Prev()	//  |<
{
	EnterCriticalSection(&cs);
	bNextPrev = false;

	bool dn=false;  int subs=0, old = plsPl->idPl;
	while (!dn && subs < plsPl->listLen-1)
	{
		subs++;  plsPl->idPl--;
		if (plsPl->idPl < 0)
			if (bRepAll)  plsPl->idPl = plsPl->listLen-1;
			else  {  plsPl->idPl = old;
				LeaveCriticalSection(&cs);	return;  }
		if (!plsPl->vList[plsPl->idPl]->isDir() &&  // playable
			plsPl->vList[plsPl->idPl]->dis == 0)  dn=true;
	}
	LeaveCriticalSection(&cs);
	if (dn)  Play(false);  bDrawPlst2 = true;
}

void cAmp::Next()	//  >|
{
	EnterCriticalSection(&cs);
	bNextPrev = true;

	bool dn=false;  int adds=0, old = plsPl->idPl;
	while (!dn && adds < plsPl->listLen-1)
	{
		adds++;  plsPl->idPl++;
		if (plsPl->idPl > plsPl->listLen-1)
			if (bRepAll)  plsPl->idPl = 0;
			else  {  plsPl->idPl = old;
				LeaveCriticalSection(&cs);	return;  }
		if (!plsPl->vList[plsPl->idPl]->isDir() &&  // playable
			plsPl->vList[plsPl->idPl]->dis == 0)  dn=true;
	}
	LeaveCriticalSection(&cs);
	if (dn)  Play(false);  bDrawPlst2 = true;
}


void cAmp::Pause()	//  ||
{
	if (!bPlay)
	{	Play();  return;  }
	
	if (bPaused)
	{	BASS_ChannelPlay(ch(),FALSE);  bPaused = false;	}
	else
	{	BASS_ChannelPause(ch());	bPaused = true;  }
}

void cAmp::Stop()	//  []
{
	if (bPlay)
	{	BASS_ChannelStop(ch());  bPlay = false;  bPaused = false;  }
}


void cAmp::PlayPrevTab()
{
	tabPrev(0,0);	plsPlChg(plsId);	bDrawPlst = true;	Play(0);
}

void cAmp::PlayNextTab()
{
	tabNext(0,0);	plsPlChg(plsId);	bDrawPlst = true;	Play(0);
}
