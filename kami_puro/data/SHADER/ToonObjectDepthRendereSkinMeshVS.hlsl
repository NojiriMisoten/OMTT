//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float4x4 gView;			// ビュープロジェクション
float4x4 gProj;			// プロジェクション
float4x4 gWorld[52];	// ワールドマトリクス
int gBlendNum;
//**********************************************************
// エントリポイント
//**********************************************************
void main(in float3 inPos:POSITION0				// 引数 FVF等に合わせる
	, in float3 inBlendWeight : BLENDWEIGHT0
	, in float4 inBlendIndex : BLENDINDICES0
	, in float3 inNormal : NORMAL0		// 引数 法線
	, in float2 inUV : TEXCOORD0		// 引数 FVF等に合わせる
	, in float4 inColor : COLOR0
	, out float4 outPos : POSITION0		// 返す値 必ずfloat4のPOSITONを返さなければならない システム(GPU)に行く ピクセルシェーダーに渡したいときはTEXCOORD~が自由に使えるのでそれを使う
	, out float4 outColor : COLOR0
	, out float4 outWPos : TEXCOORD1
	)
{
	int index[4] = (int[4])inBlendIndex;

	float weight[4] = (float[4])0.f;
	weight[0] = inBlendWeight.x;
	weight[1] = inBlendWeight.y;
	weight[2] = inBlendWeight.z;
	weight[3] = 1.0f - weight[0] - weight[1] - weight[2];

	float4x4 tempWorld[4] = (float4x4[4])0.f;
	tempWorld[0] = gWorld[index[0]];
	tempWorld[1] = gWorld[index[1]];
	tempWorld[2] = gWorld[index[2]];
	tempWorld[3] = gWorld[index[3]];

	// 重みこみワールド行列
	float LastBlendWeight = 0.0f;			// 最後の行列に掛けられる重み
	float4x4 matCombWorld = 0.0f;			// 合成ワールド変換行列

	if (gBlendNum > 1)
	{
		// ワールド変換行列をブレンド
		for (int i = 0; i < gBlendNum - 1; i++)
		{
			LastBlendWeight += weight[i];   // 最後の重みをここで計算しておく
			matCombWorld += tempWorld[i] * weight[i];
		}

		// 最後の重みを足し算
		matCombWorld += gWorld[gBlendNum - 1] * (1.0f - LastBlendWeight);
	}
	else
	{
		matCombWorld += gWorld[index[0]];
	}

	float4x4 world = matCombWorld;
	float4x4 wvp = mul(world, gView);
	wvp = mul(wvp, gProj);

	outPos = mul(float4(inPos, 1.0f), wvp);
	outWPos = outPos;
	outColor = float4(1, 1, 1, 1);
}
//EOF