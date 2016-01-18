//=============================================================================
//
// CHpBar�N���X [CHpBar.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CHpBar.h"
#include "../../../BASE_OBJECT/CScene2D.h"
#include "../PLAYER/CPlayer.h"
#include "CCutIn2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
const float CHpBar::JIJII_TEX_U = 1.0f / 4.0f;
const float CHpBar::JIJII_TEX_V = 1.0f / 2.0f;

// �ő�HP��
static const float HP_MAX = DEFAULT_HP_PARAMETER;

// HP�̊����ɂ���ĕ\������@���̒l
static const float HP_EXPRESSION[CHpBar::EXPRESSION_MAX] = {
	DEFAULT_HP_PARAMETER,
	DEFAULT_HP_PARAMETER * 0.6f,
	DEFAULT_HP_PARAMETER * 0.3f,
};

// �Ԃ��o�[��ύX����܂ł̃J�E���g��
static const short RED_CHANGE_INTERVAL = 40;
// �_���[�W���󂯂����o�[�����炷�܂ł̃t���[����(�o�[�̑��x)
static const float ERASE_INTERVAL_G = 20.0f;
static const float ERASE_INTERVAL_R = 20.0f;
// �_���[�W�����炷�Ƃ���1�t���[��������̗� ��Ԃ�time�̌v�Z�Ŏg��
static const float ERASE_ONE_FRAME_G = 1.0f / ERASE_INTERVAL_G;
static const float ERASE_ONE_FRAME_R = 1.0f / ERASE_INTERVAL_R;
// �k�킷�Ƃ��͈̔�
static const float SHAKE_RANGE = 10.0f;
// �k�킷�Ƃ��͈̔͂̒�R
static const float SHAKE_RANGE_RESIST = 0.8f;
// �k�킷�Ƃ��̎���
static const int SHAKE_INTERVAL = 15;
// �g�̂�������
static const float BAR_FRAME_WIDTH = SCREEN_WIDTH / 1280.f * 800.f * 0.8f;
static const float BAR_FRAME_HEIGHT = SCREEN_HEIGHT / 720.f * 200.f * 0.8f;
// �܂邢�ق��̘g�̂�������
static const float BAR_FRAME_CIRCLE_WIDTH = SCREEN_WIDTH / 1280.f * 160.f;
static const float BAR_FRAME_CIRCLE_HEIGHT = SCREEN_HEIGHT / 720.f * 160.f;
// �o�[�̍��W�ɑ΂��Ă̘g�̍��W��offset
static const D3DXVECTOR2 BAR_FRAME_OFFSET = D3DXVECTOR2(SCREEN_WIDTH / 1280.f * 49.f, SCREEN_HEIGHT / 720.f * 38.f);
// �o�[�̍��W�ɑ΂��Ă̂������̍��W��offset �o�[�̍��E�̒[���瑊�ΎQ��
static const D3DXVECTOR2 FACE_OFFSET = D3DXVECTOR2(SCREEN_WIDTH / 1280.f * 37.f, SCREEN_HEIGHT / 720.f * -38.f);
// �������̂�������
static const float FACE_WIDTH = SCREEN_WIDTH / 1280.f * 130.f;
static const float FACE_HEIGHT = SCREEN_HEIGHT / 720.f * 130.f;
// �������̔w�i�̐F�̓��߂���ŏ��̐F
static const D3DXCOLOR FACE_BACK_ALPHA_LEFT = D3DXCOLOR(1.0f, 0.1f, 0.0f, 0.0f);
static const D3DXCOLOR FACE_BACK_ALPHA_RIGHT = D3DXCOLOR(0.0f, 0.1f, 1.0f, 0.0f);
// �������̔w�i�̑傫��
static const float FACE_BACK_WIDTH = BAR_FRAME_CIRCLE_WIDTH - (SCREEN_WIDTH / 1280.f * 5.f);
static const float FACE_BACK_HEIGHT = BAR_FRAME_CIRCLE_HEIGHT - (SCREEN_HEIGHT / 720.f * 5.f);
// �_�ł̑���
static const float WHITE_SPEED = 0.1f;
// �_�ł��銄��
static const float WHITE_CHANGE_VALUE = HP_MAX * 0.3f;
// �΂̃t�F�[�h�X�s�[�h
static const float FIRE_FADE_SPEED = 0.1f;
// �΂̍��W�̃I�t�Z�b�g y�͌Œ� x�͔��]���ē�Ɏg����
static const D3DXVECTOR2 FIRE_OFFSET = D3DXVECTOR2(SCREEN_WIDTH / 1280.f * 40.f, SCREEN_HEIGHT / 720.f * 30.f);
// �΂̑傫��
static const float FIRE_BACK_WIDTH = SCREEN_WIDTH / 1280.f * 280.f;
static const float FIRE_BACK_HEIGHT = SCREEN_HEIGHT / 720.f * 280.f;
static const float FIRE_SIDE_WIDTH = SCREEN_WIDTH / 1280.f * 100.f;
static const float FIRE_SIDE_HEIGHT = SCREEN_HEIGHT / 720.f * 180.f;
// �΂̃e�N�X�`���̈�R�}���̑傫��
static const float FIRE_ONE_WIDTH = 1.0f / 10.0f;
// �΂̃e�N�X�`���A�j���[�V�����X�s�[�h
static const short FIRE_TEXTURE_SPEED = 4;
// �M���̃e�N�X�`���̃A�j���[�V��������R�}�̍ő�Ƃ�
static const short FIRE_TEXTURE_MAX = 7;
static const short FIRE_TEXTURE_MIN = 1;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CHpBar::CHpBar(LPDIRECT3DDEVICE9 *pDevice)
{
	// ��ԗp�̎��� 0���ƕ�Ԃ��n�܂邽��1����Ƃ�
	for (int i = 0; i < BAR_MAX; i++){
		m_pBar[i].m_p2D = NULL;
		m_pBar[i].m_PosEasingEnd = 0;
		m_pBar[i].m_PosEasingStart = 0;
		m_pBar[i].m_PosLeft = 0;
		m_pBar[i].m_PosRight = 0;
		m_pBar[i].m_TimerEasing = 1;
		m_pBar[i].m_Value = 0;
	}
	m_ValueMax = 0;
	m_WidthOneValue = 0;
	m_RedResetCountLeft = 0;
	m_RedResetCountRight = 0;
	m_isRedResetLeft = false;
	m_isRedResetRight = false;
	m_isRedEasingLeft = false;
	m_isRedEasingRight = false;

	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeEasingOneFrame = 0;
	m_AnimeTimerEasing = 0;

	m_pFrameLeft = NULL;
	m_pFrameRight = NULL;
	m_pFrameLeftBar = NULL;
	m_pFrameRightBar = NULL;

	m_isShakeLeft = false;
	m_isShakeRight = false;
	m_ShakeCountLeft = 0;
	m_ShakeCountRight = 0;
	m_PosCenterY = 0;
	m_ShakePosYLeft = 0;
	m_ShakePosYRight = 0;
	m_ShakeRangeLeft = 0;
	m_ShakeRangeRight = 0;

	m_FaceLeft.m_Expression = EXPRESSION_GOOD;
	m_FaceLeft.m_pFace2D = NULL;
	m_FaceLeft.m_Pos = D3DXVECTOR2(0, 0);
	m_FaceLeft.m_UV = UV_INDEX(0, JIJII_TEX_U, 0, JIJII_TEX_V);
	m_FaceRight.m_Expression = EXPRESSION_GOOD;
	m_FaceRight.m_pFace2D = NULL;
	m_FaceRight.m_Pos = D3DXVECTOR2(0, 0);
	m_FaceRight.m_UV = UV_INDEX(0, JIJII_TEX_U, JIJII_TEX_V, 1);
	m_AnimeCount = 0;
	m_AnimeCountMax = 0;
	m_isAnime = false;
	m_AnimeOneFrameAlpha = 0;
	// �ŏ��̃A�j���[�V�����œ�������n�܂邽��
	m_Anime2DColor = D3DXCOLOR(1, 1, 1,0);
	m_Anime2DColorJijiiLeft = FACE_BACK_ALPHA_LEFT;
	m_Anime2DColorJijiiRight = FACE_BACK_ALPHA_RIGHT;
	m_pD3DDevice = pDevice;
	m_isChangeWhiteLeft = false;
	m_isChangeWhiteRight = false;
	m_isAddWhiteLeft = true;
	m_isAddWhiteRight = true;

	m_pFireLeftBack = NULL;
	m_pFireRightBack = NULL;
	m_FireColorRight = D3DXCOLOR(1, 1, 1, 0);
	m_FireColorLeft = D3DXCOLOR(1, 1, 1, 0);
	m_FireTextureLeft = 0;
	m_FireTextureRight = 0;
	m_FireTextureCountLeft = 0;
	m_FireTextureCountRight = 0;
	m_isFireLeft = false;
	m_isFireRight = false;
	m_isFireAppearLeft = false;
	m_isFireAppearRight = false;
	m_isFireDisappearLeft = false;
	m_isFireDisappearRight = false;

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CHpBar::~CHpBar(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CHpBar::Init(
	float height,
	float posCenterY,
	float posLeftBarLeftX,
	float posLeftBarRightX,
	float posRightBarLeftX,
	float posRightBarRightX)
{
	// �ϐ����
	m_ValueMax = HP_MAX;
	m_AnimeTimerEasing = 0;

	m_pBar[BAR_GREEN_L].m_PosLeft = posLeftBarLeftX;
	m_pBar[BAR_GREEN_L].m_PosRight = posLeftBarRightX;
	m_pBar[BAR_GREEN_R].m_PosLeft = posRightBarLeftX;
	m_pBar[BAR_GREEN_R].m_PosRight = posRightBarRightX;
	m_pBar[BAR_RED_L].m_PosLeft = posLeftBarLeftX;
	m_pBar[BAR_RED_L].m_PosRight = posLeftBarRightX;
	m_pBar[BAR_RED_R].m_PosLeft = posRightBarLeftX;
	m_pBar[BAR_RED_R].m_PosRight = posRightBarRightX;

	// ������
	Init();
	// �o�[�̕�
	float barWidth = posLeftBarRightX - posLeftBarLeftX;
	// �l�im_Value�j�P������̃s�N�Z����(float)���v�Z
	m_WidthOneValue = barWidth / m_ValueMax;
	// ���S���W
	m_PosCenterY = posCenterY;

	// �o�[�̍��W
	D3DXVECTOR3 pos[BAR_MAX] = {
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
	};

	for (int i = 0; i < BAR_MAX; i++)
	{
		// �o�[��2D�̍쐬
		m_pBar[i].m_p2D = CCutIn2D::Create(m_pD3DDevice,
			pos[i],
			barWidth, height,
			i < 2 ? TEXTURE_HP_GAGE_R : TEXTURE_HP_GAGE_G);
		// �o�[�̕ϐ�
		m_pBar[i].m_Value = m_ValueMax;
		m_pBar[i].m_TimerEasing = 1;
	}

	// �ŏ���UI�J�n�A�j���[�V���������邩��A�|���S�����Z�b�g������
	// �o�[��Start�ʒu�ɓ�����
	m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_GREEN_L].m_PosRight);
	m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_GREEN_R].m_PosLeft);
	m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_RED_L].m_PosRight);
	m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_RED_R].m_PosLeft);

	// ��̍��W
	D3DXVECTOR3 facePosL = D3DXVECTOR3(posLeftBarLeftX - FACE_OFFSET.x, m_PosCenterY + FACE_OFFSET.y, 0);
	D3DXVECTOR3 facePosR = D3DXVECTOR3(posRightBarRightX + FACE_OFFSET.x, m_PosCenterY + FACE_OFFSET.y, 0);

	// �g
	m_pFrameLeftBar = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[0].x - BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_BAR);
	m_pFrameRightBar = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[1].x + BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_BAR);
	// ���]
	m_pFrameRightBar->SetUVMirror();

	// �ۂ��g
	m_pFrameLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		BAR_FRAME_CIRCLE_WIDTH, BAR_FRAME_CIRCLE_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_CIRCLE);
	m_pFrameRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		BAR_FRAME_CIRCLE_WIDTH, BAR_FRAME_CIRCLE_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_CIRCLE);

	// �琶��
	m_FaceLeft.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		FACE_WIDTH, FACE_HEIGHT, TEXTURE_JIJII);
	// �E������
	m_FaceRight.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		FACE_WIDTH, FACE_HEIGHT, TEXTURE_JIJII);
	// �������̔w�i
	m_FaceLeft.m_pBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		FACE_BACK_WIDTH, FACE_BACK_HEIGHT, TEXTURE_JIJII_BACK);
	// �E������
	m_FaceRight.m_pBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		FACE_BACK_WIDTH, FACE_BACK_HEIGHT, TEXTURE_JIJII_BACK);
	// �ŏ���V�̒l���Z�b�g���邩��4�̈����̕�
	m_FaceLeft.m_pFace2D->SetUV(&(m_FaceLeft.m_UV));
	m_FaceRight.m_pFace2D->SetUV(&(m_FaceRight.m_UV));
	// �ŏ��͌����Ȃ����瓧����
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pBack->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceRight.m_pBack->SetColorPolygon(m_Anime2DColorJijiiRight);

	// �����낪��
	m_pFireLeftBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y - FIRE_OFFSET.y, 0),
		FIRE_BACK_WIDTH, FIRE_BACK_HEIGHT, TEXTURE_FIRE_BACK);
	m_pFireRightBack = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y - FIRE_OFFSET.y, 0),
		FIRE_BACK_WIDTH, FIRE_BACK_HEIGHT, TEXTURE_FIRE_BACK);
	// ���߂����
	m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
	m_pFireRightBack->SetColorPolygon(m_FireColorRight);

	// �ŏ���V�̒l���Z�b�g���邩��4�̈����̕�
	m_pFireLeftBack->SetUV(0, FIRE_ONE_WIDTH);
	m_pFireRightBack->SetUV(0, FIRE_ONE_WIDTH);

	// �`�揇������
	for (int i = 0; i < BAR_MAX; i++)
	{
		m_pBar[i].m_p2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	}
	m_FaceLeft.m_pBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrameLeftBar->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrameRightBar->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrameLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFrameRight->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFireLeftBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pFireRightBack->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceLeft.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
}

