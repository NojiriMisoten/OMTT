//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
sampler texSampler;
float3 gMonochromeScale;
float gAlpha;
//**********************************************************
// エントリポイント
//**********************************************************
float4 main(float2 uv : TEXCOORD0) :COLOR0
{
	float4 color = tex2D(texSampler, uv);
	float grayColor = dot(color.rgb, gMonochromeScale);
	color *= float4(grayColor, grayColor, grayColor, gAlpha);
	return color;
}

//EOF