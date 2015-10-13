//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float4x4 gWorld[52];		// ワールドマトリクス
float4x4 gView;			// ビューマトリクス
float4x4 gProj;			// プロジェクション

//**********************************************************
// エントリポイント
//**********************************************************
void main(in float3 inPos:POSITION0				// 引数 FVF等に合わせる
	, in float3 inNormal : NORMAL0		// 引数 法線
	, in float2 inUV : TEXCOORD0		// 引数 FVF等に合わせる
	, in float3 inBlendWeight : BLENDWEIGHT0
	, in float4 inBlendIndex : BLENDINDICES
	, out float4 outPos : POSITION0		// 返す値 必ずfloat4のPOSITONを返さなければならない システム(GPU)に行く ピクセルシェーダーに渡したいときはTEXCOORD~が自由に使えるのでそれを使う
	, out float4 outColor : COLOR0
	, out float4 outWPos : TEXCOORD1
	)
{
	int index0, index1, index2, index3;
	float weight0, weight1, weight2, weight3;
	float4x4 world0, world1, world2, world3;

	index0 = inBlendIndex.x;
	index1 = inBlendIndex.y;
	index2 = inBlendIndex.z;
	index3 = inBlendIndex.w;

	weight0 = inBlendWeight.x;
	weight1 = inBlendWeight.y;
	weight2 = inBlendWeight.z;
	weight3 = 1.0f - weight0 - weight1 - weight2;

	world0 = gWorld[index0];
	world1 = gWorld[index1];
	world2 = gWorld[index2];
	world3 = gWorld[index3];

	// 重みこみワールド行列
	float4x4 world = world0 * weight0
		+ world1 * weight1
		+ world2 * weight2
		+ world3 * weight3;
	float4x4 wvp = world * gView * gProj;
	outPos = mul(float4(inPos, 1.0f), wvp);

	outWPos = outPos;
	// View座標系に変換
	float4 vNormal = mul(float4(inNormal, 0.0f), world);
	//法線　-1〜+1を0〜1に
	vNormal.x = 0.5f * (vNormal.x + 1.f);
	vNormal.y = 0.5f * (vNormal.y + 1.f);
	vNormal.z = 0.5f * (vNormal.z + 1.f);
	vNormal.w = 1.f;

	outColor = vNormal;
	outColor.a = 1.f;
}
//EOF