//=============================================================================
// �I��
//=============================================================================
void CHpBar::Uninit(void)
{
}

//=============================================================================
//=============================================================================
void CHpBar::CBarBase::SetImvisible(void)
{
	m_p2D->SetDrawFlag(false);
}
//=============================================================================
//=============================================================================
void CHpBar::CBarBase::SetVisible(void)
{
	m_p2D->SetDrawFlag(true);
}

//=============================================================================
// �X�V
//=============================================================================
void CHpBar::Update(void)
{
#ifdef _DEBUG
	CDebugProc::PrintL("���̗́F%+10.3f / %+10.3f\n", (float)m_pBar[BAR_GREEN_L].m_Value, (float)m_ValueMax);
	CDebugProc::PrintL("�E�̗́F%+10.3f / %+10.3f\n", (float)m_pBar[BAR_GREEN_R].m_Value, (float)m_ValueMax);
	if (m_FaceLeft.m_Expression == EXPRESSION_GOOD)
		CDebugProc::PrintL("���\��FGOOD\n");
	else if (m_FaceLeft.m_Expression == EXPRESSION_NORAML)
		CDebugProc::PrintL("���\��FNOAML\n");
	else
		CDebugProc::PrintL("���\��FBAD\n");
	
	if (m_FaceRight.m_Expression == EXPRESSION_GOOD)
		CDebugProc::PrintL("�E�\��FGOOD\n");
	else if (m_FaceRight.m_Expression == EXPRESSION_NORAML)
		CDebugProc::PrintL("�E�\��FNOAML\n");
	else
		CDebugProc::PrintL("�E�\��FBAD\n");
#endif
	// �J�n�A�j���[�V�����̍X�V
	if (m_isAnime)
	{
		UpdateAnime();
		return;
	}

	// �k���̍X�V
	UpdateShake();

	// �����݂ǂ�̕�Ԃ��s���Ȃ�
	if (m_pBar[BAR_GREEN_L].m_TimerEasing < 1.0f)
	{
		float posX = EasingInterpolation(
			m_pBar[BAR_GREEN_L].m_PosEasingStart,
			m_pBar[BAR_GREEN_L].m_PosEasingEnd,
			m_pBar[BAR_GREEN_L].m_TimerEasing);

		// ���_������
		m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(posX);
		// ��Ԃ̎��ԍX�V
		m_pBar[BAR_GREEN_L].m_TimerEasing += ERASE_ONE_FRAME_G;
	}
	// �E���݂ǂ�̕�Ԃ��s���Ȃ�
	if (m_pBar[BAR_GREEN_R].m_TimerEasing < 1.0f)
	{
		float posX = EasingInterpolation(
			m_pBar[BAR_GREEN_R].m_PosEasingStart,
			m_pBar[BAR_GREEN_R].m_PosEasingEnd,
			m_pBar[BAR_GREEN_R].m_TimerEasing);

		// ���_������
		m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(posX);
		// ��Ԃ̎��ԍX�V
		m_pBar[BAR_GREEN_R].m_TimerEasing += ERASE_ONE_FRAME_G;
	}

	// ���̐Ԃ��o�[��΂ɍ��킹��
	if (m_isRedResetLeft){
		m_RedResetCountLeft++;
		if (m_RedResetCountLeft > RED_CHANGE_INTERVAL){
			// �Ԃ��o�[�̒l��΂ɍ��킹��
			m_pBar[BAR_RED_L].m_Value = m_pBar[BAR_GREEN_L].m_Value;
			m_pBar[BAR_RED_L].m_TimerEasing = 0;
			// ��ԂŎg���ړ��O�ƈړ���̍��W��ۑ�
			if (m_isRedEasingLeft)
				m_pBar[BAR_RED_L].m_PosEasingStart = m_pBar[BAR_RED_L].m_PosEasingEnd;
			else
				m_pBar[BAR_RED_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosEasingStart;
			m_pBar[BAR_RED_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosEasingEnd;

			// �t���O������
			m_isRedResetLeft = false;
			// �Ԃ��o�[�̐��`��Ԃ�����X�V�t���Otrue
			m_isRedEasingLeft = true;
		}
	}
	// ���Ԃ��o�[�̐��`��ԍX�V
	if (m_isRedEasingLeft){
		// ��������Ԃ��s���Ȃ�
		if (m_pBar[BAR_RED_L].m_TimerEasing < 1.0f)
		{
			float posX = EasingInterpolation(
				m_pBar[BAR_RED_L].m_PosEasingStart,
				m_pBar[BAR_RED_L].m_PosEasingEnd,
				m_pBar[BAR_RED_L].m_TimerEasing);

			// ���_������
			m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(posX);
			// ��Ԃ̎��ԍX�V
			m_pBar[BAR_RED_L].m_TimerEasing += ERASE_ONE_FRAME_R;
		}
	}

	// �E�̐Ԃ��o�[��΂ɍ��킹��
	if (m_isRedResetRight){
		m_RedResetCountRight++;
		if (m_RedResetCountRight > RED_CHANGE_INTERVAL){
			// �t���O������
			m_isRedResetRight = false;
			// �Ԃ��o�[�̐��`��Ԃ�����X�V�t���Otrue
			m_isRedEasingRight = true;
			// �Ԃ��o�[�̒l��΂ɍ��킹��
			m_pBar[BAR_RED_R].m_Value = m_pBar[BAR_GREEN_R].m_Value;
			m_pBar[BAR_RED_R].m_TimerEasing = 0;
			// ��ԂŎg���ړ��O�ƈړ���̍��W��ۑ�
			if (m_isRedEasingLeft)
				m_pBar[BAR_RED_R].m_PosEasingStart = m_pBar[BAR_RED_R].m_PosEasingEnd;
			else
				m_pBar[BAR_RED_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosEasingStart;
			m_pBar[BAR_RED_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosEasingEnd;

		}
	}
	// �E�Ԃ��o�[�̐��`��ԍX�V
	if (m_isRedEasingRight){
		// ��������Ԃ��s���Ȃ�
		if (m_pBar[BAR_RED_R].m_TimerEasing < 1.0f)
		{
			float posX = EasingInterpolation(
				m_pBar[BAR_RED_R].m_PosEasingStart,
				m_pBar[BAR_RED_R].m_PosEasingEnd,
				m_pBar[BAR_RED_R].m_TimerEasing);

			// ���_������
			m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(posX);
			// ��Ԃ̎��ԍX�V
			m_pBar[BAR_RED_R].m_TimerEasing += ERASE_ONE_FRAME_R;
		}
	}

	// �������邩���̍X�V
	UpdateWhite();

	// ���̃e�N�X�`���A�j���[�V����
	UpdateFire();
}

// �����ł�
void CHpBar::FireAppearLeft(void)
{
	m_isFireAppearLeft = true;
	m_FireColorLeft.a = 0.0f;
	m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
}
// ����������
void CHpBar::FireDisappearLeft(void)
{
	m_isFireDisappearLeft = true;
	m_FireColorLeft.a = 1.0f;
	m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
}

// �݂����ł�
void CHpBar::FireAppearRight(void)
{
	m_isFireAppearRight = true;
	m_FireColorRight.a = 0.0f;
	m_pFireRightBack->SetColorPolygon(m_FireColorRight);
}
// �݂���������
void CHpBar::FireDisappearRight(void)
{
	m_isFireDisappearRight = true;
	m_FireColorRight.a = 1.0f;
	m_pFireRightBack->SetColorPolygon(m_FireColorRight);
}
//=============================================================================
// ���̉����o��
//=============================================================================
void CHpBar::FireLeft(bool isAppear)
{
	if (isAppear)
	{
		// �����܂������Ă��Ȃ�������
		if (!m_isFireLeft)
		{
			m_isFireLeft = true;
			FireAppearLeft();
		}
	}
	else
	{
		// �����łĂ�����
		if (m_isFireLeft)
		{
			m_isFireLeft = false;
			FireDisappearLeft();
		}
	}
}
//=============================================================================
// �E�̉����o��
//=============================================================================
void CHpBar::FireRight(bool isAppear)
{
	if (isAppear)
	{
		// �����܂������Ă��Ȃ�������
		if (!m_isFireRight)
		{
			m_isFireRight = true;
			FireAppearRight();
		}
	}
	else
	{
		// �����łĂ�����
		if (m_isFireRight)
		{
			m_isFireRight = false;
			FireDisappearRight();
		}
	}
}

//=============================================================================
// ���̍X�V
//=============================================================================
void CHpBar::UpdateFire(void)
{
	// �������X�V�@�Ђ���
	if (m_isFireAppearLeft)
	{
		m_FireColorLeft.a += FIRE_FADE_SPEED;
		if (m_FireColorLeft.a >= 1.0f)
		{
			m_FireColorLeft.a = 1;
			m_isFireAppearLeft = false;
		}
		m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
	}
	if (m_isFireDisappearLeft)
	{
		m_FireColorLeft.a -= FIRE_FADE_SPEED;
		if (m_FireColorLeft.a <= 0.0f)
		{
			m_FireColorLeft.a = 0;
			m_isFireDisappearLeft = false;
		}
		m_pFireLeftBack->SetColorPolygon(m_FireColorLeft);
	}
	// �������X�V�@�݂�
	if (m_isFireAppearRight)
	{
		m_FireColorRight.a += FIRE_FADE_SPEED;
		if (m_FireColorRight.a >= 1.0f)
		{
			m_FireColorRight.a = 1;
			m_isFireAppearRight = false;
		}
		m_pFireRightBack->SetColorPolygon(m_FireColorRight);
	}
	if (m_isFireDisappearRight)
	{
		m_FireColorRight.a -= FIRE_FADE_SPEED;
		if (m_FireColorRight.a <= 0.0f)
		{
			m_FireColorRight.a = 0;
			m_isFireDisappearRight = false;
		}
		m_pFireRightBack->SetColorPolygon(m_FireColorRight);
	}

	// ���̃A�j���[�V����
	if (m_isFireLeft)
	{
		m_FireTextureCountLeft++;
		if (m_FireTextureCountLeft > FIRE_TEXTURE_SPEED)
		{
			m_FireTextureCountLeft = 0;
			m_FireTextureLeft += FIRE_ONE_WIDTH;
			if (m_FireTextureLeft > FIRE_TEXTURE_MAX * FIRE_ONE_WIDTH)
			{
				m_FireTextureLeft = FIRE_TEXTURE_MIN * FIRE_ONE_WIDTH;
			}
			m_pFireLeftBack->SetUV(0 + m_FireTextureLeft, 0 + m_FireTextureLeft + FIRE_ONE_WIDTH);
		}
	}
	if (m_isFireRight)
	{
		m_FireTextureCountRight++;
		if (m_FireTextureCountRight > FIRE_TEXTURE_SPEED)
		{
			m_FireTextureCountRight = 0;
			m_FireTextureRight += FIRE_ONE_WIDTH;
			if (m_FireTextureRight > FIRE_TEXTURE_MAX * FIRE_ONE_WIDTH)
			{
				m_FireTextureRight = FIRE_TEXTURE_MIN * FIRE_ONE_WIDTH;
			}
			m_pFireRightBack->SetUV(0 + m_FireTextureRight, 0 + m_FireTextureRight + FIRE_ONE_WIDTH);
		}
	}
}

//=============================================================================
// �������锻��
//=============================================================================
void CHpBar::WhiteJudge(void)
{
	m_isChangeWhiteLeft = m_pBar[BAR_GREEN_L].m_Value < WHITE_CHANGE_VALUE;
	m_isChangeWhiteRight = m_pBar[BAR_GREEN_R].m_Value < WHITE_CHANGE_VALUE;

	// �����������������
	if (!m_isChangeWhiteLeft)
	{
		m_pBar[BAR_GREEN_L].m_p2D->InitWhite();
	}
	if (!m_isChangeWhiteRight)
	{
		m_pBar[BAR_GREEN_R].m_p2D->InitWhite();
	}
}

//=============================================================================
// ���������̍X�V
//=============================================================================
void CHpBar::UpdateWhite(void)
{
	// ��������Ȃ甒�������
	if (m_isChangeWhiteLeft){
		if (m_isAddWhiteLeft){
			bool isEnd = m_pBar[BAR_GREEN_L].m_p2D->AddWhite(WHITE_SPEED);
			if (isEnd){
				m_isAddWhiteLeft = false;
			}
		}
		else{
			bool isEnd = m_pBar[BAR_GREEN_L].m_p2D->AddWhite(-WHITE_SPEED);
			if (isEnd){
				m_isAddWhiteLeft = true;
			}
		}
	}
	if (m_isChangeWhiteRight){
		if (m_isAddWhiteRight){
			bool isEnd = m_pBar[BAR_GREEN_R].m_p2D->AddWhite(WHITE_SPEED);
			if (isEnd){
				m_isAddWhiteRight = false;
			}
		}
		else{
			bool isEnd = m_pBar[BAR_GREEN_R].m_p2D->AddWhite(-WHITE_SPEED);
			if (isEnd){
				m_isAddWhiteRight = true;
			}
		}
	}
}

//=============================================================================
// �k�킷�X�V
//=============================================================================
void CHpBar::UpdateShake(void)
{
	// �����̐k�킷
	if (m_isShakeLeft)
	{
		m_ShakeCountLeft++;
		if (m_ShakeCountLeft > SHAKE_INTERVAL)
		{
			// �t���O�؂�
			m_isShakeLeft = false;
			// ���̍��W�ɖ߂�
			m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonY(m_PosCenterY);
			m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonY(m_PosCenterY);
			m_pFrameLeft->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_pFrameLeftBar->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_FaceLeft.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
			JudgeExpressionLeft();
		}
		// ������ō��W��������
		if (m_ShakeCountLeft % 2 == 0)
			m_ShakePosYLeft = m_ShakeRangeLeft;
		else
			m_ShakePosYLeft = -m_ShakeRangeLeft;
		// �o�[�̍��W�𓮂���
		m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonY(m_PosCenterY + m_ShakePosYLeft);
		m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonY(m_PosCenterY + m_ShakePosYLeft);
		m_pFrameLeft->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYLeft);
		m_pFrameLeftBar->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYLeft);
		m_FaceLeft.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
		
		// �������͈͂�����������
		m_ShakeRangeLeft *= SHAKE_RANGE_RESIST;
	}
	// �E���̐k�킷
	if (m_isShakeRight)
	{
		m_ShakeCountRight++;
		if (m_ShakeCountRight > SHAKE_INTERVAL)
		{
			// �t���O�؂�
			m_isShakeRight = false;
			// ���̍��W�ɖ߂�
			m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonY(m_PosCenterY);
			m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonY(m_PosCenterY);
			m_pFrameRight->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_pFrameRightBar->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_FaceRight.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
			// �\������݂�HP�ɉ����ĕύX
			JudgeExpressionRight();
		}
		// ������ō��W��������
		if (m_ShakeCountRight % 2 == 0)
			m_ShakePosYRight = m_ShakeRangeRight;
		else
			m_ShakePosYRight = -m_ShakeRangeRight;
		// �o�[�̍��W�𓮂���
		m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonY(m_PosCenterY + m_ShakePosYRight);
		m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonY(m_PosCenterY + m_ShakePosYRight);
		m_pFrameRight->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYRight);
		m_pFrameRightBar->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYRight);
		m_FaceRight.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYRight);
		// �������͈͂�����������
		m_ShakeRangeRight *= SHAKE_RANGE_RESIST;		
	}
}

