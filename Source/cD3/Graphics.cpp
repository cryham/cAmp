#include "header.h"
#include "D3.h"
#include "Graphics.h"



//	Setup render states
//--------------------------------------------------------------------------------------
void Graphics::SetupStates(PDev pDev)
{
	pDev->SetRenderState(D3DRS_AMBIENT, 0);

	pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	pDev->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0);

	pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);  //CCW
	pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDev->SetRenderState( D3DRS_ZENABLE, FALSE);
	pDev->SetRenderState( D3DRS_LIGHTING, FALSE);
	pDev->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);

	D3DXMATRIX Mid;  D3DXMatrixIdentity(&Mid);
	pDev->SetTransform(D3DTS_PROJECTION, &Mid);
	pDev->SetTransform(D3DTS_VIEW, &Mid);
	pDev->SetTransform(D3DTS_WORLD, &Mid);
}


///  Render Texture Rectangle
#define V4(x,y)  D3DXVECTOR4(x,y,1,1)

//  colored texture
void Graphics::DrawRectTexClr( PDev dev, PTex pTex, int id,
	float x1,float y1, float x2,float y2, DWORD rgb, float xt1,float xt2 )
{
	QuadVectClr q[4];
	float sx = texUV[id][0]/512.f, sy = texUV[id][1]/512.f, px = texUV[id][2]/512.f, py = texUV[id][3]/512.f;
	q[0].p = V4(x1, y1);  q[0].u = xt1*sx+px;  q[0].v = 0.f*sy+py;  q[0].rgb = rgb;
	q[1].p = V4(x2, y1);  q[1].u = xt2*sx+px;  q[1].v = 0.f*sy+py;  q[1].rgb = rgb;
	q[2].p = V4(x2, y2);  q[2].u = xt2*sx+px;  q[2].v = 1.f*sy+py;  q[2].rgb = rgb;
	q[3].p = V4(x1, y2);  q[3].u = xt1*sx+px;  q[3].v = 1.f*sy+py;  q[3].rgb = rgb;

	dev->SetFVF(QuadVFClr);
	dev->SetTexture(0, pTex);
	Vd( dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, q, sizeof(QuadVectClr)) );
}

//  no texure, for effect
void Graphics::DrawRectTexC( PDev dev, PTex pTex, int id,
	float x1,float y1, float x2,float y2, float xt1,float xt2, float yt1, float yt2 )
{
	QuadVect q[4];
	float sx = texUV[id][0]/512.f, sy = texUV[id][1]/512.f, px = texUV[id][2]/512.f, py = texUV[id][3]/512.f;
	q[0].p = V4(x1, y1);  q[0].u = xt1*sx+px;  q[0].v = yt1*sy+py;
	q[1].p = V4(x2, y1);  q[1].u = xt2*sx+px;  q[1].v = yt1*sy+py;
	q[2].p = V4(x2, y2);  q[2].u = xt2*sx+px;  q[2].v = yt2*sy+py;
	q[3].p = V4(x1, y2);  q[3].u = xt1*sx+px;  q[3].v = yt2*sy+py;

	dev->SetFVF(QuadVF);
	Vd( dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, q, sizeof(QuadVect)) );
}

//  normal texture
void Graphics::DrawRectTex( PDev dev, PTex pTex, int id,
	float x1,float y1, float x2,float y2, float xt1,float xt2, float yt1, float yt2 )
{
	QuadVect q[4];
	float sx = texUV[id][0]/512.f, sy = texUV[id][1]/512.f, px = texUV[id][2]/512.f, py = texUV[id][3]/512.f;
	q[0].p = V4(x1, y1);  q[0].u = xt1*sx+px;  q[0].v = yt1*sy+py;
	q[1].p = V4(x2, y1);  q[1].u = xt2*sx+px;  q[1].v = yt1*sy+py;
	q[2].p = V4(x2, y2);  q[2].u = xt2*sx+px;  q[2].v = yt2*sy+py;
	q[3].p = V4(x1, y2);  q[3].u = xt1*sx+px;  q[3].v = yt2*sy+py;

	dev->SetFVF(QuadVF);  //?if (pTex) //
	dev->SetTexture(0, pTex);
	Vd( dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, q, sizeof(QuadVect)) );
}

void Graphics::DrawRectTex( PDev dev, PTex pTex, int id,
	float x1, float y1, float x2, float y2)
{
	DrawRectTex(dev, pTex, id,  x1,y1, x2,y2, 0.f,1.f, 0.f,1.f);
}

HRESULT Graphics::hr;