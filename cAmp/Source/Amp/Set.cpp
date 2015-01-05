#include "header.h"

#include "Amp.h"
#include "..\main\App.h"
#include "..\cD3\Osc.h"
#include "..\cD3\Graphics.h"

											///\\\  Set  \\\///

//------------------------------------------------  Load  ------------------------------------------------
const static char* sle = "Set Load";
void cAmp::SetLoad()
{	//  default set
	bFps=0;  bFInfo=0;  CList::bFilInf=1;
	fVol=1.f;  bRepAll=1; bRep1=0;  cntrPls=0;  sInsPath[0]=0;
	bWasPlay=0;  lastPos=0.0;  plsId=0; plsPlId=0;

	iPriority=1;  nDev=-1;  nFreq=48000;  bHKeys=0;
	bFltTex = 1;  bRecSet = 1;

	vRclr.clear();  vTclr.clear();  rtx=D3DXCOLOR(0,1,0,1);  tmClrMode=0;
	
	scpy(s,appPath);  sadd(s,"cAmp.xml");
	TiXmlDocument xml;  xml.LoadFile(s);
	TiXmlElement* root = xml.RootElement(), *n = NULL, *m = NULL;
	if (!root) {	Info("No set file cAmp.xml",sle);  }
	else
	{	const char* a = NULL,*avk=NULL,*asc=NULL,*amod=NULL,*aon=NULL;

		n = root->FirstChildElement("Window");	if (!n)  Info("No <Window>",sle);
		if (n)  {
			a = n->Attribute("sizeX");	if (a)  view.xSize = mia(100, xScreen, toInt(a));
			a = n->Attribute("sizeY");	if (a)	view.ySize = mia(30,  yScreen, toInt(a));
			a = n->Attribute("posX");	if (a)  view.xPos = toInt(a);
			a = n->Attribute("posY");	if (a)  view.yPos = toInt(a);
			a = n->Attribute("visH");	if (a)  view.visH  = mia(0, yScreen, toInt(a));
			a = n->Attribute("fftSize");if (a)  view.fftSize = mia(0,fftNum-1, toInt(a));
			a = n->Attribute("slider");	if (a)  view.xWplS = max(0,toInt(a));
		}
		
		n = root->FirstChildElement("Fonts");	if (!n)  Info("No <Fonts>",sle);
		if (n)  {
			a = n->Attribute("pls");	if (a)  view.cfP = mia(0,NumFnt-1, toInt(a));
			a = n->Attribute("amp");	if (a)	view.cfA = mia(0,NumFnt-1, toInt(a));
			a = n->Attribute("tabs");	if (a)  view.cfT = mia(0,NumFnt-1, toInt(a));
			a = n->Attribute("help");	if (a)  view.cfH = mia(0,NumFnt-1, toInt(a));
			a = n->Attribute("gui");	if (a)  view.cfG = mia(0,NumFnt-1, toInt(a));
		}
		
		n = root->FirstChildElement("OnOff");	if (!n)  Info("No <OnOff>",sle);
		if (n)  {
			a = n->Attribute("bRepAll");	if (a)  bRepAll = toBool(a);
			a = n->Attribute("bRep1");		if (a)  bRep1 = toBool(a);
			a = n->Attribute("eVis");		if (a)  view.eVis = (EVis)mia(0,viALL-1, toInt(a));
			a = n->Attribute("bFilInfo");	if (a)  CList::bFilInf = toBool(a);
			a = n->Attribute("iDirView");	if (a)  CList::iDirView = toInt(a);
			a = n->Attribute("bSlDrawR");	if (a)  view.bSlDrawR = toInt(a);
		}

		n = root->FirstChildElement("App");		if (!n)  Info("No <App>",sle);
		if (n)  {
			a = n->Attribute("bRec");		if (a)  bRecSet = toBool(a);
			a = n->Attribute("vprFq");		if (a)  view.pr_fq = toInt(a);
			a = n->Attribute("fltTex");		if (a)  bFltTex = toBool(a);
			a = n->Attribute("sleep");		if (a)  view.iSleep = toInt(a);
			a = n->Attribute("priority");	if (a)  iPriority = toInt(a);
			a = n->Attribute("sndDev");		if (a)  nDev = mia(-1,100/*sDevs.size()-1*/, toInt(a));
			a = n->Attribute("sndFreq");	if (a)  nFreq = toInt(a);
			a = n->Attribute("insPath");	if (a)  scpy(sInsPath, a);
		}

		n = root->FirstChildElement("Last");	if (!n)  Info("No <Last>",sle);
		if (n)  {
			a = n->Attribute("volume");		if (a)  fVol = toFloat(a);
			a = n->Attribute("bPlay");		if (a)  bWasPlay = toBool(a);
			a = n->Attribute("pos");		if (a)  lastPos = toDouble(a);
			a = n->Attribute("playPls");	if (a)  plsPlId = toInt(a);
			a = n->Attribute("curPls");		if (a)  plsId = toInt(a);
			a = n->Attribute("ofsTab");		if (a)  view.ofsTab = toInt(a);
		}

		//  views
		n = root->FirstChildElement("Playlists");	if (!n)  Info("No <Playlists>",sle);
		if (n)
		{	a = n->Attribute("tabsX");		if (a)  view.xNpt = max(1,toInt(a));
			a = n->Attribute("tabsY");		if (a)  view.yNpt = max(0,toInt(a));
			a = n->Attribute("counter");	if (a)  cntrPls = toInt(a);
			
			m = n->FirstChildElement("Pls");  //if (!m)  N("No <Pls>",sle)
			while (m)
			{	a = m->Attribute("name");	vPlsNames.push_back(std::string(a));
				m = m->NextSiblingElement("Pls");
		}	}

		//  global keys
		n = root->FirstChildElement("GlobalKeys");	if (!n)  Info("No <GlobalKeys>",sle);
		if (n)
		{	a = n->Attribute("On");  if (a)  bHKeys = toBool(a);

			m = n->FirstChildElement("Key");  if (!m)  Info("No <Key>",sle);
			int i = 0;
			while (m)
			{
				avk = m->Attribute("vk");  asc = m->Attribute("sc");
				amod = m->Attribute("mod");  aon = m->Attribute("on");
				if (avk && asc && amod && aon && strlen(avk)==2 && strlen(asc)==2)
				{
					CKey& ck = vKeys[i];
					ck.vk = toByteHex(avk);  ck.sc = toByteHex(asc);
					ck.mod = toInt(amod);  ck.on = toBool(aon);
					++i;
				}
				m = m->NextSiblingElement("Key");
		}	}
		
		//  views
		n = root->FirstChildElement("Views");	if (!n)  Info("No <Views>",sle);
		if (n)  {	int i = 0;
			m = n->FirstChildElement("View");  if (!m)  Info("No <View>",sle);
			while (m && i < MaxViews)
			{
				CViewSet* v = &views[i];
				a = m->Attribute("sizeX");	if (a)  v->xSize = mia(100, xScreen, toInt(a));
				a = m->Attribute("sizeY");	if (a)	v->ySize = mia(30,  yScreen, toInt(a));
				a = m->Attribute("posX");	if (a)  v->xPos = toInt(a);
				a = m->Attribute("posY");	if (a)  v->yPos = toInt(a);

				a = m->Attribute("visH");	if (a)  v->visH = mia(0, yScreen, toInt(a));
				a = m->Attribute("eVis");	if (a)  v->eVis = (EVis)mia(0,viALL, toInt(a));
				a = m->Attribute("sleep");	if (a)  v->iSleep = toInt(a);
				a = m->Attribute("fft");	if (a)  v->fftSize = mia(0,fftNum-1, toInt(a));
				
				a = m->Attribute("sldr");	if (a)  v->xWplS = max(0,toInt(a));
				a = m->Attribute("sldr");	if (a)  v->bSlDrawR = max(0,toInt(a));

				a = m->Attribute("Fp");		if (a)  v->cfP = mia(0,NumFnt-1, toInt(a));
				a = m->Attribute("Fa");		if (a)  v->cfA = mia(0,NumFnt-1, toInt(a));
				a = m->Attribute("Ft");		if (a)  v->cfT = mia(0,NumFnt-1, toInt(a));
				a = m->Attribute("Fh");		if (a)  v->cfH = mia(0,NumFnt-1, toInt(a));
				a = m->Attribute("Fg");		if (a)  v->cfG = mia(0,NumFnt-1, toInt(a));

				a = m->Attribute("tbX");	if (a)  v->xNpt = max(1,toInt(a));
				a = m->Attribute("tbY");	if (a)  v->yNpt = max(0,toInt(a));
				a = m->Attribute("tbO");	if (a)  v->ofsTab = toInt(a);

				a = m->Attribute("vpFq");	if (a)  v->pr_fq = toInt(a);//toFloat
				//..
				m = m->NextSiblingElement("View");  ++i;	}
		}
		
	}
}


