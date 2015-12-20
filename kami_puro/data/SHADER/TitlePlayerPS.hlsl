//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
sampler texSampler;
sampler toonSampler;
float3 gMonochromeScale;
int gMode;
//**********************************************************
// エントリポイント
//**********************************************************
float4 main(float2 inUV : TEXCOORD0
	, float2 inToonUV : TEXCOORD1
	) :COLOR0
{
	float4 color = tex2D(texSampler, inUV) * tex2D(toonSampler, inToonUV);
	
	if (gMode == 0)
	{
		float grayColor = dot(color.rgb, gMonochromeScale);
		color = float4(grayColor, grayColor, grayColor, 1.0f);
	}
	else if(gMode == 1)
	{
		float grayColor = dot(color.rgb, gMonochromeScale);
		float3 sepiaScale = float3(1.07f, 0.74f, 0.43f);
		float3 culcColor = float3(grayColor, grayColor, grayColor) * sepiaScale;
		color = float4(culcColor, 1.0f);
	}
	return color;
}

//EOF