//=============================================================================
//
// CBattleFade�N���X [CBattleFade.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CBattleFade.h"
#include "CBattleFade2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �O�ɒu���Ă����ׂ̍��W
static const D3DXVECTOR3 OUT_RANGE = D3DXVECTOR3(SCREEN_WIDTH / 1280.f * -1000.f, SCREEN_HEIGHT / 720.f * -1000, 0);

// ���̃t�F�[�h
// ���C���̑傫��
static const float LIGHT_LINE_WIDTH = SCREEN_WIDTH;
static const float LIGHT_LINE_HEIGHT = SCREEN_HEIGHT * 64.f / 720.f;
// ���C���̈ʒu
static const D3DXVECTOR3 LIGHT_LINE_POS_A = D3DXVECTOR3(
														SCREEN_WIDTH * 0.5f
														, SCREEN_HEIGHT * 240.f / 720.f
														, 0);
static const D3DXVECTOR3 LIGHT_LINE_POS_B = D3DXVECTOR3(
														SCREEN_WIDTH * 0.5f
														, SCREEN_HEIGHT * 480.f / 720.f
														, 0);
// �Ђ��鋅�̑傫��
static const float LIGHT_BALL_WIDTH = SCREEN_WIDTH * 720.f / 1280.f;
static const float LIGHT_BALL_HEIGHT = SCREEN_HEIGHT;
static const float LIGHT_BALL_SMALL_WIDTH = SCREEN_WIDTH * 720 / 1280.f;
static const float LIGHT_BALL_SMALL_HEIGHT = SCREEN_HEIGHT;
// �e�̏������W
static const D3DXVECTOR3 LIGHT_BALL_POS_A = D3DXVECTOR3(
															-LIGHT_BALL_WIDTH * 0.5f
															, SCREEN_HEIGHT * 240.f / 720.f
															, 0);
static const D3DXVECTOR3 LIGHT_BALL_POS_B = D3DXVECTOR3(
															SCREEN_WIDTH + LIGHT_BALL_WIDTH * 0.5f
															, SCREEN_HEIGHT * 480.f / 720.f
															, 0);

// �҂���
static const int LIGHT_WAIT_INTERVAL = 20;

// ��{�̐F
static const D3DXCOLOR LIGHT_COLOR_BASE_A = D3DXCOLOR(0.0f, 0.1f, 1.0f, 1.0f);
static const D3DXCOLOR LIGHT_COLOR_BASE_B = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
// ��{�̐F�Ɣ��΂̐F�������Ė��邭
static const D3DXCOLOR LIGHT_COLOR_UNBASE_A = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
static const D3DXCOLOR LIGHT_COLOR_UNBASE_B = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
// �w�i�̐F
static const D3DXCOLOR LIGHT_COLOR_BACK = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

// �t�F�[�h�̃X�s�[�h
static const float LIGHT_FADE_SPEED = 0.1f;

// �e�̈ړ����x
static const float LIGHT_BALL_SPEED = SCREEN_WIDTH / 1280.f * 82.f;

// �^�C�g�����S�̑傫��
static const float LOGO_WIDTH = SCREEN_WIDTH * 750.f / 1280.f;
static const float LOGO_HEIGHT = SCREEN_HEIGHT * 250.f / 720.f;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBattleFade::CBattleFade(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DDevice = pDevice;

	m_isIn = false;
	m_isOut = false;
	m_isWait = false;
	m_Time = 0;
	m_WaitInterval = 0;
	m_Count = 0;
	m_ColorBack = LIGHT_COLOR_BACK;
	m_BattleFadeType = BATTLE_FADE_MAX;
	m_pLightBack = NULL;
	m_pLogo = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBattleFade::~CBattleFade(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CBattleFade::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	// 2D������
	m_pLightBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_DEFAULT);
	m_pLightBack->AddLinkList(CRenderer::TYPE_RENDER_UI_BATTLE_FADE);
	m_ColorBack = LIGHT_COLOR_BACK;
	m_ColorBack.a = 0;
	m_pLightBack->SetColorPolygon(m_ColorBack);
	
	// ��
	m_LightA.m_pLightBall = CBattleFade2D::Create(m_pD3DDevice,
		D3DXVECTOR3(OUT_RANGE),
		LIGHT_BALL_WIDTH, LIGHT_BALL_HEIGHT,
		TEXTURE_UI_FADE_BALL_LARGE);
	m_LightA.m_pLightBallSmall = CBattleFade2D::Create(m_pD3DDevice,
		D3DXVECTOR3(OUT_RANGE),
		LIGHT_BALL_SMALL_WIDTH, LIGHT_BALL_SMALL_HEIGHT,
		TEXTURE_UI_FADE_BALL_SMALL);
	m_LightA.m_pLightLine = CBattleFade2D::Create(m_pD3DDevice,
		D3DXVECTOR3(LIGHT_LINE_POS_A),
		LIGHT_LINE_WIDTH, LIGHT_LINE_HEIGHT,
		TEXTURE_UI_FADE_LINE);
	m_LightB.m_pLightBall = CBattleFade2D::Create(m_pD3DDevice,
		D3DXVECTOR3(OUT_RANGE),
		LIGHT_BALL_WIDTH, LIGHT_BALL_HEIGHT,
		TEXTURE_UI_FADE_BALL_LARGE);
	m_LightB.m_pLightBallSmall = CBattleFade2D::Create(m_pD3DDevice,
		D3DXVECTOR3(OUT_RANGE),
		LIGHT_BALL_SMALL_WIDTH, LIGHT_BALL_SMALL_HEIGHT,
		TEXTURE_UI_FADE_BALL_SMALL);
	m_LightB.m_pLightLine = CBattleFade2D::Create(m_pD3DDevice,
		D3DXVECTOR3(LIGHT_LINE_POS_B),
		LIGHT_LINE_WIDTH, LIGHT_LINE_HEIGHT,
		TEXTURE_UI_FADE_LINE);
	// �F����
	m_LightA.m_ColorBall = LIGHT_COLOR_BASE_A;
	m_LightA.m_ColorLine = LIGHT_COLOR_BASE_A;
	m_LightA.m_ColorBallSmall = LIGHT_COLOR_UNBASE_A;
	m_LightB.m_ColorBall = LIGHT_COLOR_BASE_B;
	m_LightB.m_ColorLine = LIGHT_COLOR_BASE_B;
	m_LightB.m_ColorBallSmall = LIGHT_COLOR_UNBASE_B;
	// �ŏ��̓A���t�@��0�ɂ��Ă���
	m_LightA.SetAlpha(0);
	m_LightB.SetAlpha(0);

	// �^�C�g�����S
	m_pLogo = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		LOGO_WIDTH, LOGO_HEIGHT,
		TEXTURE_LOGO_TITLE);
	m_pLogo->AddLinkList(CRenderer::TYPE_RENDER_UI_BATTLE_FADE);
	m_ColorLogo = D3DXCOLOR(1, 1, 1, 0);
	m_pLogo->SetColorPolygon(m_ColorLogo);
}

