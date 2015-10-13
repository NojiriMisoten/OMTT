//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
float4x4 gView;			// �r���[
float4x4 gProj;			// �v���W�F�N�V����
float4x4 gWorld[52];	// ���[���h�z��
float3 gCameraDir;		// �J�����̃x�N�g��
float4x4 gLightVP;
//**********************************************************
// �G���g���|�C���g
//**********************************************************
void main(in float3 inPos:POSITION0		// ���� FVF���ɍ��킹��
	, in float3 inBlendWeight : BLENDWEIGHT0
	, in float4 inBlendIndex : BLENDINDICES
	, in float3 inNormal : NORMAL0		// ���� �@��
	, in float2 inUV : TEXCOORD0		// ���� FVF���ɍ��킹��
	, out float4 outPos : POSITION0		// �Ԃ��l �K��float4��POSITON��Ԃ��Ȃ���΂Ȃ�Ȃ� �V�X�e��(GPU)�ɍs�� �s�N�Z���V�F�[�_�[�ɓn�������Ƃ���TEXCOORD~�����R�Ɏg����̂ł�����g��
	, out float2 outUV : TEXCOORD0		// �Ԃ��l �s�N�Z���V�F�[�_�[�ɐ��`��Ԃ���ē͂�
	, out float2 outToonUV : TEXCOORD1
	, out float4 outWLightPos : TEXCOORD2
	, out float4 outWVPPos : TEXCOORD3
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
	float4x4 world = weight0 * world0
					+ weight1 * world1
					+ weight2 * world2
					+ weight3 * world3;
	float4x4 wvp = mul(world ,gView);
	wvp = mul(wvp, gProj);

	outPos = mul(float4(inPos, 1.0f), wvp);
	outWVPPos = mul(float4(inPos, 1.0f), wvp);

	float u = 0.5f;
	float v = 0.1 - dot(gCameraDir, inNormal) * 0.2f;
	float2 uv = float2(u, v);
	outToonUV = uv;
	outUV = inUV;

	float4x4 lightWVP = world * gLightVP;
	outWLightPos = mul(float4(inPos, 1.0f), lightWVP);
}
//EOF