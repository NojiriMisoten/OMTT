//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
sampler toonRenderSampler;
sampler fadeRenderSampler;
sampler lightBloomRenderSampler;
sampler uiRenderSampler;

//**********************************************************
// エントリポイント
//**********************************************************
float4 main(float4 diffuse : COLOR0, float2 uv : TEXCOORD0) :COLOR0
{
	float4 color = (tex2D(toonRenderSampler, uv))
				+ tex2D(lightBloomRenderSampler, uv)
				+ tex2D(uiRenderSampler, uv)
				+ tex2D(fadeRenderSampler, uv);
	return color;
}

//EOF