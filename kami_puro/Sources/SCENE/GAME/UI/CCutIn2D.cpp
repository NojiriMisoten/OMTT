//=============================================================================
//
// CCutIn2D2D�N���X [CCutIn2D2D.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCutIn2D.h"
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// �萔
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCutIn2D::CCutIn2D(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice)
{
	m_pD3DDevice = pDevice;
	m_WhiteColor = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCutIn2D::~CCutIn2D(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CCutIn2D::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	CScene2D::Init(pos, width, height, texType);
}

//=============================================================================
// �I��
//=============================================================================
void CCutIn2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CCutIn2D::Update(void)
{

}

//=============================================================================
// �`��
//=============================================================================
void CCutIn2D::DrawUIRender(void)
{
	// �s�N�Z���V�F�[�_�[
	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_FADE_WHITE);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_FADE_WHITE);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	// �e�N�X�`���̃C���f�b�N�X�擾
	UINT texSampler = (*_psc)->GetSamplerIndex("texSampler");

	// ��������
	(*_psc)->SetFloat(*m_pD3DDevice, "whiteColor", m_WhiteColor);

	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetTexture(texSampler, m_pD3DTexBuff);							// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	// �|���S���̕`��
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// �v���~�e�B�u�̎��
		, 0											// �`����J�n���钸�_�ԍ�
		, DRAW_SQUARE_PRINITIV_NUM);				// ���������|���S����

	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}

//=============================================================================
// �쐬
//=============================================================================
CCutIn2D* CCutIn2D::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	CCutIn2D* p = new CCutIn2D(pDevice);
	p->Init(pos, width, height, texType);
	p->AddLinkList(CRenderer::TYPE_RENDER_UI);

	return p;
}

//=============================================================================
// ���������̒���
// �P�ɂȂ�����true��Ԃ�
//=============================================================================
bool CCutIn2D::AddWhite(float f)
{
	m_WhiteColor += f;

	if (m_WhiteColor > 1.0f)
	{
		return true;
	}
	return false;
}
//----EOF----