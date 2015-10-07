//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
sampler texSampler;
float g_Resolution;

float g_WeihtTabl_Zero;
float4 g_WeightTabl_One_Four;
//**********************************************************
// エントリポイント
//**********************************************************
float4 main(float4 diffuse : COLOR0, float2 uv : TEXCOORD0, float3 posW : TEXCOORD1) :COLOR0
{
	float4 outColor = tex2D(texSampler, float2(uv.x - g_Resolution * 8.f, uv.y)) * g_WeightTabl_One_Four.w
					+ tex2D(texSampler, float2(uv.x - g_Resolution * 6.f, uv.y)) * g_WeightTabl_One_Four.z
					+ tex2D(texSampler, float2(uv.x - g_Resolution * 4.f, uv.y)) * g_WeightTabl_One_Four.y
					+ tex2D(texSampler, float2(uv.x - g_Resolution * 2.f, uv.y)) * g_WeightTabl_One_Four.x
					+ tex2D(texSampler, float2(uv.x,					  uv.y)) * g_WeihtTabl_Zero
					+ tex2D(texSampler, float2(uv.x + g_Resolution * 2.f, uv.y)) * g_WeightTabl_One_Four.x
					+ tex2D(texSampler, float2(uv.x + g_Resolution * 4.f, uv.y)) * g_WeightTabl_One_Four.y
					+ tex2D(texSampler, float2(uv.x + g_Resolution * 6.f, uv.y)) * g_WeightTabl_One_Four.z
					+ tex2D(texSampler, float2(uv.x + g_Resolution * 8.f, uv.y)) * g_WeightTabl_One_Four.w;

	return outColor;
}

//EOF