//------------------------------------------------  views

void cAmp::ViewLoad(int v)
{
	view = views[v];	ReInit(true);
}
void cAmp::ViewSave(int v)
{
	views[v] = view;	SetSave();
}


//------------------------------------------------  save  ------------------------------------------------
void cAmp::SetSave()
{
	TiXmlDocument xml;	TiXmlElement root("cAmp");

	TiXmlElement Wnd("Window");
		Wnd.SetAttribute("sizeX",	strI(view.xSize));
		Wnd.SetAttribute("sizeY",	strI(view.ySize));
		Wnd.SetAttribute("posX",	strI(view.xPos));
		Wnd.SetAttribute("posY",	strI(view.yPos));
		Wnd.SetAttribute("visH",	strI(view.visH));
		Wnd.SetAttribute("fftSize",	strI(view.fftSize));
		Wnd.SetAttribute("slider",	strI(view.xWplS));
	root.InsertEndChild(Wnd);
		
	TiXmlElement fnt("Fonts");
		fnt.SetAttribute("pls",		strI(view.cfP));
		fnt.SetAttribute("amp",		strI(view.cfA));
		fnt.SetAttribute("tabs",	strI(view.cfT));
		fnt.SetAttribute("help",	strI(view.cfH));
		fnt.SetAttribute("gui",		strI(view.cfG));
	root.InsertEndChild(fnt);
		
	TiXmlElement onf("OnOff");
		onf.SetAttribute("bRepAll",	strB(bRepAll));
		onf.SetAttribute("bRep1",	strB(bRep1));
		onf.SetAttribute("eVis",	strI(view.eVis));
		onf.SetAttribute("bFilInfo",strB(CList::bFilInf));
		onf.SetAttribute("iDirView",strI(CList::iDirView));
		onf.SetAttribute("bSlDrawR",strB(view.bSlDrawR));
	root.InsertEndChild(onf);

	TiXmlElement Plr("App");
		Plr.SetAttribute("bRec",	strB(bRecSet));
		Plr.SetAttribute("vprFq",	strD(view.pr_fq));
		Plr.SetAttribute("fltTex",	strB(bFltTex));
		Plr.SetAttribute("sleep",	strI(view.iSleep));
		Plr.SetAttribute("priority",strI(iPriority));
		Plr.SetAttribute("sndDev",  strI(nDev));
		Plr.SetAttribute("sndFreq", strI(nFreq));
		Plr.SetAttribute("insPath",	sInsPath);
	root.InsertEndChild(Plr);

	TiXmlElement Last("Last");
		Last.SetAttribute("volume",	strF(fVol));
		Last.SetAttribute("bPlay",	strB(bPlay));
		Last.SetAttribute("pos",	(bPlay && tmTot > 0.0) ? strD(tmPl/tmTot): "0.0");
		Last.SetAttribute("playPls",strI(plsPlId));
		Last.SetAttribute("curPls",	strI(plsId));
		Last.SetAttribute("ofsTab",	strI(view.ofsTab));
	root.InsertEndChild(Last);

	TiXmlElement Pls("Playlists");
	Pls.SetAttribute("tabsX",		strI(view.xNpt));
	Pls.SetAttribute("tabsY",		strI(view.yNpt));
	Pls.SetAttribute("counter",		strI(cntrPls));
	
	for (size_t i=0; i < vPlst.size(); i++)
	{
		TiXmlElement plst("Pls");
			plst.SetAttribute("name",	vPlst[i]->name.c_str());
			// idx,cur,ofs on each playlist here--
		Pls.InsertEndChild(plst);
	}
	root.InsertEndChild(Pls);

	TiXmlElement Keys("GlobalKeys");
		Keys.SetAttribute("On",	strB(bHKeys));
		
		for (int i = 0; i < FU_ALL; i++)
		{	TiXmlElement key("Key");
				key.SetAttribute("on",	strB(vKeys[i].on));
				key.SetAttribute("vk",	strHex(vKeys[i].vk));
				key.SetAttribute("sc",	strHex(vKeys[i].sc));
				key.SetAttribute("mod",	strI(vKeys[i].mod));
				//key.SetAttribute("fun",	cstPlrFuns[vKeys[i].fun]);
			Keys.InsertEndChild(key);
		}
	root.InsertEndChild(Keys);

	TiXmlElement Views("Views");
		//Views.SetAttribute("last",	strI(vcur));
		
		for (int i = 0; i < MaxViews; i++)
		{	TiXmlElement vw("View");
			CViewSet& v = views[i];
			vw.SetAttribute("sizeX",	strI(v.xSize));
			vw.SetAttribute("sizeY",	strI(v.ySize));
			vw.SetAttribute("posX",		strI(v.xPos));
			vw.SetAttribute("posY",		strI(v.yPos));

			vw.SetAttribute("visH",		strI(v.visH));
			vw.SetAttribute("eVis",		strI(v.eVis));
			vw.SetAttribute("sleep",	strI(v.iSleep));
			vw.SetAttribute("fft",		strI(v.fftSize));

			vw.SetAttribute("sldr",		strI(v.xWplS));
			vw.SetAttribute("slR",		strB(v.bSlDrawR));

			vw.SetAttribute("Fp",	strI(v.cfP));
			vw.SetAttribute("Fa",	strI(v.cfA));
			vw.SetAttribute("Ft",	strI(v.cfT));
			vw.SetAttribute("Fh",	strI(v.cfH));
			vw.SetAttribute("Fg",	strI(v.cfG));

			vw.SetAttribute("tbX",	strI(v.xNpt));
			vw.SetAttribute("tbY",	strI(v.yNpt));
			vw.SetAttribute("tbO",	strI(v.ofsTab));

			vw.SetAttribute("vpFq",	strI(v.pr_fq));
			//..
			Views.InsertEndChild(vw);
		}
	root.InsertEndChild(Views);

	scpy(s,appPath);  sadd(s,"cAmp.xml");
	xml.InsertEndChild(root);  xml.SaveFile(s);
}


											///  Set  Colors  ///

