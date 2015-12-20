//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
sampler texSampler;
float3 gMonochromeScale;
float gAlpha;
//**********************************************************
// �G���g���|�C���g
//**********************************************************
float4 main(float2 uv : TEXCOORD0) :COLOR0
{
	float4 color = tex2D(texSampler, uv);
	float grayColor = dot(color.rgb, gMonochromeScale);
	color *= float4(grayColor, grayColor, grayColor, gAlpha);
	return color;
}

//EOF