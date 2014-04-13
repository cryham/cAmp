//  vars

//  textures
texture TexVal;
sampler ValS = sampler_state  {  Texture = <TexVal>;
	ADDRESSU = CLAMP;	ADDRESSV = CLAMP;  };

//  structs
struct PS_IN  {  float2 tc:TEXCOORD0;  };
  


float fp(float b,float x,float p,float c)
{
	float y = b - pow(c*x/256.f, p);
	y /= 256.f;
	//if (y < 0.f)  y = 0.f;
	return y;
}

// .. . .  .            . .. Pixel .. .            .  . . ..

vector PS(PS_IN i, uniform bool bFlt):COLOR
{
	float3 tx = tex1D(ValS, i.tc.x);
	float y = i.tc.y,
		v = bFlt ? tx.r : tx.b;  // val

	if (y < v)  discard;
	
	///  clr
	float r,g,b, x;
	
	x = 71.f*v - 16.f*y;  if (x < 0.f)  x = 0.f;
	
	b = fp(205, x, 1.6f, 8.f);
	g = fp(180, x, 2.1f, 55.f);
	r = fp(140, x, 2.4f, 65.f);

	return vector(r,g,b, 1.f);
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