//------------------------------------------------  Load  ------------------------------------------------
const static char* sce = "Colors Load";
void cAmp::ClrLoad()
{
	vRclr.clear();  vTclr.clear();  rtx=D3DXCOLOR(0,1,0,1);  tmClrMode=0;

	scpy(s,appPath);  sadd(s,"Media\\colors.xml");
	TiXmlDocument xml;  xml.LoadFile(s);
	TiXmlElement* root = xml.RootElement(), *n = NULL, *m = NULL;
	if (!root) {	Info("No set file cAmp.xml",sce);  }
	else
	{	const char* a = NULL;//,*avk=NULL,*asc=NULL,*amod=NULL,*aon=NULL;

		n = root->FirstChildElement("RatingColors");	if (!n)  Info("No <RatingColors>",sce);
		{	m = NULL;
			if (n)  m = n->FirstChildElement("Rclr");  if (!m)  Info("No <Rclr>",sce);
			for (int i=0; i < chRall; ++i)
			{
				D3DXCOLOR c(0.2f,0.5f,0.8f,1.f);  if (i<cR0)  c=D3DXCOLOR(0.3f,0.6f,0.2f,1.f);
				if (m)	{
					a = m->Attribute("rgb");	if (a)	sscanf(a, "%f %f %f", &c.r, &c.g, &c.b);
					m = m->NextSiblingElement("Rclr");	}
				vRclr.push_back(c);
			}
		}
		
		n = root->FirstChildElement("RatingTex");	if (!n)  Info("No <RatingColors>",sce);
		if (n)  {
			a = n->Attribute("xmin");	if (a)	rtx.r = toFloat(a);
			a = n->Attribute("xmax");	if (a)	rtx.g = toFloat(a);
			a = n->Attribute("ymin");	if (a)	rtx.b = toFloat(a);
			a = n->Attribute("ymax");	if (a)	rtx.a = toFloat(a);
		}

		n = root->FirstChildElement("TimeColors");	if (!n)  Info("No <TimeColors>",sce);
		if (n)  {
			a = n->Attribute("mode");  if (a)  tmClrMode = toInt(a);

			m = n->FirstChildElement("Tclr");  if (!m)  Info("No <Tclr>",sce);
			while (m)	{
				D3DXCOLOR c(0.2f,0.6f,0.9f,/*tm*/1.f);
				a = m->Attribute("time");	if (a)	c.a = toFloat(a);
				a = m->Attribute("rgb");	if (a)	sscanf(a, "%f %f %f", &c.r, &c.g, &c.b);
				vTclr.push_back(c);
				m = m->NextSiblingElement("Tclr");	}
		}

	}
}

