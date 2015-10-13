//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float4x4 gView;			// ビュー
float4x4 gProj;			// プロジェクション
float4x4 gWorld[52];	// ワールド配列
float3 gCameraDir;		// カメラのベクトル
float4x4 gLightVP;
//**********************************************************
// エントリポイント
//**********************************************************
void main(in float3 inPos:POSITION0		// 引数 FVF等に合わせる
	, in float3 inBlendWeight : BLENDWEIGHT0
	, in float4 inBlendIndex : BLENDINDICES
	, in float3 inNormal : NORMAL0		// 引数 法線
	, in float2 inUV : TEXCOORD0		// 引数 FVF等に合わせる
	, out float4 outPos : POSITION0		// 返す値 必ずfloat4のPOSITONを返さなければならない システム(GPU)に行く ピクセルシェーダーに渡したいときはTEXCOORD~が自由に使えるのでそれを使う
	, out float2 outUV : TEXCOORD0		// 返す値 ピクセルシェーダーに線形補間されて届く
	, out float2 outToonUV : TEXCOORD1
	, out float4 outWLightPos : TEXCOORD2
	, out float4 outWVPPos : TEXCOORD3
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
	float4x4 world = weight0 * world0
					+ weight1 * world1
					+ weight2 * world2
					+ weight3 * world3;
	float4x4 wvp = mul(world ,gView);
	wvp = mul(wvp, gProj);

	outPos = mul(float4(inPos, 1.0f), wvp);
	outWVPPos = mul(float4(inPos, 1.0f), wvp);

	float u = 0.5f;
	float v = 0.1 - dot(gCameraDir, inNormal) * 0.2f;
	float2 uv = float2(u, v);
	outToonUV = uv;
	outUV = inUV;

	float4x4 lightWVP = world * gLightVP;
	outWLightPos = mul(float4(inPos, 1.0f), lightWVP);
}
//EOF