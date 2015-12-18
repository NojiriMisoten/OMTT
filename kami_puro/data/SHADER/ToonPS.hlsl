//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
sampler normalRendereTexSampler;	// ���ʂɏ��������
sampler normalVecTexSampler;		// �@�����o�������
float2 g_ScreenResolution;	// �𑜓x

//**********************************************************
// �G���g���|�C���g
//**********************************************************
float4 main(float4 diffuse : COLOR0, float2 uv : TEXCOORD0, float3 posW : TEXCOORD1) :COLOR0
{
	// �G�b�W����
	float2 pixelOffset = 1.f / g_ScreenResolution;

	// ����A�E���A�����A�E��
	// �F�擾
	float4 n1 = tex2D(normalVecTexSampler, uv + float2(-1.f, -1.f) * pixelOffset);
	float4 n2 = tex2D(normalVecTexSampler, uv + float2(1.f, 1.f) * pixelOffset);
	float4 n3 = tex2D(normalVecTexSampler, uv + float2(-1.f, 1.f) * pixelOffset);
	float4 n4 = tex2D(normalVecTexSampler, uv + float2(1.f, -1.f) * pixelOffset);
	n1.xyz = 2.0f * n1.xyz - float3(1.f, 1.f, 1.f);
	n2.xyz = 2.0f * n2.xyz - float3(1.f, 1.f, 1.f);
	n3.xyz = 2.0f * n3.xyz - float3(1.f, 1.f, 1.f);
	n4.xyz = 2.0f * n4.xyz - float3(1.f, 1.f, 1.f);

	// �������Z�o����
	float4 diagnalDelta = abs(n1 - n2) + abs(n3 - n4);

	// Z�����@
	float depthDelta = diagnalDelta.a;
	//								�@�@���������l�@�����x
	depthDelta = saturate((depthDelta - 20.9f) * 0.8f);
	
	// �@�������@
	float normalDelta = dot(diagnalDelta.rgb, 1);
	normalDelta = saturate((normalDelta - 20.98f) * 0.8f);
	
	// Z����+�@������
	float edgeAmount = saturate(depthDelta + normalDelta) * 1.0f;

	return (float4((float3)(1.f - edgeAmount), 1.0f)) * tex2D(normalRendereTexSampler, uv);	// �e�N�X�`��;
}

//-----EOF-----