//------------------------------------------------  save  ------------------------------------------------
void cAmp::ClrSave()  // unused
{
	TiXmlDocument xml;	TiXmlElement root("cAmp");

	TiXmlElement Rclrs("RatingColors");
		for (size_t i = 0; i < vRclr.size(); i++)
		{	TiXmlElement rclr("Rclr");
				p(s) "%.3f %.3f %.3f", vRclr[i].r, vRclr[i].g, vRclr[i].b);
				rclr.SetAttribute("rgb", s);
			Rclrs.InsertEndChild(rclr);
		}
	root.InsertEndChild(Rclrs);

	TiXmlElement Rtx("RatingTex");
		Rtx.SetAttribute("xmin",	strF(rtx.r));
		Rtx.SetAttribute("xmax",	strF(rtx.g));
		Rtx.SetAttribute("ymin",	strF(rtx.b));
		Rtx.SetAttribute("ymax",	strF(rtx.a));
	root.InsertEndChild(Rtx);

	TiXmlElement Tclrs("TimeColors");
		Tclrs.SetAttribute("mode",	strI(tmClrMode));

		for (size_t i = 0; i < vTclr.size(); i++)
		{	TiXmlElement tclr("Tclr");
				tclr.SetAttribute("time", strF(vTclr[i].a));
				p(s) "%.3f %.3f %.3f", vTclr[i].r, vTclr[i].g, vTclr[i].b);
				tclr.SetAttribute("rgb", s);
			Tclrs.InsertEndChild(tclr);
		}
	root.InsertEndChild(Tclrs);

	scpy(s,appPath);  sadd(s,"Media\\colors.xml");
	xml.InsertEndChild(root);  xml.SaveFile(s);
}
