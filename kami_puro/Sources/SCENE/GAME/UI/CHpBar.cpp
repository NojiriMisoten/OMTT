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
// �o�[�̐Ԃ��Ȃ镔���̐F
//static const D3DXCOLOR BAR_COLOR_GREEN = D3DXCOLOR(0.0f, 0.8f, 0.4f, 1.0f);
//static const D3DXCOLOR BAR_COLOR_RED = D3DXCOLOR(1, 0.4f, 0.1f, 1.0f);
// TODO ����HP�ʁ@�v���C�����玝���Ă���
static const float HP_MAX = 255;
// �Ԃ��o�[��ύX����܂ł̃J�E���g��
static const short RED_CHANGE_INTERVAL = 40;
// �_���[�W���󂯂����o�[�����炷�܂ł̃t���[����(�o�[�̑��x)
static const float ERASE_INTERVAL = 20.0f;
// �_���[�W�����炷�Ƃ���1�t���[��������̗� ��Ԃ�time�̌v�Z�Ŏg��
static const float ERASE_ONE_FRAME = 1.0f / ERASE_INTERVAL;

// �g�̂�������
static const float BAR_FRAME_WIDTH = 800 * 0.8f;
static const float BAR_FRAME_HEIGHT = 200 * 0.8f;

// �o�[�̍��W�Ƙg�̍��W��offset
static const D3DXVECTOR2 BAR_FRAME_OFFSET = D3DXVECTOR2(30, 38);

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

	Init();

	// �o�[�̕�
	float barWidth = posLeftBarRightX - posLeftBarLeftX;

	// �l�im_Value�j�P������̃s�N�Z����(float)���v�Z
	m_WidthOneValue = barWidth / m_ValueMax;

	// �o�[�̍��W
	D3DXVECTOR3 pos[BAR_MAX] = {
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f,  posCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, posCenterY, 0),
		D3DXVECTOR3(posLeftBarLeftX + barWidth * 0.5f , posCenterY, 0),
		D3DXVECTOR3(posRightBarLeftX + barWidth * 0.5f, posCenterY, 0),
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

	// �F�`�F���W
//	m_pBar[BAR_RED_R].m_p2D->SetColorPolygon(BAR_COLOR_RED);
//	m_pBar[BAR_RED_L].m_p2D->SetColorPolygon(BAR_COLOR_RED);
//	m_pBar[BAR_GREEN_L].m_p2D->SetColorPolygon(BAR_COLOR_GREEN);
//	m_pBar[BAR_GREEN_R].m_p2D->SetColorPolygon(BAR_COLOR_GREEN);

	// �ŏ���UI�J�n�A�j���[�V���������邩��A�|���S�����Z�b�g������
	// �o�[��Start�ʒu�ɓ�����
	m_pBar[BAR_GREEN_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_GREEN_L].m_PosRight);
	m_pBar[BAR_GREEN_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_GREEN_R].m_PosLeft);
	m_pBar[BAR_RED_L].m_p2D->SetVertexPolygonLeft(m_pBar[BAR_RED_L].m_PosRight);
	m_pBar[BAR_RED_R].m_p2D->SetVertexPolygonRight(m_pBar[BAR_RED_R].m_PosLeft);

	// HP�̘g��
	m_pFrameLeft = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[0].x - BAR_FRAME_OFFSET.x, pos[0].y - BAR_FRAME_OFFSET.y, 0),
		BAR_FRAME_WIDTH, BAR_FRAME_HEIGHT,
		TEXTURE_HP_GAGE_FRAME);
	m_pFrameLeft->AddLinkList(CRenderer::TYPE_RENDER_UI);

	// HP�̘g�E
	m_pFrameRight = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(pos[1].x + BAR_FRAME_OFFSET.x, pos[1].y - BAR_FRAME_OFFSET.y, 0),
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
	// �J�n�A�j���[�V�����̍X�V
	if (m_isAnime)
	{
		UpdateAnime();
		return;
	}

	CDebugProc::PrintL("���̗� %d / %d\n", (int)m_pBar[BAR_GREEN_L].m_Value, (int)m_ValueMax);
	CDebugProc::PrintL("�E�̗� %d / %d\n", (int)m_pBar[BAR_GREEN_R].m_Value, (int)m_ValueMax);

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
		m_pBar[BAR_GREEN_L].m_TimerEasing += ERASE_ONE_FRAME;
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
		m_pBar[BAR_GREEN_R].m_TimerEasing += ERASE_ONE_FRAME;
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
			m_pBar[BAR_RED_L].m_TimerEasing += ERASE_ONE_FRAME;
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
			m_pBar[BAR_RED_R].m_TimerEasing += ERASE_ONE_FRAME;
		}
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
}

//=============================================================================
// �J�n�A�j���[�V����������X�V
//=============================================================================
void CHpBar::UpdateAnime()
{
	// �J�E���g
	m_AnimeCount++;
	
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
//----EOF----