#include "header.h"
#include <sstream>
#include "str.h"
using namespace std;


///  file extensions
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
vector<string> cExt::ext, cExt::Ext;

const string cExt::sVer1 = "WAV|FLAC|OGG|MP3|APE|WV|MPC|WMA|MP2";


void cExt::Init()
{
	Init(sVer1);
}

void cExt::Init(const string str)
{
	Ext.clear();
	ext.clear();

	cStr::split(str, '|', Ext);

	for (size_t i=0; i < Ext.size(); ++i)
		ext.push_back(cStr::lower(Ext[i]));
}

bool cExt::Find(string s)
{
	string ss = cStr::lower(s);
	for (size_t i=0; i < ext.size(); ++i)
		if (ss == ext[i])  rt
	rf
}

//  filename ext
string cExt::getExt(const string& s)
{
	string e;
	string::size_type p = s.find_last_of('.');
	if (p == string::npos)
		return e;
	e = s.substr(p+1);
	return e;
}


///  split
void cExt::splitExt(const string& s, string& name, string& ext)
{
	string::size_type p = s.find_last_of('.');
	if (p == string::npos)
	{	name = s;  ext = "";  return;  }

	name = s.substr(0, p);  // skip .
	ext = s.substr(p+1);
}

void cExt::splitPath(const string& s, string& path, string& fname)
{
	string::size_type p = s.find_last_of("\\/");
	if (p == string::npos)
	{	path = s;  fname = "";  return;  }

	path = s.substr(0, p+1);
	fname = s.substr(p+1);
}


///  utils
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 

void cStr::split(const string &s, char delim, vector<string> &v)
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
		v.push_back(item);
}

string cStr::lower(const string& s)
{
	string ss = s;
	transform(ss.begin(), ss.end(), ss.begin(), ::tolower);
	return ss;
}
string cStr::upper(const string& s)
{
	string ss = s;
	transform(ss.begin(), ss.end(), ss.begin(), ::toupper);
	return ss;
}


///  Convert
//---------------------------------------------------------------------------------------------------
string cStr::iToStr(const int v, const char width/*, const char fill*/)
{
	ostringstream s;
	if (width != 0)  s.width(width);  //s.fill(fill);
	s << std::fixed << v;
	return s.str();
}

string cStr::fToStr(const float v, const char precision, const char width, const char fill)
{
	ostringstream s;
	if (width != 0)  s.width(width);  s.fill(fill);  s.precision(precision);
	s << std::fixed << v;
	return s.str();
}

///  Old
//  from str
int		cStr::toInt(chs)	{	return (int)strtol(str, NULL, 0);	}
INT64	cStr::toInt6(chs)	{	return		strtol(str, NULL, 0);	}
float	cStr::toFloat(chs)	{	return (float)atof(str);	}
double	cStr::toDouble(chs)	{	return		  atof(str);	}
bool	cStr::toBool(chs)	{	return ((int)strtol(str, NULL, 0) > 0) ? true : false;	}

//  to str
char cStr::s[512]={0};
char* cStr::strI(int i)		{	sprintf(s,"%d", i);		return s;	}
char* cStr::strI6(INT64 i)	{	sprintf(s,"%d", i);		return s;	}
char* cStr::strF(float f)	{	sprintf(s,"%.3f", f);	return s;	}
char* cStr::strF1(float f)	{	sprintf(s,"%.1f", f);	return s;	}
char* cStr::strD(double d)	{	sprintf(s,"%f", d);		return s;	}
char* cStr::strB(bool b)	{	sprintf(s,"%d", b?1:0);	return s;	}


///  Hex
const char chHex[17] = "0123456789ABCDEF";
static byte hex1(char c)
{
	if (c>='0' && c<='9')  return c-'0';
	if (c>='A' && c<='F')  return c-'A'+10;
	if (c>='a' && c<='f')  return c-'a'+10;
	return 0;
}

byte  cStr::toByteHex(chs)
{
	return (strlen(str)!=2) ? 0 : hex1(str[0])*16 + hex1(str[1]);
}
char* cStr::strHex(byte b)
{
	s[0] = chHex[b/16];  s[1] = chHex[b%16];  s[2]=0;
	return s;
}


