//=============================================================================
//
// CStaminaBarBar�N���X [CStaminaBarBar.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCrowdBar.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �o�[�̃e�N�X�`��
static const TEXTURE_TYPE BAR_TEXTURE = TEXTURE_MONO;
// TODO ���̗�
static const float CROWD_MAX = 255;
// �o�[�̐F
static const D3DXCOLOR BAR_COLOR_LEFT = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
static const D3DXCOLOR BAR_COLOR_RIGHT = D3DXCOLOR(0.0f, 0.1f, 1.0f, 1.0f);
// �o�[�̈ړ������x�����l
static const float BAR_VEL_BASE = 1.0f;
// �o�[�̈ړ������x
static const float BAR_VEL_ACCE = 1.1f;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCrowdBar::CCrowdBar(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;
	m_PosLeftX = 0;
	m_PosRightX = 0;
	m_PosCenterX = 0;
	m_PosDestX = 0;
	m_PosCurrentX = 0;
	m_PosVel = 0;
	m_isPosMove = 0;
	m_Value = 0;
	m_ValueMax = 0;
	m_WidthOneValue = 0;
	m_pBarLeft = NULL;
	m_pBarRight = NULL;

	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeEasingOneFrame = 0;
	m_AnimeEasingTimer = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCrowdBar::~CCrowdBar(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CCrowdBar::Init(
	float height,
	float posCenterY,
	float posLeft,
	float posRight,
	LPDIRECT3DDEVICE9 *pDevice)
{
	// �ϐ����
	m_Value = 0;
	m_ValueMax = CROWD_MAX;
	m_PosLeftX = posLeft;
	m_PosRightX = posRight;

	// ��̃o�[�̕� 2������0.5f
	float width = (m_PosRightX - m_PosLeftX) * 0.5f;

	// ���E�o�[�̒����̍��W ����������0.5f
	float left_center = m_PosLeftX + width * 0.5f;
	float right_center = m_PosRightX - width * 0.5f;

	// 2D������
	m_pBarLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(left_center, posCenterY, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(right_center, posCenterY, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBarRight->AddLinkList(CRenderer::TYPE_RENDER_UI);

	m_pBarLeft->SetColorPolygon(BAR_COLOR_LEFT);
	m_pBarRight->SetColorPolygon(BAR_COLOR_RIGHT);

	// �l�im_Value�j�P������̃s�N�Z����(float)���v�Z
	m_WidthOneValue = width / m_ValueMax;

	// ���S�̍��W
	m_PosCenterX = m_PosLeftX + width;
	m_PosCurrentX = m_PosCenterX;
}

//=============================================================================
// �I��
//=============================================================================
void CCrowdBar::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CCrowdBar::Update(void)
{
	CDebugProc::Print("�ϋq�l�@�@%f\n", m_Value);
	CDebugProc::Print("Cur X�@�@%f\n", m_PosCurrentX);
	CDebugProc::Print("Def X�@�@%f\n", m_PosDestX);

	// �J�n�A�j���[�V����
	if (m_isAnime){
		UpdateAnime();
		return;
	}

	// �o�[�𓮂����X�V
	UpdateBarMove();
}

//=============================================================================
// �o�[�𓮂����X�V
//=============================================================================
void CCrowdBar::UpdateBarMove(void)
{
	// ���ڂ̍��W���Z�b�g���鎞
	if (m_isPosMove)
	{
		m_PosVel *= BAR_VEL_ACCE;
		m_PosCurrentX += m_PosVel;

		m_pBarLeft->SetVertexPolygonRight(m_PosCurrentX);
		m_pBarRight->SetVertexPolygonLeft(m_PosCurrentX);

		// �����l�ɂȂ����狫�ڂ̍��W�Z�b�g�t���Ofalse
		if (m_PosVel < 0){
			if (m_PosCurrentX < m_PosDestX)
			{
				m_isPosMove = false;
				m_PosCurrentX = m_PosDestX;
				m_pBarLeft->SetVertexPolygonRight(m_PosCurrentX);
				m_pBarRight->SetVertexPolygonLeft(m_PosCurrentX);
			}
		}
		else
			if (m_PosCurrentX > m_PosDestX)
			{
			m_isPosMove = false;
			m_PosCurrentX = m_PosDestX;
			m_pBarLeft->SetVertexPolygonRight(m_PosCurrentX);
			m_pBarRight->SetVertexPolygonLeft(m_PosCurrentX);
			}
	}

}

//=============================================================================
// �`��
//=============================================================================
void CCrowdBar::DrawUI(void)
{
}

//=============================================================================
// �쐬
//=============================================================================
CCrowdBar* CCrowdBar::Create(
	float height,
	float posCenterY,
	float posLeft,
	float posRight,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CCrowdBar* p = new CCrowdBar(pDevice);
	p->Init(height, posCenterY, posLeft, posRight, pDevice);
	return p;
}

//=============================================================================
// �l�ϓ�
// �����A�������y������ɂ̓t���O�Ǘ��Œ��_�ύX����񐔂����点��
//=============================================================================
void CCrowdBar::Add(float value)
{
	m_Value += value;

	// �N�����v
	m_Value = min(m_Value, m_ValueMax);
	m_Value = max(m_Value, -m_ValueMax);

	m_isPosMove = true;
	m_PosDestX = m_Value * m_WidthOneValue + m_PosCenterX;
	
	// �ŏ��̋��ڍ��W�̊����ړ��ʐݒ�
	if (m_PosDestX < m_PosCurrentX)
		m_PosVel = -BAR_VEL_BASE;
	else
		m_PosVel = BAR_VEL_BASE;
}

//=============================================================================
// �J�n�A�j���[�V����������@������
// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
//=============================================================================
void CCrowdBar::StartAnimation(int endCount)
{
	assert(endCount > 0 && "endCount�̓}�C�i�X�̒l����Ȃ��ŁI");

	// �A�j���[�V�������I������t���[����
	m_AnimeCountMax = endCount;

	// �A�j���[�V�������邽�߂̕ϐ�������
	m_AnimeCount = 0;
	m_isAnime = true;
	m_AnimeEasingOneFrame = 1.0f / static_cast<float>(endCount);
	m_AnimeEasingTimer = 0;

	// �o�[������
	m_pBarLeft->SetVertexPolygonLeft(m_PosCenterX);
	m_pBarRight->SetVertexPolygonRight(m_PosCenterX);

	m_pBarLeft->SetVertexPolygonRight(m_PosCenterX);
	m_pBarRight->SetVertexPolygonLeft(m_PosCenterX);

	// �o�[�̊�{�̏�񏉊���
	Init();
}

//=============================================================================
// �J�n�A�j���[�V����������X�V
//=============================================================================
void CCrowdBar::UpdateAnime()
{
	if (m_AnimeEasingTimer < 1){
		float leftX = EasingInterpolation(
			m_PosCenterX,
			m_PosLeftX,
			m_AnimeEasingTimer);
		float rightX = EasingInterpolation(
			m_PosCenterX,
			m_PosRightX,
			m_AnimeEasingTimer);

		// ��Ԃ̃^�C�}���X�V
		m_AnimeEasingTimer += m_AnimeEasingOneFrame;

		// �J�����g�̒l���Z�b�g
		m_pBarLeft->SetVertexPolygonLeft(leftX);
		m_pBarRight->SetVertexPolygonRight(rightX);
	}

	// �J�E���g
	m_AnimeCount++;

	// �J�n�A�j���[�V�����I��
	if (m_AnimeCount > m_AnimeCountMax){
		m_isAnime = false;
	}
}

//=============================================================================
// �A�j���[�V�����̏�����������Ƃ��̃o�[��񏉊���
//=============================================================================
void CCrowdBar::Init()
{
	m_Value = 0;
	m_PosCurrentX = m_PosCenterX;
}
//----EOF----