//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================
float4x4 gWVP;			// ワールドビュープロジェクション
float4x4 gWorld;		// ワールドマトリクス
float4x4 gView;			// ビューマトリクス

//**********************************************************
// エントリポイント
//**********************************************************
void main(in float3 inPos:POSITION0				// 引数 FVF等に合わせる
	, in float3 inNormal : NORMAL0		// 引数 法線
	, in float2 inUV : TEXCOORD0		// 引数 FVF等に合わせる
	, out float4 outPos : POSITION0		// 返す値 必ずfloat4のPOSITONを返さなければならない システム(GPU)に行く ピクセルシェーダーに渡したいときはTEXCOORD~が自由に使えるのでそれを使う
	, out float4 outColor : COLOR0
	, out float4 outWPos : TEXCOORD1
	)
{
	outPos = mul(float4(inPos, 1.0f), gWVP);

	outWPos = outPos;
	// View座標系に変換
	float4 vNormal = mul(float4(inNormal, 0.0f), gWorld);
	//法線　-1～+1を0～1に
	vNormal.x = 0.5f * (vNormal.x + 1.f);
	vNormal.y = 0.5f * (vNormal.y + 1.f);
	vNormal.z = 0.5f * (vNormal.z + 1.f);
	vNormal.w = 1.f;
	
	outColor = vNormal;
	outColor.a = 1.f;
}
//EOF