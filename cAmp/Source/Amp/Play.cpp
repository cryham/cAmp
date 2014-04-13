#include "header.h"

#include "Amp.h"
#include "..\main\App.h"
#include "..\cD3\Graphics.h"


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

	//  get name
	char name[MP];
	tkPl->getFullName(name);
	//p(name) "%s%s.%s", tkPl->path, tkPl->name, ExtAud[tkPl->ext]);
	//N(name,"play")

	//  create stream
	BASS_StreamFree(chPl);  // free old	,stop
	chPl = BASS_StreamCreateFile(FALSE,name,0,0, (bRep1? BASS_SAMPLE_LOOP: 0) | BASS_STREAM_AUTOFREE);
	if (!chPl)
	switch (BASS_ErrorGetCode())
	{
		case BASS_ERROR_FILEOPEN:
		case BASS_ERROR_FILEFORM: // unsup format
		{	// cant open, not found
			tkPl->dis = 1;
			if (bNextPrev)	Next();  else  Prev();
		}	rf
		default:	// other
		{	p(s)"%s\n (error code: %d)",name,
			BASS_ErrorGetCode());
			Info(s,"Can't play file");
		}	rf
	}
	else  tkPl->dis = 0;

	// sync reaching end - for play next
	/*HSYNC hsy =*/ BASS_ChannelSetSync(
		chPl, BASS_SYNC_END/*or BASS_SYNC_FREE*/, 0, EndSync, this); 
		
	//  get file info
	int bitr=0;
	QWORD bytes = BASS_ChannelGetLength(chPl, BASS_POS_BYTE);
	if (bytes > 0)
	{	tmTot = BASS_ChannelBytes2Seconds(chPl, bytes);
		/*time upd again*/plsPl->allTime -= tkPl->time;
		tkPl->time = tmTot;  plsPl->allTime += tkPl->time;
		bitr = 0.008*double(tkPl->size)/tmTot;	}
	else  tmTot = 0.0;
	
	//  ext  kbps  freq  size
	BASS_CHANNELINFO info;	BASS_ChannelGetInfo(chPl, &info);
	p(sPlInf) "%4d %2d  %3.1f", bitr/*info.origres*/, info.freq/1000, double(tkPl->size-44)/1000000.0 );
	
	//  play
	BASS_ChannelSetAttribute(chPl, BASS_ATTRIB_VOL, fVol);  //,bal
	BASS_ChannelPlay(chPl, TRUE);
	bPlay = true;	bPaus = false;  rt
}


///  change pos,vol  - - - - 

void cAmp::chPos(double add)	//  <<  >>
{
	if (!bPlay)  return;
	double pos = BASS_ChannelBytes2Seconds(chPl, BASS_ChannelGetPosition(chPl, BASS_POS_BYTE));
	pos += add;
	if (pos < 0.0)  {  if (!bRep1) Prev();  pos += tmTot;  }  //out exact
	if (pos >tmTot) {  pos -= tmTot;  if (!bRep1) Next();  }
	BASS_ChannelSetPosition(chPl, BASS_ChannelSeconds2Bytes(chPl, pos), BASS_POS_BYTE);
}

void cAmp::chPosAbs(double pos)	//  <<| >>
{
	if (!bPlay)  return;
	if (pos > 0.999)  pos = 0.999;//
	BASS_ChannelSetPosition(chPl, BASS_ChannelSeconds2Bytes(chPl, pos*tmTot), BASS_POS_BYTE);
}


void cAmp::chVol(float add)  //  ^_
{
	fVol += add;
	fVol = mia(0.f,1.f, fVol);	tmd = tmD;
	if (bPlay)
		BASS_ChannelSetAttribute(chPl, BASS_ATTRIB_VOL, fVol);
	//BASS_ChannelSlideAttribute(channel, BASS_ATTRIB_VOL, 0, 1000);  
	//a negative volume value can be used to fade-out and then stop the channel.
}


///  repeat  - - - - 

void cAmp::repAll()  // @All
{
	bRepAll = !bRepAll;  tmd = tmD;
}
void cAmp::rep1()    // @1
{
	bRep1 = !bRep1;  tmd = tmD;
	if (bPlay)
		BASS_ChannelFlags(chPl, bRep1? BASS_SAMPLE_LOOP :0, BASS_SAMPLE_LOOP);
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
	if (!bPlay)  {  Play();  return;  }
	if (bPaus)
	{	BASS_ChannelPlay(chPl,FALSE);  bPaus = false;	}
	else
	{	BASS_ChannelPause(chPl);	bPaus = true;  }
}

void cAmp::Stop()	//  []
{
	if (bPlay)
	{	BASS_ChannelStop(chPl);  bPlay = false;  bPaus = false;  }
}


void cAmp::PlayPrevTab()
{
	tabPrev(0,0);	plsPlChg(plsId);	bDrawPlst = true;	Play(0);
}

void cAmp::PlayNextTab()
{
	tabNext(0,0);	plsPlChg(plsId);	bDrawPlst = true;	Play(0);
}
