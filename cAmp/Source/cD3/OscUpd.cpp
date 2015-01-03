#include "header.h"

#include "Osc.h"



//------------------------------  init
cOsc::cOsc()
{
	/*xL = xR = 0.f;*/
	dt = 0.01f;  tmd = 0;  view.fftSize=1;
	memset(A,0,sizeof(A));
	lock = false;  xpr = 0;  xpn = 0;  //ptim.iv
	bRec=0; bPlay=0; bPaused=0;
	UpdPClr();
}

//  update print colors
//------------------------------
void cOsc::UpdPClr()
{
	//  print colors
	float l = PrClrs-1,f, r,g,b, h,s,v;
	for (int i=0; i<PrClrs; ++i)
	{
		f = float(i)/l;
		//float r = 0.5*f, g = 0.78*f, b = 1.0*f;
		//pcl[i] = D3DCOLOR_COLORVALUE(r,g,b,0);
		//_par
		h = 0.65f - 0.13f * pow(f, 1.2f);
		s = 0.9f - 0.8f * pow(f, 1.6f);
		v = pow(f, 0.4f);
		hsv2rgb(h,s,v,&r,&g,&b);
		pcl[i] = D3DCOLOR_COLORVALUE(r,g,b,0);
	}
}

//  hsv to rgb  values in 0..1
void cOsc::hsv2rgb(float h, float s, float v, float *r,float *g, float *b)
{   
	if (s == 0.0)
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

//------------------------------  time string
void cOsc::strTime(char* s, double ti, bool d)
{
	DWORD t=ti, th = t/3600, tm = t/60%60, ts = t%60;
	if (d)
	{	StringCbPrintfA(s,40, "%c.%c",
		/*.s*/ DWORD(ti*10.0)%10+'0', /*s*/ ts%10+'0');  return;  }
	
	StringCbPrintfA(s,40, "%c%c:%c%c",
		/*s*/ ts%10+'0', /*10s*/ (ts>9||tm>0||th>0)? ts/10+'0':' ',//0
		/*m*/ (tm>0||th>0)? tm%10+'0':' ', /*10m*/(tm>9||th>0)? tm/10+'0':' ');
	if (th > 0) {	int l=strlen(s);  s[l]='.';  s[l+1]= th%10+'0';  s[l+2]=0;  /*h*/  }
}


//  time str - old
#define s(c)  s[c] = BYTE(a%10) + '0';
#define ch  {  if (a==0)  s[c] = ' ';  else  s(c)  a/=10;  }

void cOsc::sSize(DWORD a)
{
	int c;  s[8] = 0;  s[4] = ',';
	for (c=7; c>=5; c--)  ch
	for (c=3; c>=0; c--)  ch
}

void cOsc::sTime(DWORD a, char* s)
{
	a /= 4*441;  s[9] = 0;
	s(8)  a/=10;	s(7)  a/=10;
	s[6] = '.';
	s(5)  a/=10;	s[4] = BYTE(a%6) + '0';  a/=6;
	s[3] = ':';
	s(2)  a/=10;
	if (a==0)  s[1] = ' ';  else  s(1)  a/=10;
	if (a==0)  s[0] = ' ';  else  s(0)
}
#undef s
												
//------------------------------  utility
// hex
const char chHex[17]="0123456789ABCDEF";
static BYTE hex1(char c)
{
	if(c>='0'&&c<='9') return c-'0';
	if(c>='A'&&c<='Z') return c-'A'+10;
	if(c>='a'&&c<='z') return c-'a'+10;
	return 0;
}

//  from str
int		cOsc::toInt(chs)	{	return (int)strtol(str, NULL, 0);	}
INT64	cOsc::toInt6(chs)	{	return strtol(str, NULL, 0);	}
float	cOsc::toFloat(chs)	{	return (float)atof(str);	}
double	cOsc::toDouble(chs)	{	return atof(str);	}
bool	cOsc::toBool(chs)	{	return ((int)strtol(str, NULL, 0) > 0) ? true : false;	}
BYTE	cOsc::toByteHex(chs){	if (strlen(str)!=2)  return 0;  else  return hex1(str[0])*16+hex1(str[1]);  }

//  to str
char* cOsc::strI( int i)	{	p(s)"%d", i);		return s;	}
char* cOsc::strI6(INT64 i)	{	p(s)"%d", i);		return s;	}
char* cOsc::strF( float f)	{	p(s)"%.3f", f);		return s;	}
char* cOsc::strD( double d)	{	p(s)"%f", d);		return s;	}
char* cOsc::strB( bool b)	{	p(s)"%d", b?1:0);	return s;	}
char* cOsc::strHex(BYTE b)	{	s[0]=chHex[b/16];  s[1]=chHex[b%16];  s[2]=0;	return s;	}

char cOsc::s[300], cOsc::appPath[MAX_PATH];
