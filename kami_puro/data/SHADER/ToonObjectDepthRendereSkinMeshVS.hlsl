//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
float4x4 gVP;			// �r���[�v���W�F�N�V����
float4x4 gWorld[52];		// ���[���h�}�g���N�X

//**********************************************************
// �G���g���|�C���g
//**********************************************************
void main(in float3 inPos:POSITION0				// ���� FVF���ɍ��킹��
	, in float3 inNormal : NORMAL0		// ���� �@��
	, in float2 inUV : TEXCOORD0		// ���� FVF���ɍ��킹��
	, in float3 inBlendWeight : BLENDWEIGHT0
	, in float4 inBlendIndex : BLENDINDICES
	, out float4 outPos : POSITION0		// �Ԃ��l �K��float4��POSITON��Ԃ��Ȃ���΂Ȃ�Ȃ� �V�X�e��(GPU)�ɍs�� �s�N�Z���V�F�[�_�[�ɓn�������Ƃ���TEXCOORD~�����R�Ɏg����̂ł�����g��
	, out float4 outColor : COLOR0
	, out float4 outWPos : TEXCOORD1
	)
{
	int index0, index1, index2, index3;
	float weight0, weight1, weight2, weight3;
	float4x4 world0, world1, world2, world3;

	index0 = inBlendIndex.x;
	index1 = inBlendIndex.y;
	index2 = inBlendIndex.z;
	index3 = inBlendIndex.w;

	weight0 = inBlendWeight.x;
	weight1 = inBlendWeight.y;
	weight2 = inBlendWeight.z;
	weight3 = 1.0f - weight0 - weight1 - weight2;

	world0 = gWorld[index0];
	world1 = gWorld[index1];
	world2 = gWorld[index2];
	world3 = gWorld[index3];

	// �d�݂��݃��[���h�s��
	float4x4 world = world0 * weight0
		+ world1 * weight1
		+ world2 * weight2
		+ world3 * weight3;
	float4x4 wvp = world * gVP;

	outPos = mul(float4(inPos, 1.0f), wvp);
	outWPos = outPos;
	outColor = float4(1, 1, 1, 1);
}
//EOF