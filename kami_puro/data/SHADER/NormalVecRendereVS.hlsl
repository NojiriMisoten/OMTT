//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
float4x4 gWVP;			// ���[���h�r���[�v���W�F�N�V����
float4x4 gWorld;		// ���[���h�}�g���N�X
float4x4 gView;			// �r���[�}�g���N�X

//**********************************************************
// �G���g���|�C���g
//**********************************************************
void main(in float3 inPos:POSITION0				// ���� FVF���ɍ��킹��
	, in float3 inNormal : NORMAL0		// ���� �@��
	, in float2 inUV : TEXCOORD0		// ���� FVF���ɍ��킹��
	, out float4 outPos : POSITION0		// �Ԃ��l �K��float4��POSITON��Ԃ��Ȃ���΂Ȃ�Ȃ� �V�X�e��(GPU)�ɍs�� �s�N�Z���V�F�[�_�[�ɓn�������Ƃ���TEXCOORD~�����R�Ɏg����̂ł�����g��
	, out float4 outColor : COLOR0
	, out float4 outWPos : TEXCOORD1
	)
{
	outPos = mul(float4(inPos, 1.0f), gWVP);

	outWPos = outPos;
	// View���W�n�ɕϊ�
	float4 vNormal = mul(float4(inNormal, 0.0f), gWorld);
	//�@���@-1�`+1��0�`1��
	vNormal.x = 0.5f * (vNormal.x + 1.f);
	vNormal.y = 0.5f * (vNormal.y + 1.f);
	vNormal.z = 0.5f * (vNormal.z + 1.f);
	vNormal.w = 1.f;
	
	outColor = vNormal;
	outColor.a = 1.f;
}
//EOF