//=============================================================================
// �`��
//=============================================================================
void CHpBar::DrawUI(void)
{
}

//=============================================================================
// �쐬
//=============================================================================
CHpBar* CHpBar::Create(
	float height,
	float posCenterY,
	float posLeftBarLeftX,
	float posLeftBarRightX,
	float posRightBarLeftX,
	float posRightBarRightX,
	LPDIRECT3DDEVICE9 *pDevice)
{
	CHpBar* p = new CHpBar(pDevice);
	p->Init(
		height,
		posCenterY,
		posLeftBarLeftX, posLeftBarRightX,
		posRightBarLeftX, posRightBarRightX);
	return p;
}

//=============================================================================
// ���T�C�h��HP�������̒l�ŉ��Z
//=============================================================================
void CHpBar::AddLeft(float value)
{
	// value���͈͊O�̎��̓N�����v�����l�𗘗p
	if (m_pBar[BAR_GREEN_L].m_Value + value > m_ValueMax)
	{
		value = m_ValueMax - m_pBar[BAR_GREEN_L].m_Value;
	}

	// ��ԂŎg���ړ��O�ƈړ���̍��W��ۑ�
	m_pBar[BAR_GREEN_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_pBar[BAR_GREEN_L].m_Value;
	m_pBar[BAR_GREEN_L].m_Value += value;
	m_pBar[BAR_GREEN_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_pBar[BAR_GREEN_L].m_Value;

	// �ꉞ�N�����v
	m_pBar[BAR_GREEN_L].m_Value = min(m_pBar[BAR_GREEN_L].m_Value, m_ValueMax);

	// ��Ԃ���Ƃ��̃^�C�}��������
	m_pBar[BAR_GREEN_L].m_TimerEasing = 0;

	// �ԃo�[�ύX����t���O
	m_isRedResetLeft = true;
	m_RedResetCountLeft = 0;

	// HP���������ꍇ�ɂ͑����ɐԂ��o�[���݂ǂ�Ɠ������ɂ���
	m_RedResetCountLeft = RED_CHANGE_INTERVAL;

	// �\��ύX
	JudgeExpressionLeft();

	// �������邩����
	WhiteJudge();
}

//=============================================================================
// ���T�C�h��HP�������̒l�Ō��Z
//=============================================================================
void CHpBar::SubLeft(float value)
{
	value *= -1;

	// value���͈͊O�̎��̓N�����v�����l�𗘗p
	if (m_pBar[BAR_GREEN_L].m_Value + value < 0)
	{
		value = -m_pBar[BAR_GREEN_L].m_Value;
	}

	// ��ԂŎg���ړ��O�ƈړ���̍��W��ۑ�
	m_pBar[BAR_GREEN_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_pBar[BAR_GREEN_L].m_Value;
	m_pBar[BAR_GREEN_L].m_Value += value;
	m_pBar[BAR_GREEN_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_pBar[BAR_GREEN_L].m_Value;

	// �ꉞ�N�����v
	m_pBar[BAR_GREEN_L].m_Value = max(m_pBar[BAR_GREEN_L].m_Value, 0);

	// �ԃo�[�ύX����t���O
	m_isRedResetLeft = true;
	m_RedResetCountLeft = 0;

	// ��Ԃ���Ƃ��̃^�C�}��������
	m_pBar[BAR_GREEN_L].m_TimerEasing = 0;

	// �k�킷
	ShakeLeft();

	// �������邩����
	WhiteJudge();
}

//=============================================================================
// �E�T�C�h��HP�������̒l�ŉ��Z
//=============================================================================
void CHpBar::AddRight(float value)
{
	// value���͈͊O�̎��̓N�����v�����l�𗘗p
	if (m_pBar[BAR_GREEN_R].m_Value + value > m_ValueMax)
	{
		value = m_ValueMax - m_pBar[BAR_GREEN_R].m_Value;
	}

	// ��ԂŎg���ړ��O�ƈړ���̍��W��ۑ�
	m_pBar[BAR_GREEN_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_pBar[BAR_GREEN_R].m_Value;
	m_pBar[BAR_GREEN_R].m_Value += value;
	m_pBar[BAR_GREEN_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_pBar[BAR_GREEN_R].m_Value;

	// �ꉞ�N�����v
	m_pBar[BAR_GREEN_R].m_Value = min(m_pBar[BAR_GREEN_R].m_Value, m_ValueMax);

	// ��Ԃ���Ƃ��̃^�C�}��������
	m_pBar[BAR_GREEN_R].m_TimerEasing = 0;

	// �ԃo�[�ύX����t���O
	m_isRedResetRight = true;
	m_RedResetCountRight = 0;

	// HP���������ꍇ�ɂ͑����ɐԂ��o�[���݂ǂ�Ɠ������ɂ���
	m_RedResetCountRight = RED_CHANGE_INTERVAL;
	
	// �\��ύX
	JudgeExpressionRight();

	// �������邩����
	WhiteJudge();
}

//=============================================================================
// �E�T�C�h��HP�������̒l�Ō��Z
//=============================================================================
void CHpBar::SubRight(float value)
{
	value *= -1;

	// value���͈͊O�̎��̓N�����v�����l�𗘗p
	if (m_pBar[BAR_GREEN_R].m_Value + value < 0)
	{
		value = -m_pBar[BAR_GREEN_R].m_Value;
	}
	// ��ԂŎg���ړ��O�ƈړ���̍��W��ۑ�
	m_pBar[BAR_GREEN_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_pBar[BAR_GREEN_R].m_Value;
	m_pBar[BAR_GREEN_R].m_Value += value;
	m_pBar[BAR_GREEN_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_pBar[BAR_GREEN_R].m_Value;

	// �ꉞ�N�����v
	m_pBar[BAR_GREEN_R].m_Value = max(m_pBar[BAR_GREEN_R].m_Value, 0);

	// ��Ԃ���Ƃ��̃^�C�}��������
	m_pBar[BAR_GREEN_R].m_TimerEasing = 0;

	// �ԃo�[�ύX����t���O
	m_isRedResetRight = true;
	m_RedResetCountRight = 0;

	// �k�킷
	ShakeRight();

	// �������邩����
	WhiteJudge();
}

//=============================================================================
// �J�n�A�j���[�V����������@������
// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
//=============================================================================
void CHpBar::StartAnimation(int endCount)
{
#ifdef _DEBUG
	assert(endCount > 0 && "endCount�̓}�C�i�X�̒l����Ȃ��ŁI");
#endif
	// �A�j���[�V�������I������t���[����
	m_AnimeCountMax = endCount;

	// �A�j���[�V�������邽�߂̕ϐ�������
	m_AnimeCount = 0;
	m_isAnime = true;
	m_AnimeEasingOneFrame = 1.0f / static_cast<float>(endCount);
	m_AnimeOneFrameAlpha = 1.0f / endCount;
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);

	// ���ꂢ��Ȃ���
	m_pBar[BAR_GREEN_L].m_TimerEasing = 0;
	m_pBar[BAR_GREEN_R].m_TimerEasing = 0;
	// �A�j���[�V�����p�̕ۊǃ^�C�}
	m_AnimeTimerEasing = 0;

	m_pBar[BAR_GREEN_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosRight;
	m_pBar[BAR_GREEN_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosRight - m_WidthOneValue * m_ValueMax;
	m_pBar[BAR_GREEN_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosLeft;
	m_pBar[BAR_GREEN_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosLeft + m_WidthOneValue * m_ValueMax;

	// �o�[��Start�ʒu�ɓ�����
	m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_GREEN_L].m_PosRight);
	m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_GREEN_R].m_PosLeft);
	m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_RED_L].m_PosRight);
	m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_RED_R].m_PosLeft);

	// ������
	m_Anime2DColor = D3DXCOLOR(1, 1, 1, 0);
	m_Anime2DColorJijiiLeft = FACE_BACK_ALPHA_LEFT;
	m_Anime2DColorJijiiRight = FACE_BACK_ALPHA_RIGHT;
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pBack->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceRight.m_pBack->SetColorPolygon(m_Anime2DColorJijiiRight);

}

//=============================================================================
// �J�n�A�j���[�V����������X�V
//=============================================================================
void CHpBar::UpdateAnime()
{
	// �J�E���g
	m_AnimeCount++;
	
	// �o�[�Ƃ��̈ʒu
	if (m_AnimeTimerEasing < 1.0f){

		float leftX = EasingInterpolation(
			m_pBar[BAR_GREEN_L].m_PosEasingStart,
			m_pBar[BAR_GREEN_L].m_PosEasingEnd,
			m_AnimeTimerEasing);

		float rightX = EasingInterpolation(
			m_pBar[BAR_GREEN_R].m_PosEasingStart,
			m_pBar[BAR_GREEN_R].m_PosEasingEnd,
			m_AnimeTimerEasing);

		// ���_������
		m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(leftX);
		m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(leftX);
		m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(rightX);
		m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(rightX);

		// ��Ԃ̎��ԍX�V ����͍��݂ǂ�̃^�C�}�������p
		m_AnimeTimerEasing += m_AnimeEasingOneFrame;
	}

	// �A���t�@�l�X�V
	m_Anime2DColor.a += m_AnimeOneFrameAlpha;
	m_Anime2DColorJijiiLeft.a += m_AnimeOneFrameAlpha;
	m_Anime2DColorJijiiRight.a += m_AnimeOneFrameAlpha;
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceLeft.m_pBack->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceRight.m_pBack->SetColorPolygon(m_Anime2DColorJijiiRight);

	// �J�n�A�j���[�V�����I��
	if (m_AnimeCount > m_AnimeCountMax){
		// �����ŃA���t�@�l��1.0�ɂȂ�͂������ǈꉞ�I�����Ƃ��I
		m_Anime2DColor.a = 1.0f;
		m_Anime2DColorJijiiLeft.a = 1.0f;
		m_Anime2DColorJijiiLeft.a = 1.0f;
		m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
		m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
		m_FaceLeft.m_pBack->SetColorPolygon(m_Anime2DColorJijiiLeft);
		m_FaceRight.m_pBack->SetColorPolygon(m_Anime2DColorJijiiRight);
		m_isAnime = false;
	}


	// �J�n�A�j���[�V�����I��
	if (m_AnimeCount > m_AnimeCountMax){
		m_isAnime = false;
		Init();
	}
}

//=============================================================================
// �e�l�̏������@�J�n�A�j���V�����̌�ŌĂ�
//=============================================================================
void CHpBar::Init(){
	
	for (int i = 0; i < BAR_MAX; i++){
		m_pBar[i].m_Value = m_ValueMax;
		m_pBar[i].m_TimerEasing = 1;
	}
	// �o�[�̒l
	m_pBar[BAR_RED_L].m_PosEasingStart = m_pBar[BAR_GREEN_L].m_PosLeft;
	m_pBar[BAR_RED_L].m_PosEasingEnd = m_pBar[BAR_GREEN_L].m_PosLeft;
	m_pBar[BAR_RED_R].m_PosEasingStart = m_pBar[BAR_GREEN_R].m_PosRight;
	m_pBar[BAR_RED_R].m_PosEasingEnd = m_pBar[BAR_GREEN_R].m_PosRight;
}

//=============================================================================
// ������HP�o�[��k�킷
// �����F�k�킷����
//=============================================================================
void CHpBar::ShakeLeft()
{
	// �k�킷�����̏�����
	m_isShakeLeft = true;
	m_ShakeCountLeft = 0;
	m_ShakeRangeLeft = SHAKE_RANGE;

	// �U������Ă���\��ɃZ�b�g
	m_FaceLeft.m_Expression = EXPRESSION_ATTACKED;
	m_FaceLeft.SetUV();
}

//=============================================================================
// �E����HP�o�[��k�킷
// �����F�k�킷����
//=============================================================================
void CHpBar::ShakeRight()
{
	// �k�킷�����̏�����
	m_isShakeRight = true;
	m_ShakeCountRight = 0;
	m_ShakeRangeRight = SHAKE_RANGE;
	
	// �U������Ă���\��ɃZ�b�g
	m_FaceRight.m_Expression = EXPRESSION_ATTACKED;
	m_FaceRight.SetUV();
}

//=============================================================================
// ���݂�HP����\���ύX����BUV���Z�b�g����
//=============================================================================
void CHpBar::JudgeExpressionLeft()
{
	// ���̕\����Z�o
	for (int i = 0; i < EXPRESSION_MAX; i++)
	{
		if (HP_EXPRESSION[i] > m_pBar[BAR_GREEN_L].m_Value)
		{
			m_FaceLeft.m_Expression = static_cast<Expression>(i);
		}
	}
	// �\����e�N�X�`���ɃZ�b�g
	m_FaceLeft.SetUV();
}

//=============================================================================
// ���݂�HP����\���ύX����BUV���Z�b�g����
//=============================================================================
void CHpBar::JudgeExpressionRight()
{
	// ���̕\����Z�o
	for (int i = 0; i < EXPRESSION_MAX; i++)
	{
		if (HP_EXPRESSION[i] > m_pBar[BAR_GREEN_R].m_Value)
		{
			m_FaceRight.m_Expression = static_cast<Expression>(i);
		}
	}
	// �\����e�N�X�`���ɃZ�b�g
	m_FaceRight.SetUV();
}

//=============================================================================
// ��\��
//=============================================================================
void CHpBar::SetImvisible(void)
{
	for (int i = 0; i < BAR_MAX; i++)
	{
		m_pBar[i].SetImvisible();
	}
	m_pFrameLeft->SetDrawFlag(false);
	m_pFrameLeftBar->SetDrawFlag(false);
	m_pFrameRight->SetDrawFlag(false);
	m_pFrameRightBar->SetDrawFlag(false);
	m_FaceLeft.SetImvisible();
	m_FaceRight.SetImvisible();
	m_pFireLeftBack->SetDrawFlag(false);
	m_pFireRightBack->SetDrawFlag(false);
}

//=============================================================================
// �\��
//=============================================================================
void CHpBar::SetVisible(void)
{
	for (int i = 0; i < BAR_MAX; i++)
	{
		m_pBar[i].SetVisible();
	}
	m_pFrameLeft->SetDrawFlag(true);
	m_pFrameLeftBar->SetDrawFlag(true);
	m_pFrameRight->SetDrawFlag(true);
	m_pFrameRightBar->SetDrawFlag(true);
	m_FaceLeft.SetVisible();
	m_FaceRight.SetVisible();
	m_pFireLeftBack->SetDrawFlag(true);
	m_pFireRightBack->SetDrawFlag(true);
}

//=============================================================================
// HP�Q�[�W�̍ŏ�����ő��2D���W
//=============================================================================
float CHpBar::GetPosHpCenter(int playerNum)
{
	// �����̒l�̂܂�Ȃ�
	if (playerNum == 0)
	{
		// �͂�����
		return m_pBar[BAR_GREEN_L].m_PosEasingEnd;
		// �܂�Ȃ�
		return (m_pBar[BAR_GREEN_L].m_PosRight - m_pBar[BAR_GREEN_L].m_PosEasingEnd) * 0.5f + m_pBar[BAR_GREEN_L].m_PosEasingEnd;
	}
	else
	{
		// �͂�����
		return m_pBar[BAR_GREEN_R].m_PosEasingEnd;
		// �܂�Ȃ�
		return (m_pBar[BAR_GREEN_R].m_PosEasingEnd - m_pBar[BAR_GREEN_R].m_PosLeft) * 0.5f + m_pBar[BAR_GREEN_R].m_PosLeft;
	}
}
//----EOF----