//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
sampler texSampler;

//**********************************************************
// エントリポイント
//**********************************************************
float4 main(float4 diffuse : COLOR0, float2 uv : TEXCOORD0) :COLOR0
{
	float3 color = tex2D(texSampler, uv).rgb;
	float briteness = color.x * 0.299f + color.y * 0.587f + color.z * 0.114f;
	
	float4 outColor;
	outColor.rgb = briteness > 0.3f ? color : float3(0,0,0);
	outColor.a = 1.0f;

	return outColor;
	
}

//EOF