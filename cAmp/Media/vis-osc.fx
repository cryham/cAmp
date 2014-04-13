//  vars
float gWidth = 0.0017f, gPow = 0.12f;

//  textures
texture TexVal, TexVal2;
sampler ValS = sampler_state  {  Texture = <TexVal>;  	ADDRESSU = CLAMP;	ADDRESSV = CLAMP;  };
sampler ValS2= sampler_state  {  Texture = <TexVal2>;  	ADDRESSU = CLAMP;	ADDRESSV = CLAMP;  };

//  structs
struct PS_IN  {  float2 tc:TEXCOORD0;  };
  


float fp(float b,float x,float p,float c)
{
	float y = b - pow(c*x/256.f, p);
	y /= 256.f;
	if (y < 0.f)  y = 0.f;
	return y;
}

// .. . .  .            . .. Pixel .. .            .  . . ..

vector PS(PS_IN i, uniform bool bFlt):COLOR
{
	float3 tx = tex1D(ValS, i.tc.x);
	float y = i.tc.y, v,vv, z;
	//const float s = 0.0017f, ss = 136*s;
	float s = gWidth;

	if (!bFlt)  {  // from,to
		v = tx.b;  vv = tx.g;  
	}else{
		float3 tx2 = tex1D(ValS2, i.tc.x);
		v = tx.r;  vv = tx2.r;  // flt 2tex
	}

	if (vv < v)  { z=v;  v=vv;  vv=z; }  //xchg

	float e = v-y, f = y-vv;
	if (e<0) e=0;  if (f<0) f=0;
	//if (e > ss)  discard;
	//if (f > ss)  discard;
	
	///  clr
	float r=0,g=0,b=0, x;
	
	if (y > v-s && y < vv+s)  {
	x = (v-vv)*600.f;
	b = fp(225-110,x, 1.6f, 11.f)+110/256.f;
	g = fp(190-95, x, 2.1f, 28.f)+95/256.f;
	r = fp(140-80, x, 1.8f, 31.f)+80/256.f;
	return vector(r,g,b, 1.f);  }

	//x = pow((e+f)*1.22f,0.12f);  //0.13f
	//x = pow((e+f)*1.2f, 0.16f);  //0.22f
	x = pow((e+f)*1.2f, gPow);  //0.22f
	b = 0.96-x;
	g = 0.86-x;
	r = 0.61-x;

	return vector(r,g,b, g);
	//return vector(0,0,0, 1.f);
}


//  _ __ tech __ _

technique T0
{
	pass P0
	{
		PixelShader = compile ps_2_0 PS(false);
	}
}
technique T1
{
	pass P0
	{
		PixelShader = compile ps_2_0 PS(true);
	}
}
