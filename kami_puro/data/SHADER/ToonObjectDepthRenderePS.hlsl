//==========================================================
// 定数レジスタ CPU側から変更できる数少ないもの
//==========================================================

//**********************************************************
// エントリポイント
//**********************************************************
float4 main(float4 inColor : COLOR0, float4 inWPos : TEXCOORD1) :COLOR0
{
	float4 color = inColor;
	float4 pos = inWPos;

	// 深度値をfloat4つであらわす
	float bias = 1.f / 255.f;	// 誤差修整用
	color.r = pos.z / pos.w;	// 深度値を入れる
	
	int mask = color.r * 255;	// 整数部を取り出す（あとで小数部のみ、つかいたいので）
	color.g = color.r * 255.f - mask;
	
	mask = color.g * 255;
	color.b = color.g * 255.f - mask;
	
	mask = color.b * 255;
	color.a = color.b * 255.f - mask;
	
	color.r = color.r - color.g * bias;
	color.g = color.g - color.b * bias;
	color.b = color.b - color.a * bias;
	color.a = 1.f;
	return color;
}

//EOF