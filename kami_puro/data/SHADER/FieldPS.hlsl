//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
sampler texSampler;
sampler depthSampler;

//**********************************************************
// �G���g���|�C���g
//**********************************************************
float4 main(float4 diffuse : COLOR0, float2 inUV : TEXCOORD0, float4 inWPos : TEXCOORD1) :COLOR0
{
	float4 color = tex2D(texSampler, inUV);
	float4 pos = inWPos;
	float depth = pos.z / pos.w;	// �[�x�l������

	// �e�N�X�`�����W�ɕϊ�
	float2 TransTexCoord;
	TransTexCoord.x = 0.5f + pos.x / pos.w * 0.5f;
	TransTexCoord.y = 0.5f - pos.y / pos.w * 0.5f;

	float4 depthTexColor = tex2D(depthSampler, TransTexCoord);
	float texDepth = depthTexColor.r
					+ depthTexColor.g * (1.f / 255.f)
					+ depthTexColor.b * (1.f / (255.f * 255.f));

	float s = depth > texDepth ? 0.2f : 1.0f;
	color.rgb *= s;

	return color;
}

//EOF