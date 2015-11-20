//=============================================================================
//
// CCutIn�N���X [CCutIn.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCutIn.h"
#include "CCutIn2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �J�b�g�C���̑傫��
static const float CUT_IN_WIDTH = SCREEN_WIDTH;
static const float CUT_IN_HEIGHT = 300.f;
static const float CUT_IN_MOVE_SPEED = 0.1f;

// �J�b�g�C�����Ƃ̃t�F�[�h�A�E�g����X�s�[�h
static const float FADE_SPEED[CUT_IN_MAX] = {
	0.08f,
	0.08f
};

// �J�b�g�C�����Ƃ̕\�����钷��
static const int CUT_IN_INTERVAL[CUT_IN_MAX] =
{
	30,
	30
};

// ��
static const int PLAYER_0 = 0;
static const int PLAYER_1 = 1;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCutIn::CCutIn(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;

	m_pCutInPolygon = NULL;
	m_pCutInPolygonBack = NULL;
	m_isIn = false;
	m_isOut = false;
	m_isWait = false;
	m_Time = 0;
	m_IntervalMax = 0;
	m_IntervalCount = 0;
	m_Color = D3DXCOLOR(1, 1, 1, 1);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCutIn::~CCutIn(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CCutIn::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	// 2D������
	m_pCutInPolygonBack = CCutIn2D::Create(m_pD3DDevice,
		D3DXVECTOR3(-CUT_IN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
		CUT_IN_WIDTH, CUT_IN_HEIGHT,
		TEXTURE_HP_GAGE_G);

	m_pCutInPolygon = CCutIn2D::Create(m_pD3DDevice,
		D3DXVECTOR3(-CUT_IN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
		CUT_IN_WIDTH, CUT_IN_HEIGHT,
		TEXTURE_CROWD_SPARK);
}

//=============================================================================
// �I��
//=============================================================================
void CCutIn::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CCutIn::Update(void)
{
	if (m_isIn)		In();
	if (m_isWait)	Wait();
	if (m_isOut)	Out();
}

//=============================================================================
// �`��
//=============================================================================
void CCutIn::DrawUI(void)
{
}

//=============================================================================
// �쐬
//=============================================================================
CCutIn* CCutIn::Create(
	LPDIRECT3DDEVICE9 *pDevice)
{
	CCutIn* p = new CCutIn(pDevice);
	p->Init(pDevice);
	return p;
}

//=============================================================================
// �J�b�g�C���X�^�[�g
//=============================================================================
void CCutIn::Start(int ID, CutInType type)
{
	m_isIn = true;
	m_isOut = false;
	m_isWait = false;
	m_IntervalCount = 0;
	m_Time = 0;
	m_CutInType = type;

	// �^�C�v���Ƃɏ�����
	if (type == CUT_IN_JIJII)
	{
		m_pCutInPolygon->ChangeTexture(TEXTURE_JIJII);
		m_pCutInPolygonBack->ChangeTexture(TEXTURE_HP_GAGE_G);
	}
	if (type == CUT_IN_SPARK)
	{
		m_pCutInPolygon->ChangeTexture(TEXTURE_CROWD_SPARK);
		m_pCutInPolygonBack->ChangeTexture(TEXTURE_HP_GAGE_R);
	}
	// �J�b�g�C����\�����钷��
	m_IntervalMax = CUT_IN_INTERVAL[type];

	// �v���C���[�ԍ����Ƃɍ��W��ݒ肷��
	if (ID == PLAYER_0)
	{
		m_Pos = -CUT_IN_WIDTH;
		m_PosDest = SCREEN_WIDTH * 0.5f;
	}
	else
	{
		m_Pos = CUT_IN_WIDTH;
		m_PosDest = SCREEN_WIDTH * 0.5f;
	}

	// �t�F�[�h�A�E�g�̔���������
	m_pCutInPolygon->InitWhite();
	m_pCutInPolygonBack->InitWhite();
	
}

//=============================================================================
// ��ʓ��ɓ����Ă���X�V
//=============================================================================
void CCutIn::In()
{
	// ���W�ړ�
	m_Time += CUT_IN_MOVE_SPEED;
	if (m_Time <= 1.0f)
	{
		float x = EasingInterpolation(m_Pos, m_PosDest, m_Time);
		m_pCutInPolygon->SetVertexPolygonX(x);
		m_pCutInPolygonBack->SetVertexPolygonX(x);
	}
	else
	{
		m_isIn = false;
		m_isWait = true;
	}
}

//=============================================================================
// �J�b�g�C�����ĕ\�����Ă�����
//=============================================================================
void CCutIn::Wait()
{
	// �t�F�[�h�A�E�g���I���t���O
	bool isEnd = false;

	// �J�b�g�C�����A�E�g����܂ł̃J�E���g
	m_IntervalCount++;
	if (m_IntervalCount > m_IntervalMax)
	{
		isEnd = m_pCutInPolygon->AddWhite(FADE_SPEED[m_CutInType]);
		m_pCutInPolygonBack->AddWhite(FADE_SPEED[m_CutInType]);
	}

	// �t�F�[�h�A�E�g���I�������
	if (isEnd)
	{
		m_isWait = false;
		m_isOut = true;
		m_Time = 0;
	}
}

//=============================================================================
// ��ʊO�ɏo�Ă����X�V
//=============================================================================
void CCutIn::Out()
{
//	m_pCutInPolygon->SetColorPolygon()

	m_Time += CUT_IN_MOVE_SPEED;
	if (m_Time <= 1.0f)
	{
	}
	else
	{
		m_isOut = false;
		m_pCutInPolygon->SetVertexPolygonX(-CUT_IN_WIDTH);
		m_pCutInPolygonBack->SetVertexPolygonX(-CUT_IN_WIDTH);
	}

}

//----EOF----