//=============================================================================
// �I��
//=============================================================================
void CBattleFade::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CBattleFade::Update(void)
{
	if (m_isIn)		In();
	if (m_isWait)	Wait();
	if (m_isOut)	Out();
}

//=============================================================================
// �`��
//=============================================================================
void CBattleFade::DrawUIBattleFadeRender(void)
{
}

//=============================================================================
// �쐬
//=============================================================================
CBattleFade* CBattleFade::Create(
	LPDIRECT3DDEVICE9 *pDevice)
{
	CBattleFade* p = new CBattleFade(pDevice);
	p->Init(pDevice);
	return p;
}

//=============================================================================
// �X�^�[�g
//=============================================================================
void CBattleFade::Start(BattleFade type)
{
	m_isIn = true;
	m_isOut = false;
	m_isWait = false;
	m_Count = 0;
	m_Time = 0;
	m_BattleFadeType = type;

	// �^�C�v���Ƃɏ�����
	if (type == BATTLE_FADE_LIGHT)
	{
		// In��Out�̊Ԃ̑҂���
		m_WaitInterval = LIGHT_WAIT_INTERVAL;
		// �F
		m_ColorLogo.a = 0;
		m_ColorBack.a = 0;
		m_pLightBack->SetColorPolygon(m_ColorBack);
		m_pLogo->SetColorPolygon(m_ColorLogo);
		m_LightA.SetAlpha(0);
		m_LightB.SetAlpha(0);
		// ���W
		m_LightA.m_Pos = LIGHT_BALL_POS_A;
		m_LightB.m_Pos = LIGHT_BALL_POS_B;
		m_LightA.SetPosPolygon();
		m_LightB.SetPosPolygon();
	}
}

//=============================================================================
// ��ʓ��ɓ����Ă���X�V
//=============================================================================
void CBattleFade::In()
{
	m_ColorLogo.a += LIGHT_FADE_SPEED;
	m_pLogo->SetColorPolygon(m_ColorLogo);
	m_ColorBack.a += LIGHT_FADE_SPEED;
	m_pLightBack->SetColorPolygon(m_ColorBack);
	m_LightA.AddAlpha(LIGHT_FADE_SPEED);
	m_LightB.AddAlpha(LIGHT_FADE_SPEED);

	if (m_ColorBack.a > 1.0f)
	{
		m_LightA.SetAlpha(1);
		m_LightB.SetAlpha(1);
		m_ColorBack.a = 1;
		m_pLightBack->SetColorPolygon(m_ColorBack);
		m_ColorLogo.a = 1;
		m_pLogo->SetColorPolygon(m_ColorLogo);
		m_isIn = false;
		m_isWait = true;
	}
}

//=============================================================================
// �J�b�g�C�����ĕ\�����Ă�����
//=============================================================================
void CBattleFade::Wait()
{
	m_Count++;

	if (m_Count > m_WaitInterval)
	{
		m_Count = 0;
		m_isWait = false;
		m_isOut = true;
	}

	m_LightA.m_Pos.x += LIGHT_BALL_SPEED;
	m_LightB.m_Pos.x -= LIGHT_BALL_SPEED;
	m_LightA.SetPosPolygon();
	m_LightB.SetPosPolygon();
}

//=============================================================================
// ��ʊO�ɏo�Ă����X�V
//=============================================================================
void CBattleFade::Out()
{
	m_ColorLogo.a -= LIGHT_FADE_SPEED;
	m_pLogo->SetColorPolygon(m_ColorLogo);
	m_ColorBack.a -= LIGHT_FADE_SPEED;
	m_pLightBack->SetColorPolygon(m_ColorBack);
	m_LightA.AddAlpha(-LIGHT_FADE_SPEED);
	m_LightB.AddAlpha(-LIGHT_FADE_SPEED);

	if (m_ColorBack.a < 0.0f)
	{
		m_ColorBack.a = 0;
		m_ColorLogo.a = 0;
		m_pLightBack->SetColorPolygon(m_ColorBack);
		m_pLogo->SetColorPolygon(m_ColorLogo);
		m_LightA.SetAlpha(0);
		m_LightB.SetAlpha(0);

		m_isOut = false;
	}

}

//----EOF----