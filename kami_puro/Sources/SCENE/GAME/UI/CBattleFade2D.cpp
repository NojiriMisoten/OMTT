//=============================================================================
//
// CBattleFade2D�N���X [CBattleFade2D.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CBattleFade2D.h"
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// �萔
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBattleFade2D::CBattleFade2D(LPDIRECT3DDEVICE9 *pDevice) : CScene2D(pDevice)
{
	m_pD3DDevice = pDevice;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBattleFade2D::~CBattleFade2D(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CBattleFade2D::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	CScene2D::Init(pos, width, height, texType);
}

//=============================================================================
// �I��
//=============================================================================
void CBattleFade2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CBattleFade2D::Update(void)
{

}

//=============================================================================
// �`��
//=============================================================================
void CBattleFade2D::DrawUIBattleFadeRender(void)
{
	// ���Z����
	(*m_pD3DDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);							// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	// �|���S���̕`��
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// �v���~�e�B�u�̎��
		, 0											// �`����J�n���钸�_�ԍ�
		, DRAW_SQUARE_PRINITIV_NUM);				// ���������|���S����

	(*m_pD3DDevice)->SetTexture(0, NULL);

	// ���Z���������ɖ߂�
	(*m_pD3DDevice)->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// �쐬
//=============================================================================
CBattleFade2D* CBattleFade2D::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType)
{
	CBattleFade2D* p = new CBattleFade2D(pDevice);
	p->Init(pos, width, height, texType);
	p->AddLinkList(CRenderer::TYPE_RENDER_UI_BATTLE_FADE);

	return p;
}

//----EOF----