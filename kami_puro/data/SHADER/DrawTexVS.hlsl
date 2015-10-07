//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float4x4 gWVP;			// ワールドビュープロジェクション

//**********************************************************
// エントリポイント
//**********************************************************
void main(in float3 inPos:POSITION0		// 引数 FVF等に合わせる
	, in float3 inNormal : NORMAL0		// 引数 法線
	, in float2 inUV : TEXCOORD0		// 引数 FVF等に合わせる
	, out float4 outPos : POSITION0		// 返す値 必ずfloat4のPOSITONを返さなければならない システム(GPU)に行く ピクセルシェーダーに渡したいときはTEXCOORD~が自由に使えるのでそれを使う
	, out float2 outUV : TEXCOORD0
	)
{
	outPos = mul(float4(inPos, 1.0f), gWVP);
	outUV = inUV;
}
//EOF