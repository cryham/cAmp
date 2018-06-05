#include "header.h"
#include "Amp.h"
#include "..\main\App.h"
#include "..\cD3\Snd.h"
#include "..\cD3\Graphics.h"
using namespace std;

											///\\\  Set  \\\///

//------------------------------------------------  Load  ------------------------------------------------
const static string sle = "Load cAmp.xml: ";
void cAmp::SetLoad()
{
	//  defaults
	SetDefault();

	string p = appPath + "cAmp.xml";
	TiXmlDocument xml;  xml.LoadFile(p.c_str());
	TiXmlElement* root = xml.RootElement(), *n = NULL, *m = NULL;
	if (!root) {	log(sle+"No set file cAmp.xml");  }
	else
	{	const char* a = NULL;

		n = root->FirstChildElement("OnOff");	if (!n)  log(sle+"No <OnOff>");
		if (n)  {
			a = n->Attribute("bRepAll");	if (a)  bRepAll = toBool(a);
			a = n->Attribute("bRep1");		if (a)  bRep1 = toBool(a);

			a = n->Attribute("bFilInfo");	if (a)  CList::bFilInf = toBool(a);
			a = n->Attribute("iDirView");	if (a)  CList::iDirView = toInt(a);
		}
		
		n = root->FirstChildElement("App");		if (!n)  log(sle+"No <App>");
		if (n)  {
			a = n->Attribute("bRec");		if (a)  bRecSet = toBool(a);
			a = n->Attribute("fltTex");		if (a)  bFltTex = toBool(a);
			a = n->Attribute("priority");	if (a)  iPriority = toInt(a);
			a = n->Attribute("sndDev");		if (a)  nDev = mia(-1,100/*sDevs.size()-1*/, toInt(a));
			a = n->Attribute("sndFreq");	if (a)  nFreq = toInt(a);
		}

		n = root->FirstChildElement("Path");	//if (!n)  log("No <Path>",sle);
		if (n)  {
			a = n->Attribute("ins");	if (a)  sInsPath = string(a);
			a = n->Attribute("copy");	if (a)  copyPath = string(a);
		}

		n = root->FirstChildElement("Last");	if (!n)  log(sle+"No <Last>");
		if (n)  {
			a = n->Attribute("volume");		if (a)  fVol = toFloat(a);
			a = n->Attribute("bPlay");		if (a)  bWasPlay = toBool(a);
			a = n->Attribute("pos");		if (a)  lastPos = toDouble(a);
			a = n->Attribute("playPls");	if (a)  plsPlId = toInt(a);
			a = n->Attribute("curPls");		if (a)  plsId = toInt(a);
		}

		//  views
		n = root->FirstChildElement("Views");	if (!n)  log(sle+"No <Views>");
		if (n)
		{	int i = 0;
			m = n->FirstChildElement("View");  if (!m)  log(sle+"No <View>");
			while (m && i < MaxViews)
			{
				CViewSet* v = &views[i];
				v->LoadXml(m, this);
				m = m->NextSiblingElement("View");  ++i;
			}
			//  current
			m = n->FirstChildElement("Vcur");  if (!m)  log(sle+"No <Vcur>");
			if (m)
				view.LoadXml(m, this);
		}		

		//  seek
		n = root->FirstChildElement("Seek");	if (!n)  log(sle+"No <Seek>");
		if (n)
		{	m = n->FirstChildElement("Pos");  if (!m)  log(sle+"No <Pos>");
			while (m)
			{
				SpdSeek3 s;
				a = m->Attribute("slow");	if (a)  s.s[0].add = toInt(a);
				a = m->Attribute("norm");	if (a)  s.s[1].add = toInt(a);
				a = m->Attribute("fast");	if (a)  s.s[2].add = toInt(a);

				vSpdSeek.push_back(s);
				m = m->NextSiblingElement("Pos");
			}
			m = n->FirstChildElement("Vol");  if (!m)  log(sle+"No <Vol>");
			while (m)
			{
				SpdVol3 v;
				a = m->Attribute("slow");	if (a)  v.v[0] = toFloat(a);
				a = m->Attribute("norm");	if (a)  v.v[1] = toFloat(a);
				a = m->Attribute("fast");	if (a)  v.v[2] = toFloat(a);

				vSpdVol.push_back(v);
				m = m->NextSiblingElement("Vol");
			}
		}
		//  empty
		if (vSpdSeek.empty())
		{	SpdSeek3 s;  s.s[0].add = 1;  s.s[1].add = 5;  s.s[2].add = 15;
			vSpdSeek.push_back(s);
		}
		if (vSpdVol.empty())
		{	SpdVol3 v;  v.v[0] = 1.f;  v.v[1] = 2.f;  v.v[2] = 5.f;
			vSpdVol.push_back(v);
		}
		aSpdSeek = vSpdSeek.size();
		aSpdVol = vSpdVol.size();

		if (n)
		{	a = n->Attribute("pos");	if (a)  iSpdSeek = mia(0,aSpdSeek-1, toInt(a));
			a = n->Attribute("vol");	if (a)  iSpdVol  = mia(0,aSpdVol -1, toInt(a));
		}


		//  playlists
		n = root->FirstChildElement("Playlists");	if (!n)  log(sle+"No <Playlists>");
		if (n)
		{	a = n->Attribute("counter");	if (a)  cntrPls = toInt(a);
			
			m = n->FirstChildElement("Pls");  if (!m)  log(sle+"No <Pls>");
			while (m)
			{
				SetPls sp;
				a = m->Attribute("name");	sp.name = a;
				a = m->Attribute("bokm");	if (a)  sp.bokm = toInt(a);

				vSetPls.push_back(sp);
				m = m->NextSiblingElement("Pls");
		}	}

		//  global keys
		n = root->FirstChildElement("GlobalKeys");	if (!n)  log(sle+"No <GlobalKeys>");
		if (n)
		{	a = n->Attribute("On");  if (a)  bHKeys = toBool(a);

			m = n->FirstChildElement("Key");  if (!m)  log(sle+"No <Key>");

			const char *avk=NULL,*asc=NULL,*amod=NULL,*aon=NULL;
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
		
		
		//  ext list,  optional, in ver2  . . . .
		sExtAll = cExt::sExt1;
		sModAll = cExt::sMod1;
			
		n = root->FirstChildElement("Ext");
		if (n)
		{	a = n->Attribute("all");
			if (a)  sExtAll = string(a);
	
			a = n->Attribute("mod");
			if (a)  sModAll = string(a);
		}
	}

	//  init ext  . . . . . . . . . . . 
	cExt::Init(sExtAll, sModAll);
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
	size_t i;

	TiXmlElement onf("OnOff");
		onf.SetAttribute("bRepAll",	strB(bRepAll));
		onf.SetAttribute("bRep1",	strB(bRep1));
		onf.SetAttribute("eVis",	strI(view.eVis));
		onf.SetAttribute("bFilInfo",strB(CList::bFilInf));
		onf.SetAttribute("iDirView",strI(CList::iDirView));
	root.InsertEndChild(onf);
		
	TiXmlElement ap("App");
		ap.SetAttribute("bRec",	strB(bRecSet));
		ap.SetAttribute("fltTex",	strB(bFltTex));
		ap.SetAttribute("priority",strI(iPriority));
		ap.SetAttribute("sndDev",  strI(nDev));
		ap.SetAttribute("sndFreq", strI(nFreq));
	root.InsertEndChild(ap);

	TiXmlElement Pth("Path");
		Pth.SetAttribute("ins",	sInsPath);
		Pth.SetAttribute("copy",copyPath);
	root.InsertEndChild(Pth);

	TiXmlElement Last("Last");
		Last.SetAttribute("volume",	strF(fVol));
		Last.SetAttribute("bPlay",	strB(bPlay));
		Last.SetAttribute("pos",	(bPlay && tmTot > 0.0) ? strD(tmPl/tmTot): "0.0");
		Last.SetAttribute("playPls",strI(plsPlId));
		Last.SetAttribute("curPls",	strI(plsId));
	root.InsertEndChild(Last);

	TiXmlElement eViews("Views");
		TiXmlElement eVc("Vcur");
		view.SaveXml(&eVc);
		eViews.InsertEndChild(eVc);
		
		for (i = 0; i < MaxViews; i++)
		{
			TiXmlElement eVw("View");
			CViewSet& v = views[i];
			v.SaveXml(&eVw);
			eViews.InsertEndChild(eVw);
		}
	root.InsertEndChild(eViews);

	TiXmlElement eSeek("Seek");
		eSeek.SetAttribute("pos",		strI(iSpdSeek));
		eSeek.SetAttribute("vol",		strI(iSpdVol));

		for (i=0; i < vSpdSeek.size(); i++)
		{
			TiXmlElement sPos("Pos");
				sPos.SetAttribute("slow",	strI(vSpdSeek[i].s[0].add));
				sPos.SetAttribute("norm",	strI(vSpdSeek[i].s[1].add));
				sPos.SetAttribute("fast",	strI(vSpdSeek[i].s[2].add));
			eSeek.InsertEndChild(sPos);
		}
		for (i=0; i < vSpdVol.size(); i++)
		{
			TiXmlElement sVol("Vol");
				sVol.SetAttribute("slow",	strF1(vSpdVol[i].v[0]));
				sVol.SetAttribute("norm",	strF1(vSpdVol[i].v[1]));
				sVol.SetAttribute("fast",	strF1(vSpdVol[i].v[2]));
			eSeek.InsertEndChild(sVol);
		}
	root.InsertEndChild(eSeek);

	TiXmlElement ePls("Playlists");
		ePls.SetAttribute("counter",		strI(cntrPls));
	
		for (i=0; i < vPlst.size(); i++)
		{
			TiXmlElement plst("Pls");
				plst.SetAttribute("name",	vPlst[i]->name);
				plst.SetAttribute("bokm",	strI(vPlst[i]->bokm));
				// idx,cur,ofs on each playlist here--
			ePls.InsertEndChild(plst);
		}
	root.InsertEndChild(ePls);

	TiXmlElement eKeys("GlobalKeys");
		eKeys.SetAttribute("On",	strB(bHKeys));
		
		for (i = 0; i < FU_ALL; i++)
		{
			TiXmlElement key("Key");
				key.SetAttribute("on",	strB(vKeys[i].on));
				key.SetAttribute("vk",	strHex(vKeys[i].vk));
				key.SetAttribute("sc",	strHex(vKeys[i].sc));
				key.SetAttribute("mod",	strI(vKeys[i].mod));
				//key.SetAttribute("fun",	cstPlrFuns[vKeys[i].fun]);
			eKeys.InsertEndChild(key);
		}
	root.InsertEndChild(eKeys);

	TiXmlElement eExt("Ext");
		eExt.SetAttribute("all",	sExtAll.c_str());
		eExt.SetAttribute("mod",	sModAll.c_str());
	root.InsertEndChild(eExt);


	string p = appPath + "cAmp.xml";
	xml.InsertEndChild(root);  xml.SaveFile(p.c_str());
}


											///  Set  Colors  ///

//------------------------------------------------  Load  ------------------------------------------------
const static string sce = "Load colors.xml: ";
void cAmp::ClrLoad()
{
	vRclr.clear();  vTclr.clear();  rtx=D3DXCOLOR(0,1,0,1);  tmClrMode=0;

	string p = appPath + "Media\\colors.xml";
	TiXmlDocument xml;  xml.LoadFile(p.c_str());
	TiXmlElement* root = xml.RootElement(), *n = NULL, *m = NULL;
	if (!root) {	log(sce+"No set file cAmp.xml");  }
	else
	{	const char* a = NULL;

		n = root->FirstChildElement("RatingColors");	if (!n)  log(sce+"No <RatingColors>");
		{	m = NULL;
			if (n)  m = n->FirstChildElement("Rclr");  if (!m)  log(sce+"No <Rclr>");
			for (int i=0; i < chRall; ++i)
			{
				D3DXCOLOR c(0.2f,0.5f,0.8f,1.f);  if (i<cR0)  c=D3DXCOLOR(0.3f,0.6f,0.2f,1.f);
				if (m)	{
					a = m->Attribute("rgb");	if (a)	sscanf(a, "%f %f %f", &c.r, &c.g, &c.b);
					m = m->NextSiblingElement("Rclr");	}
				vRclr.push_back(c);
			}
		}
		
		n = root->FirstChildElement("RatingTex");	if (!n)  log(sce+"No <RatingColors>");
		if (n)  {
			a = n->Attribute("xmin");	if (a)	rtx.r = toFloat(a);
			a = n->Attribute("xmax");	if (a)	rtx.g = toFloat(a);
			a = n->Attribute("ymin");	if (a)	rtx.b = toFloat(a);
			a = n->Attribute("ymax");	if (a)	rtx.a = toFloat(a);
		}

		n = root->FirstChildElement("TimeColors");	if (!n)  log(sce+"No <TimeColors>");
		if (n)  {
			a = n->Attribute("mode");  if (a)  tmClrMode = toInt(a);

			m = n->FirstChildElement("Tclr");  if (!m)  log(sce+"No <Tclr>");
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
	char s[64];
	TiXmlDocument xml;	TiXmlElement root("cAmp");

	TiXmlElement Rclrs("RatingColors");
		for (size_t i = 0; i < vRclr.size(); i++)
		{	TiXmlElement rclr("Rclr");
				sprintf_s(s, sizeof(s)-1,
					"%.3f %.3f %.3f", vRclr[i].r, vRclr[i].g, vRclr[i].b);
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
				sprintf_s(s, sizeof(s)-1,
					"%.3f %.3f %.3f", vTclr[i].r, vTclr[i].g, vTclr[i].b);
				tclr.SetAttribute("rgb", s);
			Tclrs.InsertEndChild(tclr);
		}
	root.InsertEndChild(Tclrs);

	string p = appPath + "Media\\colors.xml";
	xml.InsertEndChild(root);  xml.SaveFile(p.c_str());
}
