//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================

//**********************************************************
// �G���g���|�C���g
//**********************************************************
float4 main(float4 inColor : COLOR0, float4 inWPos : TEXCOORD1) :COLOR0
{
	inColor.a = inWPos.z / inWPos.w;		// �����ɐ[�x�l������
	return inColor;
}

//EOF