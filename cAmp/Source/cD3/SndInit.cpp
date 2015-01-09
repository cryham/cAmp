#include "header.h"
#include "Snd.h"
#include "..\main\App.h"
using namespace std;


BOOL CALLBACK  Recording(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return TRUE;  // continue
}

//                                           - -=  init Snd  =- -
bool cSnd::InitSnd()
{
	log("Init Snd ----");
	bRec = 0;
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{	Err("Incorrect bass.dll");  rf  }

	//  find all devs
	sDevs.clear();
	BASS_DEVICEINFO di;
	for (int n=0; BASS_RecordGetDeviceInfo(n,&di); n++)
	if (di.flags & BASS_DEVICE_ENABLED)
	{
		sDevs.push_back(di.name);
		log("Device " + iToStr(n) + ": " + di.name + (di.flags & BASS_DEVICE_DEFAULT ? " (default)": ""));
	}
	nDev = mia(-1, sDevs.size()-1, nDev);
	
	//  Init
	//?if (!BASS_SetConfig(BASS_CONFIG_REC_BUFFER, 1537)) {  bErr("Set config");  /**/}
	if (!BASS_Init(nDev,nFreq, 0,hWnd,NULL)) {  bErr("Can't initialize bass");  rf  }
	
	//  Load Plugins 
	string p = appPath + "bass*.dll";

	WIN32_FIND_DATAA fd;
	HANDLE fh = FindFirstFileA(p.c_str(), &fd);
	log("Loading plugins");
	if (fh != INVALID_HANDLE_VALUE)
	{	do
		{	HPLUGIN plug;
			if (strcmp(fd.cFileName,"bass.dll")!=0)
			if (plug = BASS_PluginLoad(fd.cFileName,0))
			{	//  plugin loaded
				const BASS_PLUGININFO *pinfo = BASS_PluginGetInfo(plug);
				for (int a=0; a < pinfo->formatc; a++)
					log(string("  ") + fd.cFileName + " " + pinfo->formats[a].name +" (" + pinfo->formats[a].exts + ")");
			}else
				log(string("plugin load error:") + fd.cFileName);
		} while (FindNextFileA(fh,&fd));
		FindClose(fh);
	}
	
	//  get freq info-
	BASS_INFO in;
	BOOL b = BASS_GetInfo(&in);
	if (b)
	{	log("speakers: "+iToStr(in.speakers));
		log("max freq: "+iToStr(in.maxrate));
		log("cur freq: "+iToStr(in.freq));
	}
	
	//  record init (for vis when not playing)
	if (bRecSet)
	{	bRec = 1;
		if (!BASS_RecordInit(nDev))
		{	bErr("Can't init record");  bRec=0;  }
		if (!(chRec = BASS_RecordStart(nFreq,2, 0, &Recording,this)))
		{	bErr("Can't start record");  bRec=0;  }
	}else
		bRec = 0;
	rt
}

void cSnd::DestSnd()
{
	if (bRecSet)
		BASS_RecordFree();
	BASS_Free();
}


//  err msg
//----------------------------------------------------------------------------------------
void cSnd::bErr(const char *se)
{
	int ec = BASS_ErrorGetCode();
	Err(string(se) + "\n code: " + iToStr(ec) + "\n " + GetErrStr(ec));
}

//  util-
char* cSnd::GetErrStr(int err)
{
	switch (err)  {
	case BASS_OK				: return "all is OK";
	case BASS_ERROR_MEM			: return "memory error";
	case BASS_ERROR_FILEOPEN	: return "can't open the file";
	case BASS_ERROR_DRIVER		: return "can't find a free/valid driver";
	case BASS_ERROR_BUFLOST		: return "the sample buffer was lost";
	case BASS_ERROR_HANDLE		: return "invalid handle";
	case BASS_ERROR_FORMAT		: return "unsupported sample format";
	case BASS_ERROR_POSITION	: return "invalid position";
	case BASS_ERROR_INIT		: return "BASS_Init has not been successfully called";
	case BASS_ERROR_START		: return "BASS_Start has not been successfully called";
	case BASS_ERROR_ALREADY		: return "already initialized/paused/whatever";
	case BASS_ERROR_NOCHAN		: return "can't get a free channel";
	case BASS_ERROR_ILLTYPE		: return "an illegal type was specified";
	case BASS_ERROR_ILLPARAM	: return "an illegal parameter was specified";
	case BASS_ERROR_NO3D		: return "no 3D support";
	case BASS_ERROR_NOEAX		: return "no EAX support";
	case BASS_ERROR_DEVICE		: return "illegal device number";
	case BASS_ERROR_NOPLAY		: return "not playing";
	case BASS_ERROR_FREQ		: return "illegal sample rate";
	case BASS_ERROR_NOTFILE		: return "the stream is not a file stream";
	case BASS_ERROR_NOHW		: return "no hardware voices available";
	case BASS_ERROR_EMPTY		: return "the MOD music has no sequence data";
	case BASS_ERROR_NONET		: return "no internet connection could be opened";
	case BASS_ERROR_CREATE		: return "couldn't create the file";
	case BASS_ERROR_NOFX		: return "effects are not available";
	case BASS_ERROR_NOTAVAIL	: return "requested data is not available";
	case BASS_ERROR_DECODE		: return "the channel is a decoding channel";
	case BASS_ERROR_DX			: return "a sufficient DirectX version is not installed";
	case BASS_ERROR_TIMEOUT		: return "connection timedout";
	case BASS_ERROR_FILEFORM	: return "unsupported file format";
	case BASS_ERROR_SPEAKER		: return "unavailable speaker";
	case BASS_ERROR_VERSION		: return "invalid BASS version (used by add-ons)";
	case BASS_ERROR_CODEC		: return "codec is not available/supported";
	case BASS_ERROR_ENDED		: return "the channel/file has ended";
	default:
	case BASS_ERROR_UNKNOWN		: return "unknown problem";
	}
}
