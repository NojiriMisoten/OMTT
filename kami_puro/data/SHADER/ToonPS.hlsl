//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
sampler normalRendereTexSampler;	// 普通に書いたやつ
sampler normalVecTexSampler;		// 法線抽出したやつ
float2 g_ScreenResolution;	// 解像度

//**********************************************************
// エントリポイント
//**********************************************************
float4 main(float4 diffuse : COLOR0, float2 uv : TEXCOORD0, float3 posW : TEXCOORD1) :COLOR0
{
	// エッジだす
	float2 pixelOffset = 1.f / g_ScreenResolution;

	// 左上、右下、左下、右上
	// 色取得
	float4 n1 = tex2D(normalVecTexSampler, uv + float2(-1.f, -1.f) * pixelOffset);
	float4 n2 = tex2D(normalVecTexSampler, uv + float2(1.f, 1.f) * pixelOffset);
	float4 n3 = tex2D(normalVecTexSampler, uv + float2(-1.f, 1.f) * pixelOffset);
	float4 n4 = tex2D(normalVecTexSampler, uv + float2(1.f, -1.f) * pixelOffset);
	n1.xyz = 2.0f * n1.xyz - float3(1.f, 1.f, 1.f);
	n2.xyz = 2.0f * n2.xyz - float3(1.f, 1.f, 1.f);
	n3.xyz = 2.0f * n3.xyz - float3(1.f, 1.f, 1.f);
	n4.xyz = 2.0f * n4.xyz - float3(1.f, 1.f, 1.f);

	// 差分を算出する
	float4 diagnalDelta = abs(n1 - n2) + abs(n3 - n4);

	// Z差分法
	float depthDelta = diagnalDelta.a;
	//								　　↓しきい値　↓感度
	depthDelta = saturate((depthDelta - 20.9f) * 0.8f);
	
	// 法線差分法
	float normalDelta = dot(diagnalDelta.rgb, 1);
	normalDelta = saturate((normalDelta - 20.98f) * 0.8f);
	
	// Z差分+法線差分
	float edgeAmount = saturate(depthDelta + normalDelta) * 1.0f;

	return (float4((float3)(1.f - edgeAmount), 1.0f)) * tex2D(normalRendereTexSampler, uv);	// テクスチャ;
}

//-----EOF-----