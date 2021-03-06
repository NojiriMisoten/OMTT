//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float4x4 gWVP;
float4x4 gLightWVP;
//**********************************************************
// エントリポイント
//**********************************************************
void main(in float3 inPos:POSITION0			// 引数 FVF等に合わせる
		, in float3 inNormal : NORMAL0		// 引数 法線
		, in float4 inDiffuse : COLOR0		// 引数 FVF等に合わせる	FVF_VERTEX_2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
		, in float2 inUV : TEXCOORD0		// 引数 FVF等に合わせる
		, out float4 outPos : POSITION0		// 返す値 必ずfloat4のPOSITONを返さなければならない システム(GPU)に行く ピクセルシェーダーに渡したいときはTEXCOORD~が自由に使えるのでそれを使う
		, out float4 outDiffuse : COLOR0	// 返す値 ピクセルシェーダーに線形補間されて届く
		, out float2 outUV : TEXCOORD0		// 返す値 ピクセルシェーダーに線形補間されて届く
		, out float4 outWPos : TEXCOORD1
		)
{
	outDiffuse = inDiffuse;
	outDiffuse.a = 1.0f;

	outPos = mul(float4(inPos, 1.0f), gWVP);

	outUV = inUV;
	outWPos = mul(float4(inPos, 1.0f), gLightWVP);
}
//EOF