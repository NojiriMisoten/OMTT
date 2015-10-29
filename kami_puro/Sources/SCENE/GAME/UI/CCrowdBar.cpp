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

// �g�̂�������
static const float BAR_FRAME_WIDTH = 940;
static const float BAR_FRAME_HEIGHT = 25;
// �o�[�Ƙg�̃I�t�Z�b�g
static const D3DXVECTOR2 BAR_FRAME_OFFSET = D3DXVECTOR2(0, 0);

// �ϋq�A�j���[�V������2D�̑傫��
static const float CROWD_WIDTH = 140;
static const float CROWD_HEIGHT = 60;
// �ϋq�A�j���[�V����2D�̃o�[����̃I�t�Z�b�g
static const float CROWD_OFFSET_Y = -40;
// �ϋq�A�j���[�V�����̊Ԋu
static const int CROWD_ANIME_INTERVAL = 30;
// �ϋq�A�j���[�V�����̂t�u�̈�i1 / �A�j�����j
static const float CROWD_ANIME_UV_ONE = 1.0f / 4.0f;

// �o�`�o�`�̑傫��
static const float SPARK_WIDTH = 50;
static const float SPARK_HEIGHT = 50;
// �o�`�o�`�̃o�[����̃I�t�Z�b�g
static const D3DXVECTOR2 SPARK_OFFSET = D3DXVECTOR2(0, 0);
// �o�`�o�`�̊g�k�Ԋu
static const int SPARK_ANIME_INTERVAL = 30;

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
	m_pFrame = NULL;
	m_pCrowd = NULL;
	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeEasingOneFrame = 0;
	m_AnimeEasingTimer = 0;
	m_CrowdAnimeCount = 0;
	m_pSpark = NULL;
	m_SparkRot = 0;
	m_SparkCount = 0;
	m_isSparkAdd = true;
	m_SparkPos = D3DXVECTOR3(0, 0, 0);
	m_AnimeOneFrameAlpha = 0;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);	// �ŏ��̃A�j���[�V�����œ�������n�܂邽��
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
		D3DXVECTOR3(left_center, posCenterY + BAR_FRAME_OFFSET.y, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(right_center, posCenterY + BAR_FRAME_OFFSET.y, 0),
		width, height,
		BAR_TEXTURE);

	m_pBarLeft->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_pBarRight->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	m_pBarLeft->SetColorPolygon(BAR_COLOR_LEFT);
	m_pBarRight->SetColorPolygon(BAR_COLOR_RIGHT);

	// �l�im_Value�j�P������̃s�N�Z����(float)���v�Z
	m_WidthOneValue = width / m_ValueMax;

	// ���S�̍��W
	m_PosCenterX = m_PosLeftX + width;
	m_PosCurrentX = m_PosCenterX;

	// �ŏ���UI�̊J�n�A�j���[�V���������邽�߃|���S���̈ʒu��ύX
	m_pBarLeft->SetVertexPolygonLeft(m_PosCenterX);
	m_pBarRight->SetVertexPolygonRight(m_PosCenterX);
	m_pBarLeft->SetVertexPolygonRight(m_PosCenterX);
	m_pBarRight->SetVertexPolygonLeft(m_PosCenterX);

	// �g
	m_pFrame = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, posCenterY + BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_CROWD_GAGE_FRAME);
	m_pFrame->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// �ϋq�����̊G
	m_pCrowd = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, posCenterY + CROWD_OFFSET_Y, 0),
		CROWD_WIDTH, CROWD_HEIGHT,
		TEXTURE_CROWD_GAGE_HUMAN);
	m_pCrowd->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// �ϋq�����̂t�u�l
	m_CrowdUV = UV_INDEX(0.0f, 0.25f, 0.0f, 1.0f);
	m_pCrowd->SetUV(&m_CrowdUV);

	// �o�`�o�`
	m_SparkPos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, posCenterY + SPARK_OFFSET.y, 0);
	m_pSpark = CScene2D::Create(m_pD3DDevice,
		m_SparkPos, SPARK_WIDTH, SPARK_HEIGHT,
		TEXTURE_CROWD_SPARK);
	m_pSpark->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_pSpark->SetColorPolygon(m_Anime2DColor);
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

	// �ϋq�����̃A�j���[�V�����X�V
	UpdateCrowdAnimation();

	// �o�[�𓮂����X�V
	UpdateBarMove();

	// �o�`�o�`�̃A�j���[�V����
	UpdateSparkAnimation();
}

