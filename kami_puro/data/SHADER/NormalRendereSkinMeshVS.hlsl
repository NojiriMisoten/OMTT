//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
float4x4 gView;			// �r���[
float4x4 gProj;			// �v���W�F�N�V����
float4x4 gWorld[52];	// ���[���h�z��
float3 gCameraDir;		// �J�����̃x�N�g��
float4x4 gLightView;
float4x4 gLightProj;
int gBlendNum;
//**********************************************************
// �G���g���|�C���g
//**********************************************************
void main(in float3 inPos:POSITION0		// ���� FVF���ɍ��킹��
	, in float3 inBlendWeight : BLENDWEIGHT0
	, in float4 inBlendIndex : BLENDINDICES0
	, in float3 inNormal : NORMAL0		// ���� �@��
	, in float2 inUV : TEXCOORD0		// ���� FVF���ɍ��킹��
	, in float4 inColor : COLOR0		// ���� FVF���ɍ��킹��
	, out float4 outPos : POSITION0		// �Ԃ��l �K��float4��POSITON��Ԃ��Ȃ���΂Ȃ�Ȃ� �V�X�e��(GPU)�ɍs�� �s�N�Z���V�F�[�_�[�ɓn�������Ƃ���TEXCOORD~�����R�Ɏg����̂ł�����g��
	, out float2 outUV : TEXCOORD0		// �Ԃ��l �s�N�Z���V�F�[�_�[�ɐ��`��Ԃ���ē͂�
	, out float2 outToonUV : TEXCOORD1
	, out float4 outWLightPos : TEXCOORD2
	, out float4 outWVPPos : TEXCOORD3
	, out float4 outColor : COLOR0
	)
{
	int index[4] = (int[4])inBlendIndex;

	float weight[4] = (float[4])0.f;
	weight[0] = inBlendWeight.x;
	weight[1] = inBlendWeight.y;
	weight[2] = inBlendWeight.z;
	weight[3] = 1.0f - weight[0] - weight[1] - weight[2];

	float4x4 tempWorld[4] = (float4x4[4])0.f;
	tempWorld[0] = gWorld[index[0]];
	tempWorld[1] = gWorld[index[1]];
	tempWorld[2] = gWorld[index[2]];
	tempWorld[3] = gWorld[index[3]];

	// �d�݂��݃��[���h�s��
	float LastBlendWeight = 0.0f;			// �Ō�̍s��Ɋ|������d��
	float4x4 matCombWorld = 0.0f;			// �������[���h�ϊ��s��

	if (gBlendNum > 1)
	{
		// ���[���h�ϊ��s����u�����h
		for (int i = 0; i < gBlendNum - 1; i++)
		{
			LastBlendWeight += weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
			matCombWorld += tempWorld[i] * weight[i];
		}

		// �Ō�̏d�݂𑫂��Z
		matCombWorld += gWorld[gBlendNum - 1] * (1.0f - LastBlendWeight);
	}
	else
	{
		matCombWorld += gWorld[index[0]];
	}

	float4x4 world = matCombWorld;
	float4x4 wvp = mul(world ,gView);
	wvp = mul(wvp, gProj);

	outPos = mul(float4(inPos, 1.0f), wvp);
	outWVPPos = mul(float4(inPos, 1.0f), wvp);

	float u = 0.5f;
	float v = 0.1f - dot(gCameraDir, inNormal) * 0.2f;
	v = max(v, 0.f);
	float2 uv = float2(u, v);
	outToonUV = uv;
	outUV = inUV;

	float4x4 lightWVP = mul(world, gLightView);
	lightWVP = mul(lightWVP, gLightProj);
	outWLightPos = mul(float4(inPos, 1.0f), lightWVP);

	outColor = float4(1,0,0,1);
}
//EOF