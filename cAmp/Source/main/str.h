#pragma once
#define Usize  boost::uintmax_t  // file,total size


//  string utilities
class cStr
{
public:
	//  utils
	static void split(const std::string &s, char delim, std::vector<std::string> &v);
	static std::string lower(const std::string& s);
	static std::string upper(const std::string& s);

	//  convert ss
	static std::string iToStr(const int v,                         const char width=0/*, const char fill=' '*/);
	static std::string fToStr(const float v, const char precision, const char width=0, const char fill=' ');

	//  convert old, for set xml
	static char s[512];
	static char* strI(int i),    *strF(float i),  *strF1(float i),  *strB(bool b);  // uses s
	static char* strI6(INT64 i), *strD(double d), *strHex(byte b);

	#define chs  const char* str
	static int   toInt(chs);   static float toFloat(chs);    static bool toBool(chs);
	static INT64 toInt6(chs);  static double toDouble(chs);  static byte toByteHex(chs);

	//  time
	static void strTime(char* s, double ti, bool dec=false);  //old
	//static std::string strTime(double ti, bool dec=false);  // dec - for < 10s, with dot, eg. 9.5
	static std::string strTimeTot(long ti);

	//  hsv to rgb
	static void hsv2rgb(float h, float s, float v,
						float *r,float *g, float *b);  // out
};


//  playable audio file extensions
class cExt
{
public:
	static std::vector<std::string> ext,Ext;  // lower,upper chars

	static int Size()  // count
	{	return Ext.size();  }
	
	static void Init(const std::string all);
	static void Init();  // default
	const static std::string sVer1;
	
	static bool Find(std::string s);  // playable, true if s in ext, ignores case

	///  path,ext utils
	static std::string getExt(const std::string& filename);
	static void splitExt(const std::string& filename, std::string& name, std::string& ext);

	//  get path from full path\file
	static void splitPath(const std::string& spath, std::string& path, std::string& fname);
};