//=============================================================================
// �ϋq�����̃A�j���[�V�����X�V
//=============================================================================
void CCrowdBar::UpdateCrowdAnimation(void)
{
	// �A�j���[�V�����J�E���g
	m_CrowdAnimeCount++;

	// �e�N�X�`����ς���^�C�~���O
	if (m_CrowdAnimeCount > CROWD_ANIME_INTERVAL)
	{
		// �J�E���g������
		m_CrowdAnimeCount = 0;
		
		// �t�u�ύX
		CrowdChangeUV();
	}

}

//=============================================================================
// �ϋq�����̃A�j���[�V�����̃e�N�X�`�����W��ύX����
//=============================================================================
void CCrowdBar::CrowdChangeUV(void)
{
	// �E�[�܂ōs���Ă��烋�[�v
	if (m_CrowdUV.right >= 1.0f)
	{
		m_CrowdUV.left = 0;
		m_CrowdUV.right = CROWD_ANIME_UV_ONE;
	}
	// ���͉E�ɃX�N���[��
	else
	{
		m_CrowdUV.left += CROWD_ANIME_UV_ONE;
		m_CrowdUV.right += CROWD_ANIME_UV_ONE;
	}

	// �t�u�Z�b�g
	m_pCrowd->SetUV(&m_CrowdUV);
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

		// �o�`�o�`�̈ړ�
		m_SparkPos.x = m_PosCurrentX;
		m_pSpark->SetVertexPolygon(m_SparkPos, SPARK_WIDTH, SPARK_HEIGHT);

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
// �o�`�o�`�̃A�j���[�V����
//=============================================================================
void CCrowdBar::UpdateSparkAnimation(void)
{
	// �p�x
	float num = m_SparkCount - static_cast<float>(SPARK_ANIME_INTERVAL)* 0.5f;
	float rot = num * 0.01f;
	m_pSpark->SetRot(D3DXVECTOR3(0, 0, rot));
	// �傫��
	
	// �傫������Ə�̊ϋq�̊G�Ƃ��Ԃ邵�ǂ����悤���ȁ[

	// ���Z�ƌ��Z���J��Ԃ�
	if (m_isSparkAdd)
	{
		m_SparkCount++;
		if (m_SparkCount > SPARK_ANIME_INTERVAL)
		{
			m_isSparkAdd = !m_isSparkAdd;
		}
	}
	else
	{
		m_SparkCount--;
		if (m_SparkCount <= 0)
		{
			m_isSparkAdd = !m_isSparkAdd;
		}
	}
}

//=============================================================================
// �`��
//=============================================================================
void CCrowdBar::DrawNormalRender(void)
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

	// �o�`�o�`�̏�����
	m_AnimeOneFrameAlpha = 1.0f / endCount;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);
	m_pSpark->SetColorPolygon(m_Anime2DColor);
}

//=============================================================================
// �J�n�A�j���[�V����������X�V
//=============================================================================
void CCrowdBar::UpdateAnime()
{
	if (m_AnimeEasingTimer < 1)
	{
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
	m_Anime2DColor.a += m_AnimeOneFrameAlpha;
	m_pSpark->SetColorPolygon(m_Anime2DColor);

	// �J�n�A�j���[�V�����I��
	if (m_AnimeCount > m_AnimeCountMax){
		m_isAnime = false;
		m_Anime2DColor.a = 1.0f;
		m_pSpark->SetColorPolygon(m_Anime2DColor);
	}
}

//=============================================================================
// �A�j���[�V�����̏�����������Ƃ��̃o�[��񏉊���
//=============================================================================
void CCrowdBar::Init()
{
	m_Value = 0;
	m_PosCurrentX = m_PosCenterX;
	m_SparkRot = 0;
	m_SparkCount = 0;
	m_isSparkAdd = true;
}
//----EOF----