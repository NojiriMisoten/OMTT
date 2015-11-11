//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================

//**********************************************************
// �G���g���|�C���g
//**********************************************************
float4 main(float4 inColor : COLOR0, float4 inWPos : TEXCOORD1) :COLOR0
{
	float4 color = inColor;
	float4 pos = inWPos;

	// �[�x�l��float4�ł���킷
	float bias = 1.f / 255.f;	// �덷�C���p
	color.r = pos.z / pos.w;	// �[�x�l������
	
	int mask = color.r * 255;	// �����������o���i���Ƃŏ������̂݁A���������̂Łj
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