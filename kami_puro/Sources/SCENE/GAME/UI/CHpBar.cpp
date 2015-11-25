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

//*****************************************************************************
// �萔
//*****************************************************************************
const float CHpBar::JIJII_TEX_U = 1.0f / 3.0f;
const float CHpBar::JIJII_TEX_V = 1.0f / 2.0f;

// TODO ����HP�ʁ@�v���C�����玝���Ă��邩�Q�[������Z�b�g����
static const float HP_MAX = 100;
// HP�̊����ɂ���ĕ\������@���̒l
static const float HP_EXPRESSION[CHpBar::EXPRESSION_MAX] = {
	100,
	60,
	30,
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
static const float BAR_FRAME_WIDTH = 800 * 0.8f;
static const float BAR_FRAME_HEIGHT = 200 * 0.8f;
// �o�[�̍��W�ɑ΂��Ă̘g�̍��W��offset
static const D3DXVECTOR2 BAR_FRAME_OFFSET = D3DXVECTOR2(49, 38);
// �o�[�̍��W�ɑ΂��Ă̂������̍��W��offset �o�[�̍��E�̒[���瑊�ΎQ��
static const D3DXVECTOR2 FACE_OFFSET = D3DXVECTOR2(37, -44);
// �������̂�������
static const float FACE_WIDTH = 120;
static const float FACE_HEIGHT = 150;
// �������̔w�i��������
static const float FACE_BACK_WIDTH = 160;
static const float FACE_BACK_HEIGHT = 160;
// �������̔w�i�̐F�̓��߂���ŏ��̐F
static const D3DXCOLOR FACE_BACK_ALPHA_LEFT = D3DXCOLOR(1.0f, 0.1f, 0.0f, 0.0f);
static const D3DXCOLOR FACE_BACK_ALPHA_RIGHT = D3DXCOLOR(0.0f, 0.1f, 1.0f, 0.0f);

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
	m_pFrameLeftTop = NULL;
	m_pFrameRightTop = NULL;

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
	m_FaceLeft.m_pBack2D = NULL;
	m_FaceLeft.m_pFace2D = NULL;
	m_FaceLeft.m_Pos = D3DXVECTOR2(0, 0);
	m_FaceLeft.m_UV = UV_INDEX(0, JIJII_TEX_U, 0, JIJII_TEX_V);
	m_FaceRight.m_Expression = EXPRESSION_GOOD;
	m_FaceRight.m_pBack2D = NULL;
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
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f,  m_PosCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f , m_PosCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, m_PosCenterY, 0),
	};

	for (int i = 0; i < BAR_MAX; i++)
	{
		// �o�[��2D�̍쐬
		m_pBar[i].m_p2D = CScene2D::Create(m_pD3DDevice,
			pos[i],
			barWidth, height,
			i < 2 ? TEXTURE_HP_GAGE_R : TEXTURE_HP_GAGE_G);
		// �o�[��2D������
		m_pBar[i].m_p2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
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
	// ��̔w�i
	m_FaceLeft.m_pBack2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		FACE_BACK_WIDTH, FACE_BACK_HEIGHT, TEXTURE_JIJII_BACK);
	m_FaceRight.m_pBack2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		FACE_BACK_WIDTH, FACE_BACK_HEIGHT, TEXTURE_JIJII_BACK);
	// �����_�\�ǉ�
	m_FaceLeft.m_pBack2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pBack2D->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// HP�̘g�� �㔼��
	m_pFrameLeftTop = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[0].x - BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_TOP);
	m_pFrameLeftTop->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// HP�̘g�E �㔼��
	m_pFrameRightTop = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[1].x + BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME_TOP);
	m_pFrameRightTop->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// ���]
	m_pFrameRightTop->SetUVMirror();

	// �琶��
	m_FaceLeft.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosL.x, facePosL.y, 0),
		FACE_WIDTH, FACE_HEIGHT, TEXTURE_JIJII);
	// �E������
	m_FaceRight.m_pFace2D = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(facePosR.x, facePosR.y, 0),
		FACE_WIDTH, FACE_HEIGHT, TEXTURE_JIJII);
	m_FaceLeft.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_FaceRight.m_pFace2D->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// �ŏ���V�̒l���Z�b�g���邩��4�̈����̕�
	m_FaceLeft.m_pFace2D->SetUV(&(m_FaceLeft.m_UV));
	m_FaceRight.m_pFace2D->SetUV(&(m_FaceRight.m_UV));
	// �ŏ��͌����Ȃ����瓧����
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiRight);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);

	// HP�̘g��
	m_pFrameLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[0].x - BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME);
	m_pFrameLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// HP�̘g�E
	m_pFrameRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[1].x + BAR_FRAME_OFFSET.x, m_PosCenterY - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME);
	m_pFrameRight->AddLinkList(CRenderer::TYPE_RENDER_UI);
	// ���]
	m_pFrameRight->SetUVMirror();

}

//=============================================================================
// �I��
//=============================================================================
void CHpBar::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CHpBar::Update(void)
{
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
			m_pFrameLeftTop->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_FaceLeft.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
			m_FaceLeft.m_pBack2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
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
		m_pFrameLeftTop->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYLeft);
		m_FaceLeft.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
		m_FaceLeft.m_pBack2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
		
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
			m_pFrameRightTop->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y);
			m_FaceRight.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
			m_FaceRight.m_pBack2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y);
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
		m_pFrameRightTop->SetVertexPolygonY(m_PosCenterY - BAR_FRAME_OFFSET.y + m_ShakePosYRight);
		m_FaceRight.m_pFace2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
		m_FaceRight.m_pBack2D->SetVertexPolygonY(m_PosCenterY + FACE_OFFSET.y + m_ShakePosYLeft);
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

	// �\��ύX
	JudgeExpressionLeft();
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

	// �\��ύX
	JudgeExpressionRight();
}

//=============================================================================
// �J�n�A�j���[�V����������@������
// �I������܂ł̃J�E���g(���t���[���A�j���[�V�������邩)
//=============================================================================
void CHpBar::StartAnimation(int endCount)
{
	assert(endCount > 0 && "endCount�̓}�C�i�X�̒l����Ȃ��ŁI");

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
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiRight);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);

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
	m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiLeft);
	m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiRight);
	m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
	// �J�n�A�j���[�V�����I��
	if (m_AnimeCount > m_AnimeCountMax){
		// �����ŃA���t�@�l��1.0�ɂȂ�͂������ǈꉞ�I�����Ƃ��I
		m_Anime2DColor.a = 1.0f;
		m_FaceLeft.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiLeft);
		m_FaceLeft.m_pFace2D->SetColorPolygon(m_Anime2DColor);
		m_FaceRight.m_pBack2D->SetColorPolygon(m_Anime2DColorJijiiRight);
		m_FaceRight.m_pFace2D->SetColorPolygon(m_Anime2DColor);
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
	m_isShakeLeft = true;
	m_ShakeCountLeft = 0;
	m_ShakeRangeLeft = SHAKE_RANGE;
}

//=============================================================================
// �E����HP�o�[��k�킷
// �����F�k�킷����
//=============================================================================
void CHpBar::ShakeRight()
{
	m_isShakeRight = true;
	m_ShakeCountRight = 0;
	m_ShakeRangeRight = SHAKE_RANGE;
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

//----EOF----