#pragma once



class Graphics
{
public:
static HRESULT hr;
static void  SetupStates( PDev pDev );

static void  DrawRectTexC(PDev dev, PTex pTex, float x1,float y1, float x2,float y2, float xt1,float xt2, float yt1=0.f,float yt2=1.f);
static void  DrawRectTex( PDev dev, PTex pTex, float x1,float y1, float x2,float y2, float xt1,float xt2, float yt1=0.f,float yt2=1.f);
static void  DrawRectTex( PDev dev, PTex pTex, float x1,float y1, float x2,float y2);
static void  DrawRectTexClr(PDev dev,PTex pTex,float x1,float y1, float x2,float y2, DWORD rgb, float xt1=0.f,float xt2=1.f);
static void  DrawLine( PDev pDev, D3DXVECTOR3 p1, D3DXVECTOR3 p2, DWORD clr1, DWORD clr2 );
};


struct LINEVERTEX
{
	FLOAT  x,y,z;	DWORD  color;
};
const DWORD D3DFVF_LINEVERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE;

struct QuadVect
{
	D3DXVECTOR4 p;	float u,v;
};
const DWORD QuadVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

struct QuadVectClr
{
	D3DXVECTOR4 p;	DWORD rgb;  float u,v;
};
const DWORD QuadVFClr = D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE;


#define  Rtex	Graphics::DrawRectTex     //x1,y1, x2,y2,  xt1,xt2, yt1,yt2
#define  RtxC	Graphics::DrawRectTexClr  //x1,y1, x2,y2,  clr,  xt1, xt2
#define  clFl   D3DCLEAR_TARGET
