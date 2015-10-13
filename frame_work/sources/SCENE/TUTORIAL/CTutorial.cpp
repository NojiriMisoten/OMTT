//=============================================================================
//
// CTutorial�N���X [CTutorial.h]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CTutorial.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneAnime.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �G�̈ړ���
static const D3DXVECTOR3 TUTORIAL_PICT_VELO = D3DXVECTOR3(100, 0, 0);
// �G�̊��������ʒu
static const float TUTORIAL_PICT_RESIST = 0.98f;
// �G�̈ʒu
static const D3DXVECTOR3 TUTORIAL_PICT_POS[TUTORIAL_PICT_MAX] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f + SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
};
// �y�[�W���ɑΉ������e�N�X�`��
static const TEXTURE_TYPE TUTORIAL_PICT_TEXTURE[TUTORIAL_PAGE_MAX] = {
	TEXTURE_TUTORIAL_0,
	TEXTURE_TUTORIAL_1,
	TEXTURE_TUTORIAL_2,
	TEXTURE_TUTORIAL_3,
};
// ���A�j���[�V����
static const D3DXVECTOR3 TUTORIAL_ARROR_POS_R = D3DXVECTOR3(50, SCREEN_HEIGHT * 0.5f, 0);
static const D3DXVECTOR3 TUTORIAL_ARROR_POS_L = D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT * 0.5f, 0);
static const float TUTORIAL_ARROR_WIDTH = 100;
static const float TUTORIAL_ARROR_HEIGHT = 100;
static const int TUTORIAL_ARROW_ANIME_SPEED = 10;
static const int TUTORIAL_ARROR_TEXTURE_SEP_X = 1;
static const int TUTORIAL_ARROR_TEXTURE_SEP_Y = 1;
// ���̊g�k�l
static const float TUTORIAL_ARROW_SCALE = 1.3f;
// ��󂪊g��Ək����؂�ւ���C���^�[�o��
static const int TUTORIAL_ARROW_SCALE_INTERVAL = 40;
// �y�[�W�\���̍��W
static const D3DXVECTOR3 TUTORIAL_PAGE_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT -50, 0);
// �y�[�W�\���̂�������
static const float TUTORIAL_PAGE_WIDTH = 100;
static const float TUTORIAL_PAGE_HEIGHT = 100;
// �y�[�W�\���̃e�N�X�`���i�y�[�W���ɑΉ����Ă���j
static const TEXTURE_TYPE TUTORIAL_PAGE_TEXTURE[TUTORIAL_PAGE_MAX] = {
	TEXTURE_TUTORIAL_PAGE_1_4,
	TEXTURE_TUTORIAL_PAGE_2_4,
	TEXTURE_TUTORIAL_PAGE_3_4,
	TEXTURE_TUTORIAL_PAGE_4_4,
};

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTutorial::CTutorial(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTutorial ::~CTutorial(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CTutorial::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �ϐ�������
	m_pBackGround = NULL;
	m_vVelo = D3DXVECTOR3(0, 0, 0);
	m_nPage = 0;
	m_bRight = true;
	m_nPictNumLeft = 0;
	m_nPictNumCenter = 1;
	m_nPictNumRight = 2;
	m_pArrowRight = NULL;
	m_pArrowLeft = NULL;
	m_bArrowScale = true;
	m_nArrowCount = 0;
	m_pPage2D = NULL;

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �w�i
	m_pBackGround = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_TUTORIAL, TYPE_PRIORITY_BG);

	// �`���[�g���A���̊G
	for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
		m_p2D[n] = CScene2D::Create(m_pD3DDevice,
			TUTORIAL_PICT_POS[n],
			static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
			TEXTURE_TUTORIAL_0, TYPE_PRIORITY_FIELD);
	}

	// ���A�j���[�V����
	m_pArrowLeft = CSceneAnime::Create(m_pD3DDevice,
		TUTORIAL_ARROR_POS_L,
		TUTORIAL_ARROR_WIDTH, TUTORIAL_ARROR_HEIGHT,
		TEXTURE_TUTORIAL_ARROW, TUTORIAL_ARROR_TEXTURE_SEP_X, TUTORIAL_ARROR_TEXTURE_SEP_Y,
		TUTORIAL_ARROW_ANIME_SPEED, -1);

	m_pArrowRight = CSceneAnime::Create(m_pD3DDevice,
		TUTORIAL_ARROR_POS_R,
		TUTORIAL_ARROR_WIDTH, TUTORIAL_ARROR_HEIGHT,
		TEXTURE_TUTORIAL_ARROW, TUTORIAL_ARROR_TEXTURE_SEP_X, TUTORIAL_ARROR_TEXTURE_SEP_Y,
		TUTORIAL_ARROW_ANIME_SPEED, -1);
	m_pArrowRight->SetRot(D3DXVECTOR3(0, 0, D3DX_PI));

	// �y�[�W�\���QD
	m_pPage2D = CScene2D::Create(m_pD3DDevice,
		TUTORIAL_PAGE_POS,
		TUTORIAL_PAGE_WIDTH, TUTORIAL_PAGE_HEIGHT,
		TUTORIAL_PAGE_TEXTURE[m_nPage], TYPE_PRIORITY_FIELD);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// �a�f�l�Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM_TUTORIAL);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CTutorial::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CTutorial::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	// ���̃A�j���[�V����
	UpdateArrow();

	// �t�F�[�h�A�E�g���ă^�C�g���w���ǂ�
	// ���͂��ꂽ��PUSH START���s
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_DECIDE, 0)){
		CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}

	// �G���~�܂��Ă���Ƃ��̂ݓ�������
	if (m_vVelo.x == 0){
		// ���E�L�[�Ŏ��̃y�[�W�ֈړ�
		for (int i = 0; i < PLAYER_MAX; i++){
			if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
				CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_LEFT, i)){
				CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
				// �G�̈ړ��ʂ�����
				m_vVelo = TUTORIAL_PICT_VELO;
				// �t���O�؂�ւ�
				m_bRight = true;
				// �y�[�W�؂�ւ�
				m_nPage--;
				if (m_nPage <= -1){
					m_nPage = TUTORIAL_PAGE_MAX - 1;
	  			}
				// �e�N�X�`���؂�ւ�
				m_p2D[m_nPictNumLeft]->ChangeTexture(TUTORIAL_PICT_TEXTURE[m_nPage]);
				// ��񂾂��ʂ�
				break;
			}
			else if (CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
				CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_RIGHT, i)){
				CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
				// �G�̈ړ��ʂ�����
				m_vVelo = -TUTORIAL_PICT_VELO;
				// �t���O�؂�ւ�
				m_bRight = false;
				// �y�[�W�؂�ւ�
				m_nPage++;
				if (m_nPage > TUTORIAL_PAGE_MAX - 1){
					m_nPage = 0;
				}
				// �e�N�X�`���؂�ւ�
				m_p2D[m_nPictNumRight]->ChangeTexture(TUTORIAL_PICT_TEXTURE[m_nPage]);
				// ��񂾂��ʂ�
				break;
			}
		}
	}

	// �G�̓���
	for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
		m_vVelo *= TUTORIAL_PICT_RESIST;
		m_p2D[n]->AddPos(m_vVelo);
	}

	// �[���������蔻��
	if (m_bRight){
		if (m_p2D[m_nPictNumLeft]->GetPos().x >= SCREEN_WIDTH * 0.5f){
			// �ړ��ʏ�����
			m_vVelo = D3DXVECTOR3(0, 0, 0);

			// �Ή��Y�����̓���ւ�
			short t = m_nPictNumRight;
			m_nPictNumRight = m_nPictNumCenter;
			m_nPictNumCenter = m_nPictNumLeft;
			m_nPictNumLeft = t;

			// �S�ʒu����
			m_p2D[m_nPictNumLeft]->SetPos(TUTORIAL_PICT_POS[0]);
			m_p2D[m_nPictNumCenter]->SetPos(TUTORIAL_PICT_POS[1]);
			m_p2D[m_nPictNumRight]->SetPos(TUTORIAL_PICT_POS[2]);

			// �y�[�W�\���̐؂�ւ�
			m_pPage2D->ChangeTexture(TUTORIAL_PAGE_TEXTURE[m_nPage]);
		}
	}
	else{
		if (m_p2D[m_nPictNumRight]->GetPos().x <= SCREEN_WIDTH * 0.5f){
			// �ړ��ʏ�����
			m_vVelo = D3DXVECTOR3(0, 0, 0);

			// �Ή��Y�����̓���ւ�
			short t = m_nPictNumLeft;
			m_nPictNumLeft = m_nPictNumCenter;
			m_nPictNumCenter = m_nPictNumRight;
			m_nPictNumRight = t;

			// �S�ʒu����
			m_p2D[m_nPictNumLeft]->SetPos(TUTORIAL_PICT_POS[0]);
			m_p2D[m_nPictNumCenter]->SetPos(TUTORIAL_PICT_POS[1]);
			m_p2D[m_nPictNumRight]->SetPos(TUTORIAL_PICT_POS[2]);

			// �y�[�W�\���̐؂�ւ�
			m_pPage2D->ChangeTexture(TUTORIAL_PAGE_TEXTURE[m_nPage]);
		}
	}

}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CTutorial* CTutorial::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CTutorial* pTitle = NULL;

	// �쐬
	pTitle = new CTutorial;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
