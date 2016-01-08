//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
sampler texSampler;
sampler toonSampler;
sampler depthSampler;
//**********************************************************
// エントリポイント
//**********************************************************
float4 main(float2 inUV : TEXCOORD0
			, float2 inToonUV : TEXCOORD1
			, float4 inWLightPos : TEXCOORD2
			, float4 inWVPPos : TEXCOORD3
			, float4 inColor : COLOR0
			) :COLOR0
{
	float4 color = tex2D(texSampler, inUV) * tex2D(toonSampler, inToonUV);

	float4 pos = inWLightPos;
	float depth = pos.z / pos.w;	// 深度値を入れる

	// テクスチャ座標に変換
	float2 TransTexCoord;
	TransTexCoord.x = 0.5f + pos.x / pos.w * 0.5f;
	TransTexCoord.y = 0.5f - pos.y / pos.w * 0.5f;

	// 深度値取り出す
	float4 depthTexColor = tex2D(depthSampler, TransTexCoord);
	float texDepth = depthTexColor.r
						+ depthTexColor.g * (1.f / 255.f)
						+ depthTexColor.b * (1.f / (255.f * 255.f));

	// 影かどうかの判断
	float s = depth > texDepth + 0.0001f ? 0.2f : 1.0f;
	color.rgb *= s;
	color.a = 1.f;

	return color;
}

//EOF