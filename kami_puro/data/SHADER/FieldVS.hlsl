//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
float4x4 gWVP;
float4x4 gLightWVP;
//**********************************************************
// �G���g���|�C���g
//**********************************************************
void main(in float3 inPos:POSITION0			// ���� FVF���ɍ��킹��
		, in float3 inNormal : NORMAL0		// ���� �@��
		, in float4 inDiffuse : COLOR0		// ���� FVF���ɍ��킹��	FVF_VERTEX_2D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
		, in float2 inUV : TEXCOORD0		// ���� FVF���ɍ��킹��
		, out float4 outPos : POSITION0		// �Ԃ��l �K��float4��POSITON��Ԃ��Ȃ���΂Ȃ�Ȃ� �V�X�e��(GPU)�ɍs�� �s�N�Z���V�F�[�_�[�ɓn�������Ƃ���TEXCOORD~�����R�Ɏg����̂ł�����g��
		, out float4 outDiffuse : COLOR0	// �Ԃ��l �s�N�Z���V�F�[�_�[�ɐ��`��Ԃ���ē͂�
		, out float2 outUV : TEXCOORD0		// �Ԃ��l �s�N�Z���V�F�[�_�[�ɐ��`��Ԃ���ē͂�
		, out float4 outWPos : TEXCOORD1
		)
{
	outDiffuse = inDiffuse;
	outDiffuse.a = 1.0f;

	outPos = mul(float4(inPos, 1.0f), gWVP);

	outUV = inUV;
	outWPos = mul(float4(inPos, 1.0f), gLightWVP);
}
//EOF