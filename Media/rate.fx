//  vars
float3 color;

//  textures
texture TexDiff;
sampler DiffS = sampler_state  {  Texture = <TexDiff>;  };

//  structs
struct PS_IN  {  float2 tc:TEXCOORD0;  };
  


// .. . .  .            . .. Pixel .. .            .  . . ..

vector PS(PS_IN i):COLOR
{
	float3 tx = tex2D(DiffS, i.tc) * color;

	return vector(tx, 1.f);
}


//  _ __ tech __ _

technique T0
{
	pass P0
	{
		PixelShader = compile ps_2_0 PS();
	}
}