// ���A�j���[�V�����X�V
//*****************************************************************************
void CTutorial::UpdateArrow(){

	// ���J�E���g�Ŋg�k�t���O�؂�ւ�
	m_nArrowCount++;
	if (m_nArrowCount > TUTORIAL_ARROW_SCALE_INTERVAL){
		m_nArrowCount = 0;
		m_bArrowScale = !m_bArrowScale;
	}
	// �g�傷��
	if (m_bArrowScale){
		m_pArrowLeft->AddHeight_BaseBottom(-TUTORIAL_ARROW_SCALE);
		m_pArrowLeft->AddHeight_BaseTop(-TUTORIAL_ARROW_SCALE);
		m_pArrowRight->AddHeight_BaseBottom(-TUTORIAL_ARROW_SCALE);
		m_pArrowRight->AddHeight_BaseTop(-TUTORIAL_ARROW_SCALE);
	}
	// �k������
	else{
		m_pArrowLeft->AddHeight_BaseBottom(TUTORIAL_ARROW_SCALE);
		m_pArrowLeft->AddHeight_BaseTop(TUTORIAL_ARROW_SCALE);
		m_pArrowRight->AddHeight_BaseBottom(TUTORIAL_ARROW_SCALE);
		m_pArrowRight->AddHeight_BaseTop(TUTORIAL_ARROW_SCALE);
	}
}

//----EOF----