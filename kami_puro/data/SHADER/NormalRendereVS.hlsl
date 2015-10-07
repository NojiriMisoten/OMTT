//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float4x4 gWVP;			// ワールドビュープロジェクション
float3 gCameraDir;		// カメラのベクトル
float4x4 gLightWVP;
//**********************************************************
// エントリポイント
//**********************************************************
void main(in float3 inPos:POSITION0		// 引数 FVF等に合わせる
	, in float3 inNormal : NORMAL0		// 引数 法線
	, in float2 inUV : TEXCOORD0		// 引数 FVF等に合わせる
	, out float4 outPos : POSITION0		// 返す値 必ずfloat4のPOSITONを返さなければならない システム(GPU)に行く ピクセルシェーダーに渡したいときはTEXCOORD~が自由に使えるのでそれを使う
	, out float2 outUV : TEXCOORD0		// 返す値 ピクセルシェーダーに線形補間されて届く
	, out float2 outToonUV : TEXCOORD1
	, out float4 outWLightPos : TEXCOORD2
	, out float4 outWVPPos : TEXCOORD3
	)
{
	outPos = mul(float4(inPos, 1.0f), gWVP);

	outUV = inUV;

	float u = 0.5f;
	float v = 0.1 - dot(gCameraDir, inNormal) * 0.2f;
	float2 uv = float2(u, v);

	outToonUV = uv;
	outWLightPos = mul(float4(inPos, 1.0f), gLightWVP);
	outWVPPos = mul(float4(inPos, 1.0f), gWVP);
}
//EOF