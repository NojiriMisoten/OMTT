//=============================================================================
//
// CForcusLine�N���X [CForcusLine.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CForcusLine.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �e�N�X�`���A�j���\�V�����̃X�s�[�h
static const int ANIME_SPEED = 2;
// �e�N�X�`���̈�R�}��UV�l
static const float ANIME_ONE_WIDTH = 1.0f / 4.0f;
// �t�F�[�h�̃X�s�[�h
static const float FADE_SPEED = 0.2f;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CForcusLine::CForcusLine(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;

	m_pPolygon = NULL;
	m_pBack = NULL;
	m_isIn = false;
	m_isOut = false;
	m_isWait = false;
	m_IntervalMax = 0;
	m_IntervalCount = 0;
	m_Color = D3DXCOLOR(1, 1, 1, 0);
	m_ColorBack = D3DXCOLOR(1, 1, 1, 0);
	m_BackUV = UV_INDEX(0, 1, 0, 1);
	m_TextureCount = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CForcusLine::~CForcusLine(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CForcusLine::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_FORCUS_LINE_BACK);
	m_pBack->SetColorPolygon(m_ColorBack);
	m_pBack->AddLinkList(CRenderer::TYPE_RENDER_UI);

	m_pPolygon = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_FORCUS_LINE);
	m_pPolygon->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pPolygon->SetColorPolygon(m_Color);
	m_BackUV.left = 0.0f;
	m_BackUV.right = ANIME_ONE_WIDTH;
	m_pPolygon->SetUV(&m_BackUV);
}

//=============================================================================
// �I��
//=============================================================================
void CForcusLine::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CForcusLine::Update(void)
{
	if (m_isIn)		In();
	if (m_isWait)	Wait();
	if (m_isOut)	Out();
}

//=============================================================================
// �`��
//=============================================================================
void CForcusLine::DrawUI(void)
{
}

//=============================================================================
// �쐬
//=============================================================================
CForcusLine* CForcusLine::Create(
	LPDIRECT3DDEVICE9 *pDevice)
{
	CForcusLine* p = new CForcusLine(pDevice);
	p->Init(pDevice);
	return p;
}

//=============================================================================
// �J�b�g�C���X�^�[�g
//=============================================================================
void CForcusLine::Start(int interval, bool isFinish)
{
	m_isIn = true;
	m_isOut = false;
	m_isWait = false;
	m_IntervalCount = 0;

	// �J�b�g�C����\�����钷��
	m_IntervalMax = interval;

	m_Color.a = 0.0f;
	m_pPolygon->SetColorPolygon(m_Color);
	m_ColorBack.a = 0.0f;
	m_pBack->SetColorPolygon(m_ColorBack);

	if (!isFinish)
		m_pPolygon->ChangeTexture(TEXTURE_FORCUS_LINE);
	else
		m_pPolygon->ChangeTexture(TEXTURE_FORCUS_LINE_RED);
}

//=============================================================================
// ��ʓ��ɓ����Ă���X�V
//=============================================================================
void CForcusLine::In()
{
	m_ColorBack.a += FADE_SPEED;
	m_Color.a += FADE_SPEED;
	if (m_Color.a > 1.0f){
		m_isIn = false;
		m_isWait = true;
		m_Color.a = 1.0f;
		m_ColorBack.a = 1.0f;
	}
	m_pPolygon->SetColorPolygon(m_Color);
	m_pBack->SetColorPolygon(m_ColorBack);
}

//=============================================================================
// �J�b�g�C�����ĕ\�����Ă�����
//=============================================================================
void CForcusLine::Wait()
{
	// �J�b�g�C�����A�E�g����܂ł̃J�E���g
	m_IntervalCount++;
	if (m_IntervalCount > m_IntervalMax)
	{
		m_isWait = false;
		m_isOut = true;
	}

	// �A�j���[�V����
	m_TextureCount++;
	if (m_TextureCount > ANIME_SPEED){
		m_TextureCount = 0;

		m_BackUV.left += ANIME_ONE_WIDTH;
		m_BackUV.right += ANIME_ONE_WIDTH;
		if (m_BackUV.left >= 1.0f){
			m_BackUV.left = 0.0f;
			m_BackUV.right = ANIME_ONE_WIDTH;
		}
		m_pPolygon->SetUV(&m_BackUV);
	}
}

//=============================================================================
// ��ʊO�ɏo�Ă����X�V
//=============================================================================
void CForcusLine::Out()
{
	m_Color.a -= FADE_SPEED;
	m_ColorBack.a -= FADE_SPEED;
	if (m_Color.a < 0.0f){
		m_isOut = false;
		m_Color.a = 0.0f;
		m_ColorBack.a = 0.0f;
	}
	m_pPolygon->SetColorPolygon(m_Color);
	m_pBack->SetColorPolygon(m_ColorBack);
}

//----EOF----