///  Time string  (chars reversed, needs dir = -1)
//---------------------------------------------------------------------------------------------------
void cStr::strTime(char* s, double ti, bool d)
{
	DWORD t=ti, th = t/3600, tm = t/60%60, ts = t%60;
	if (d)  // < 10s  1.2
	{	StringCbPrintfA(s,40, "%c.%c",
		/*.s*/ DWORD(ti*10.0)%10+'0', /*s*/ ts%10+'0');  return;  }
	
	if (tm==0 && th==0)  // < 1m  :12
	{	StringCbPrintfA(s,40, "%c%c: ",
		/*s*/ ts%10+'0', /*10s*/ (ts>9)? ts/10+'0':' '/*0*/);  return;  }

	if (tm <= 9 && th==0)  // < 10m  1:23
	{	StringCbPrintfA(s,40, "%c%c:%c ",
		/*s*/ ts%10+'0', /*10s*/ ts/10+'0', /*m*/ tm%10+'0');  return;  }

	StringCbPrintfA(s,40, "%c%c:%c%c ",  // >10m  12:34
		/*s*/ ts%10+'0', /*10s*/ ts/10+'0', /*m*/ tm%10+'0', tm/10+'0');
		
	if (th > 0) {	int l=strlen(s)-1;
		s[l]='.';  s[l+1]= th%10+'0';  s[l+2]=' ';  s[l+3]=0;  /*h*/  }
}

#if 0
string cStr::strTime(double ti, bool dec)
{
	string s;   // hour, min, sec
	long t = ti, th = t/3600, tm = t/60%60, ts = t%60;
	if (dec)
	{	s = string("")+
			char( int(ti*10.0)%10+'0')  /*.s*/
			+ "." +
			char( ts%10+'0');  /*s*/
		return s;
	}
	s =	string("")+
		char( ts%10+'0') +  /*s*/
		char( (ts>9||tm>0||th>0)? ts/10+'0':' ')  //0  /*10s*/
		+ ":" +
		char( (tm>0||th>0)? tm%10+'0':' ' ) +  /*m*/
		char( (tm>9||th>0)? tm/10+'0':' ' );  /*10m*/
		
	if (th > 0)
	{	s += string(".") +
			char( th%10+'0' );
		if (th>9)
			s += string("")+ char( th/10+'0' );
	}
	return s;
}
#endif

string cStr::strTimeTot(long ti)
{
	string s;  // sec, min, hour, days
	long t = ti, ts,tm,th,td;
	ts = t%60;  t/=60;  tm = t%60;  t/=60;  th = t%24;  td = t/24;
	
	s = string("") +
		char( tm%10+'0' ) +
		char( tm/10+'0' );

	if (th > 0 || td > 0)
	{	s += string(" h") +
			char( th%10+'0' ) +
			char( th>9? th/10+'0': td>0?'0':' ' );
		if (td > 0)
			s += string(" d") +
				char( td%10+'0') +
				char( td>9? td/10%10+'0':' ') +
				char( td>99? td/100%10+'0':' ');
	}
	return s;
}


///  hsv to rgb  values in 0..1
//------------------------------------------------------------
void cStr::hsv2rgb(float h, float s, float v,
					float *r,float *g, float *b)
{   
	if (s == 0.f)
	{	*r = v;  *g = v;  *b = v;	return;  }

	if (h >= 1.f)  h = 0.f;
	else  h *= 6.f;

	int i = (int)h;   // in the range 0..5
	float f = h - i;  // fractional part

	float m = v * (1.f - s);
	float n = v * (1.f - s * f);
	float k = v * (1.f - s * (1.f - f));

	if (i == 0) {  *r = v;  *g = k;  *b = m;  }else
	if (i == 1) {  *r = n;  *g = v;  *b = m;  }else
	if (i == 2) {  *r = m;  *g = v;  *b = k;  }else
	if (i == 3) {  *r = m;  *g = n;  *b = v;  }else
	if (i == 4) {  *r = k;  *g = m;  *b = v;  }else
	if (i == 5) {  *r = v;  *g = m;  *b = n;  }
}
