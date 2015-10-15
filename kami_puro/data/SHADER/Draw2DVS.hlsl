//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float4x4 gWorld;			// ワールドマトリクス
float4x4 gScreenProj;		// 射影行列

//**********************************************************
// エントリポイント
//**********************************************************
void main(in float3 inPos : POSITION		// 引数 FVF等に合わせる
	, in float4 inDiffuse : COLOR0		// 引数 FVF等に合わせる	FVF_VERTEX_2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	, in float2 inUV : TEXCOORD0		// 引数 FVF等に合わせる
	, in float3 inNormal : NORMAL0		// 引数 法線
	, out float4 outPos : POSITION		// 返す値 必ずfloat4のPOSITONを返さなければならない システム(GPU)に行く ピクセルシェーダーに渡したいときはTEXCOORD~が自由に使えるのでそれを使う
	, out float4 outDiffuse : COLOR0	// 返す値 ピクセルシェーダーに線形補間されて届く
	, out float2 outUV : TEXCOORD0		// 返す値 ピクセルシェーダーに線形補間されて届く
	, out float3 posW : TEXCOORD1)		// 返す値 ピクセルシェーダーに送るワールド座標
{
	// ピクセルシェーダーに送るワールド座標
	posW = mul(float4(inPos, 1.0f), gWorld).xyz;

	// 2D変換
	outPos = mul(float4(inPos, 1.0f), gScreenProj);

	outDiffuse = inDiffuse;
	outUV = inUV;
}
//EOF