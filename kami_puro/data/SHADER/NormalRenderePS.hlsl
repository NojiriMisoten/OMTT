//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
sampler texSampler;
sampler toonSampler;
sampler depthSampler;
//**********************************************************
// �G���g���|�C���g
//**********************************************************
float4 main(float2 inUV : TEXCOORD0
			, float2 inToonUV : TEXCOORD1
			, float4 inWLightPos : TEXCOORD2
			, float4 inWVPPos : TEXCOORD3
			, float4 inColor : COLOR0
			) :COLOR0
{
	float4 color = tex2D(texSampler, inUV) * tex2D(toonSampler, inToonUV);

	float4 pos = inWLightPos;
	float depth = pos.z / pos.w;	// �[�x�l������

	// �e�N�X�`�����W�ɕϊ�
	float2 TransTexCoord;
	TransTexCoord.x = 0.5f + pos.x / pos.w * 0.5f;
	TransTexCoord.y = 0.5f - pos.y / pos.w * 0.5f;

	// �[�x�l���o��
	float4 depthTexColor = tex2D(depthSampler, TransTexCoord);
	float texDepth = depthTexColor.r
						+ depthTexColor.g * (1.f / 255.f)
						+ depthTexColor.b * (1.f / (255.f * 255.f));

	// �e���ǂ����̔��f
	float s = depth > texDepth + 0.0001f ? 0.2f : 1.0f;
	color.rgb *= s;
	color.a = 1.f;

	return color;
}

//EOF