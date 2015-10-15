//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
float4x4 gWVP;			// ���[���h�r���[�v���W�F�N�V����
float3 gCameraDir;		// �J�����̃x�N�g��
float4x4 gLightWVP;
//**********************************************************
// �G���g���|�C���g
//**********************************************************
void main(in float3 inPos:POSITION0		// ���� FVF���ɍ��킹��
	, in float3 inNormal : NORMAL0		// ���� �@��
	, in float2 inUV : TEXCOORD0		// ���� FVF���ɍ��킹��
	, out float4 outPos : POSITION0		// �Ԃ��l �K��float4��POSITON��Ԃ��Ȃ���΂Ȃ�Ȃ� �V�X�e��(GPU)�ɍs�� �s�N�Z���V�F�[�_�[�ɓn�������Ƃ���TEXCOORD~�����R�Ɏg����̂ł�����g��
	, out float2 outUV : TEXCOORD0		// �Ԃ��l �s�N�Z���V�F�[�_�[�ɐ��`��Ԃ���ē͂�
	, out float2 outToonUV : TEXCOORD1
	, out float4 outWLightPos : TEXCOORD2
	, out float4 outWVPPos : TEXCOORD3
	)
{
	outPos = mul(float4(inPos, 1.0f), gWVP);

	outUV = inUV;

	float u = 0.5f;
	float v = 0.1 - dot(gCameraDir, inNormal) * 0.2f;
	float2 uv = float2(u, v);

	outToonUV = uv;
	outWLightPos = mul(float4(inPos, 1.0f), gLightWVP);
	outWVPPos = mul(float4(inPos, 1.0f), gWVP);
}
//EOF