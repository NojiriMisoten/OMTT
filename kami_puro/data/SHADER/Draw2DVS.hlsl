//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
float4x4 gWorld;			// ���[���h�}�g���N�X
float4x4 gScreenProj;		// �ˉe�s��

//**********************************************************
// �G���g���|�C���g
//**********************************************************
void main(in float3 inPos : POSITION		// ���� FVF���ɍ��킹��
	, in float4 inDiffuse : COLOR0		// ���� FVF���ɍ��킹��	FVF_VERTEX_2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	, in float2 inUV : TEXCOORD0		// ���� FVF���ɍ��킹��
	, in float3 inNormal : NORMAL0		// ���� �@��
	, out float4 outPos : POSITION		// �Ԃ��l �K��float4��POSITON��Ԃ��Ȃ���΂Ȃ�Ȃ� �V�X�e��(GPU)�ɍs�� �s�N�Z���V�F�[�_�[�ɓn�������Ƃ���TEXCOORD~�����R�Ɏg����̂ł�����g��
	, out float4 outDiffuse : COLOR0	// �Ԃ��l �s�N�Z���V�F�[�_�[�ɐ��`��Ԃ���ē͂�
	, out float2 outUV : TEXCOORD0		// �Ԃ��l �s�N�Z���V�F�[�_�[�ɐ��`��Ԃ���ē͂�
	, out float3 posW : TEXCOORD1)		// �Ԃ��l �s�N�Z���V�F�[�_�[�ɑ��郏�[���h���W
{
	// �s�N�Z���V�F�[�_�[�ɑ��郏�[���h���W
	posW = mul(float4(inPos, 1.0f), gWorld).xyz;

	// 2D�ϊ�
	outPos = mul(float4(inPos, 1.0f), gScreenProj);

	outDiffuse = inDiffuse;
	outUV = inUV;
}
//EOF