//=============================================================================
//
// �Ԃ�����V�F�[�_�[ [redPS.hlsl]
// Author : �˖{�r�F
//
//=============================================================================


sampler g_Sampler;


//**********************************************************
// �G���g���|�C���g
//**********************************************************
float4 main(float2 uv : TEXCOORD0):COLOR0
{
	float4 outColor = tex2D(g_Sampler, uv);
	outColor.r = outColor.r * 2.0f;
	outColor.g = outColor.g * 0.5f;
	outColor.b = outColor.b * 0.5f;
	return outColor;
}

//EOF