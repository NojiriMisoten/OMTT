//=============================================================================
//
// CCharaSelect�N���X [CCharaSelect.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCharaSelect.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneAnime.h"
#include "../GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// ���S�̑傫��
static const D3DXVECTOR2 CHARASELECT_LOGO_SIZE = D3DXVECTOR2(800, 150);
// ���S�̍��W
static const D3DXVECTOR3 CHARASELECT_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 75, 0);
// �v���C���w�i�̑傫��
static const D3DXVECTOR2 CHARASELECT_PLAYER_BG_SIZE = D3DXVECTOR2(320, 200);
static const D3DXVECTOR2 CHARASELECT_PLAYER_BG_SIZE_HALF = D3DXVECTOR2(160, 100);
// �v���C���w�i�̍��W
static const D3DXVECTOR3 CHARASELECT_PLAYER_BG_POS[PLAYER_MAX] = {
	D3DXVECTOR3(330, 280, 0),
	D3DXVECTOR3(950, 280, 0),
	D3DXVECTOR3(330, 510, 0),
	D3DXVECTOR3(950, 510, 0),
};
// PUSH�w�i�傫�� �F
static const D3DXVECTOR2 CHARASELECT_PUSH_BG_SIZE = D3DXVECTOR2(205, 95);
static const D3DXCOLOR CHARASELECT_PUSH_BG_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
// PUSH�傫��
static const D3DXVECTOR2 CHARASELECT_PUSH_SIZE = D3DXVECTOR2(200, 90);
// PUSH�̓_�ŊԊu
static const int CHARASELECT_PUSH_FLASH_INTERVAL = 15;
static const D3DXCOLOR CHARASELECT_PUSH_ADDCOLOR = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.0f);
// �����̑傫��
static const D3DXVECTOR2 CHARASELECT_RIMO_SIZE = D3DXVECTOR2(100, 160);
// �����̍��W
static const D3DXVECTOR3 CHARASELECT_RIMO_POS[PLAYER_MAX] = {
	D3DXVECTOR3(390, 300, 0),
	D3DXVECTOR3(1010, 300, 0),
	D3DXVECTOR3(390, 530, 0),
	D3DXVECTOR3(1010, 530, 0),
};
// �Q��2D�傫��
static const D3DXVECTOR2 CHARASELECT_JOIN_SIZE = D3DXVECTOR2(180, 80);
// �Q��2D���W
static const D3DXVECTOR3 CHARASELECT_JOIN_POS[PLAYER_MAX] = {
	D3DXVECTOR3(260, 230, 0),
	D3DXVECTOR3(880, 230, 0),
	D3DXVECTOR3(260, 460, 0),
	D3DXVECTOR3(880, 460, 0),
};
// �{�^���n�傫��
static const D3DXVECTOR2 CHARASELECT_BUTTON_SIZE = D3DXVECTOR2(200, 80);
static const D3DXVECTOR2 CHARASELECT_BUTTON_SIZE_HALF = D3DXVECTOR2(100, 40);
// ���������{�^���̈ʒu
static const D3DXVECTOR3 CHARASELECT_START_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400, 0);
// �L�[�R���t�B�O�{�^���̈ʒu
static const D3DXVECTOR3 CHARASELECT_CONFIG_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 610, 0);
// �J�[�\���̃}�b�N�X
static const int CHARASELECT_CURSOL_MAX = 3;
// �J�[�\���̑傫��
static const D3DXVECTOR2 CHARASELECT_CURSOL_SIZE = D3DXVECTOR2(50, 50);
// �v���C���[���Ƃ̃J�[�\���̏����ʒu
static const D3DXVECTOR3 CHARASELECT_CURSOLR_POS[PLAYER_MAX] = {
	D3DXVECTOR3(440, 220, 0),
	D3DXVECTOR3(1060, 220, 0),
	D3DXVECTOR3(440, 450, 0),
	D3DXVECTOR3(1060, 450, 0),
};
// �J�[�\���̊p�x
static const float CHARASELECT_CURSOL_RAD_MAX = D3DX_PI / 4;
static const float CHARASELECT_CURSOL_ADDRAD = 0.07f;
// �J�[�\���̐F
static const D3DXCOLOR CHARASELECT_CURSOL_ADDCOLOR = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.1f);
// �J�[�\���̈ړ���
static const float CHARASELECT_CURSOL_VELO = 4;
// �J�[�\���̓����蔻������Ƃ��̒����ʒu
static const D3DXVECTOR3 CHARASELECT_CURSOL_COLI_DIST = D3DXVECTOR3(-10, 10, 0);
// ��������2D�̑傫��
static const D3DXVECTOR2 CHARASELECT_READY_SIZE = D3DXVECTOR2(250, 100);
// �v���C���w�i�̍��W
static const D3DXVECTOR3 CHARASELECT_READY_POS[PLAYER_MAX] = {
	D3DXVECTOR3(330, 180, 0),
	D3DXVECTOR3(950, 180, 0),
	D3DXVECTOR3(330, 510, 0),
	D3DXVECTOR3(950, 510, 0),
};
// ���������e�N�X�`���̐F�i���ߓx�ݒ�j
static const D3DXCOLOR CHARASELECT_READY_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
// �S���������������Ă��玟�̃V�[���Ɉڍs����܂ł̎��� �Ȃ��ƂȂ����ꂽ��
static const int CHARASELECT_NEXT_PHASE_INTERVAL = 60;
// BG�̃A�j���[�V�����ړ���
static const float CHARASELECT_BG_VELO = 1;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCharaSelect::CCharaSelect(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCharaSelect ::~CCharaSelect(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CCharaSelect::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �ϐ�������
	for (int i = 0; i < PLAYER_MAX; i++){
		m_bPush[i] = false;
		m_pPush2D[i] = NULL;
		m_pPush2DBG[i] = NULL;
		m_nPushFlashCount[i] = 0;
		m_bPushFlash[i] = false;
		m_pRimo[i] = NULL;
		m_nRimoTextureIndex[i] = 0;
		m_nRimoTextureCount[i] = 0;
		m_nRimoTextureMax[i] = 0;
		m_nRimoTextureMin[i] = 0;
		m_pJoin2D[i] = NULL;
		m_nCursol[i] = 0;
		m_pCursol2D[i] = NULL;
		m_fCursolRad[i] = 0;
		m_bCursolFlg[i] = false;
		m_bReady[i] = false;
		m_pReady2D[i] = NULL;
		m_nReadyFlashCount[i] = 0;
		m_bReadyFlash[i] = false;
		m_PlayerMode[i] = PLAYER_MODE_NONE;
	}
	m_pConfig = NULL;
	m_pStart2D = NULL;
	m_bNextPhaseOnece = false;
	m_nNextPhaseCount = 0;
	m_nPlayerManualNum = 0;
	m_nPlayerReadyNum = 0;
	m_nBgDirection = 0;

	// �Q�킵�Ă��������擾����
	m_nPlayerJoinedNum = CManager::GetJoinNum();

	// �w�i
	InitializeBG();

	for (int i = 0; i < PLAYER_MAX; i++){
		// �v���C���w�i
		m_pPlayerBG[i] = CScene2D::Create(m_pD3DDevice,
			CHARASELECT_PLAYER_BG_POS[i],
			CHARASELECT_PLAYER_BG_SIZE.x, CHARASELECT_PLAYER_BG_SIZE.y,
			TEXTURE_CHARA_SELECT_PLAYER_BG, TYPE_PRIORITY_FIELD);
		// �v���C���̐F�ɍ��킹��
		m_pPlayerBG[i]->SetColorPolygon(PLAYER_COLOR[i] + D3DXCOLOR(0, 0, 0, 1));
		// �o�t�r�g�{�^���w�i
		m_pPush2DBG[i] = CScene2D::Create(m_pD3DDevice,
			CHARASELECT_PLAYER_BG_POS[i],
			CHARASELECT_PUSH_BG_SIZE.x, CHARASELECT_PUSH_BG_SIZE.y,
			TEXTURE_RESULT_PUSH, TYPE_PRIORITY_GOAL);
		// �v���C���̐F�ɍ��킹��
		m_pPush2DBG[i]->SetColorPolygon(CHARASELECT_PUSH_BG_COLOR);
		// �o�t�r�g�{�^��
		m_pPush2D[i] = CScene2D::Create(m_pD3DDevice,
			CHARASELECT_PLAYER_BG_POS[i],
			CHARASELECT_PUSH_SIZE.x, CHARASELECT_PUSH_SIZE.y,
			TEXTURE_RESULT_PUSH, TYPE_PRIORITY_GOAL);
		// �v���C���̐F�ɍ��킹��
		m_pPush2D[i]->SetColorPolygon(PLAYER_COLOR[i] + CHARASELECT_PUSH_ADDCOLOR);
		// ��������2D���ɍ���ăt���O�ŊǗ����悤
		m_pReady2D[i] = CScene2D::Create(m_pD3DDevice,
			CHARASELECT_PLAYER_BG_POS[i],
			CHARASELECT_READY_SIZE.x, CHARASELECT_READY_SIZE.y,
			TEXTURE_CHARA_SELECT_READY, TYPE_PRIORITY_UI);
		m_pReady2D[i]->SetColorPolygon(CHARASELECT_READY_COLOR);
		m_pReady2D[i]->SetDrawFlag(false);
	}

	// �X�^�[�g�{�^��
	m_pStart2D = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_START_POS,
		CHARASELECT_BUTTON_SIZE.x, CHARASELECT_BUTTON_SIZE.y,
		TEXTURE_CHARA_SELECT_START, TYPE_PRIORITY_GOAL);

	// �L�[�R���t�B�O�{�^��
	m_pConfig = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_CONFIG_POS,
		CHARASELECT_BUTTON_SIZE.x, CHARASELECT_BUTTON_SIZE.y,
		TEXTURE_CHARA_SELECT_CONFIG, TYPE_PRIORITY_GOAL);

	// PUSH2D�����Join�ō���ď����Ƃ���1�t���[���������邩��`�悵�Ȃ��悤�ɂ��Ă���
	m_pPush2D[0]->SetDrawFlag(false);
	m_pPush2DBG[0]->SetDrawFlag(false);

	// 1P�ƎQ�킵�Ă����������Q��
	Join(0);
	for (int i = 1; i < m_nPlayerJoinedNum; i++){
		Join(i);
	}

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// �a�f�l�Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM_STAGE_SELECT);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CCharaSelect::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CCharaSelect::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	// BG�̃A�j���[�V����
	UpdateBG();

	// PUSH�̓_�ŃA�j���[�V����
	UpdatePushAnimation();

	// �����̃A�j���[�V����
	UpdateRimoAnimation();

	// �Ȃ񂩂̓��͂���������v���C���[�Q��
	UpdateInputJoin();

	// �J�[�\���ɂ��Ă̍X�V
	UpdateCursol();

	// ���������̓_�ŃA�j���[�V���� ����݂ɂ����I
	//	UpdateReadyAnimation();

	// ���͂Ńe�N�X�`���A�j���[�V����
	UpdateInput();

	// �S��������������������X�e�[�W�Z���N�g�Ɉړ�
	if (!m_bNextPhaseOnece){
		// 1P�������������Ă��Ȃ��ꍇ�͐i�܂Ȃ�
		if (m_bReady[0]){
			// �Q�������v���C���l�����S��������������������
			if (m_nPlayerReadyNum == m_nPlayerManualNum){
				// ���̂ݎ��s
				m_bNextPhaseOnece = true;
				// �v���C���̐l���𑗂�
				CManager::SetPlayerNum(m_nPlayerManualNum);
				CManager::PlaySoundA(SOUND_LABEL_SE_START);
			}
		}
	}
	// ���S��������������������J�E���g������V�[���J��
	else{
		m_nNextPhaseCount++;
		if (m_nNextPhaseCount > CHARASELECT_NEXT_PHASE_INTERVAL){
			// �t�F�[�h�A�E�g�J�n
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
			// �R���t�B�O�w
			m_pManager->SetNextPhase(MODE_PHASE_STAGE_SELECT);
		}
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CCharaSelect* CCharaSelect::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CCharaSelect* pTitle = NULL;

	// �쐬
	pTitle = new CCharaSelect;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//-----------------------------------------------------------------------------
// �w�i������
//-----------------------------------------------------------------------------
void CCharaSelect::InitializeBG(void){
	// �S�̂̔w�i
	m_pBG = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0),
		static_cast<float>(SCREEN_WIDTH)* 2.1f, static_cast<float>(SCREEN_HEIGHT)* 2.1f,
		TEXTURE_BG_CHARA_SELECT, TYPE_PRIORITY_BG);

	// �L�����I�����Ă������S�\��
	m_pLogo = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_LOGO_POS,
		CHARASELECT_LOGO_SIZE.x, CHARASELECT_LOGO_SIZE.y,
		TEXTURE_CHARA_SELECT_LOGO, TYPE_PRIORITY_FIELD);
}

//-----------------------------------------------------------------------------
// BG�̍X�V
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateBG(){

	D3DXVECTOR3 pos = m_pBG->GetPos();

	if (m_nBgDirection == 3){
		if (pos.y > SCREEN_HEIGHT){
			m_nBgDirection = 0;
			pos.y = SCREEN_HEIGHT;
		}
	}
	if (m_nBgDirection == 2){
		if (pos.x > SCREEN_WIDTH){
			m_nBgDirection = 3;
			pos.x = SCREEN_WIDTH;
		}
	}
	if (m_nBgDirection == 1){
		if (pos.y < 0){
			m_nBgDirection = 2;
			pos.y = 0;
		}
	}
	if (m_nBgDirection == 0){
		if (pos.x < 0){
			m_nBgDirection = 1;
			pos.x = 0;
		}
	}

	// ����
	if (m_nBgDirection == 0) m_vBgVelo = D3DXVECTOR3(-CHARASELECT_BG_VELO, 0, 0);
	// ���
	if (m_nBgDirection == 1) m_vBgVelo = D3DXVECTOR3(0, -CHARASELECT_BG_VELO, 0);
	// �E��
	if (m_nBgDirection == 2) m_vBgVelo = D3DXVECTOR3(CHARASELECT_BG_VELO, 0, 0);
	// ����
	if (m_nBgDirection == 3) m_vBgVelo = D3DXVECTOR3(0, CHARASELECT_BG_VELO, 0);

	// �ړ��ʉ��Z
	m_pBG->AddPos(m_vBgVelo);
}

//-----------------------------------------------------------------------------
// �w�肵���v���C�����Q��
//-----------------------------------------------------------------------------
void CCharaSelect::Join(int playerNum){

	// ���łɎQ���ς݂͔�����
	if (m_bPush[playerNum]){
		return;
	}

	CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_SANSEN);

	// �������t���O
	m_bPush[playerNum] = true;

	// �o�t�r�g2D����
	SAFE_RELEASE(m_pPush2DBG[playerNum]);
	SAFE_RELEASE(m_pPush2D[playerNum]);

	// ����ł���v���C���l���C���N�������g
	m_nPlayerManualNum++;

	// ���2D
	m_pRimo[playerNum] = CSceneAnime::Create(m_pD3DDevice,
		CHARASELECT_RIMO_POS[playerNum],
		CHARASELECT_RIMO_SIZE.x, CHARASELECT_RIMO_SIZE.y,
		PLAYER_TEXTURE[playerNum], PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
		PLAYER_ANIME_SPEED, -1, TYPE_PRIORITY_PLAYER);

	// �����Ńe�N�X�`���A�j���[�V�������Ȃ�
	m_pRimo[playerNum]->SetAutoUpdate(false);
	// ���ʂ̕������[�V����
	m_nRimoTextureIndex[playerNum] = PLAYER_STOP_TEXTURE_MIN;
	m_pRimo[playerNum]->SetIndex(m_nRimoTextureIndex[playerNum]);
	m_nRimoTextureMin[playerNum] = PLAYER_STOP_TEXTURE_MIN;
	m_nRimoTextureMax[playerNum] = PLAYER_STOP_TEXTURE_MAX;
	// ����ς萳�ʌ����ĐÎ~
	m_nRimoTextureIndex[playerNum] = PLAYER_STOP_TEXTURE_MIN;
	m_pRimo[playerNum]->SetIndex(m_nRimoTextureIndex[playerNum]);
	m_nRimoTextureMin[playerNum] = PLAYER_STOP_TEXTURE_MIN;
	m_nRimoTextureMax[playerNum] = PLAYER_STOP_TEXTURE_MAX;

	// �Q��2D
	m_pJoin2D[playerNum] = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_JOIN_POS[playerNum],
		CHARASELECT_JOIN_SIZE.x, CHARASELECT_JOIN_SIZE.y,
		TEXTURE_CHARA_SELECT_JOIN, TYPE_PRIORITY_PLAYER);

	// �J�[�\����������
	m_pCursol2D[playerNum] = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_CURSOLR_POS[playerNum],
		CHARASELECT_CURSOL_SIZE.x, CHARASELECT_CURSOL_SIZE.y,
		TEXTURE_CHARA_SELECT_CURSOL, TYPE_PRIORITY_UI);
	m_pCursol2D[playerNum]->SetColorPolygon(PLAYER_COLOR[playerNum] + CHARASELECT_CURSOL_ADDCOLOR);

	// ������ԏ�����
	if (m_pReady2D[playerNum])
		m_pReady2D[playerNum]->SetDrawFlag(false);
	m_bReady[playerNum] = false;
}

//-----------------------------------------------------------------------------
// �w�肵���v���C����s�Q��
//-----------------------------------------------------------------------------
void CCharaSelect::UnJoin(int playerNum){

	// ���łɕs�Q���Ȃ甲����
	if (!m_bPush[playerNum]){
		return;
	}
	// �����������Ă��������
	if (m_bReady[playerNum]){
		m_bReady[playerNum] = false;
		m_pReady2D[playerNum]->SetDrawFlag(false);
		m_nReadyFlashCount[playerNum] = 0;
		m_bReadyFlash[playerNum] = false;
		m_nPlayerReadyNum--;
	}

	// �������t���O
	m_bPush[playerNum] = false;

	// �o�t�r�g�{�^���w�i
	m_pPush2DBG[playerNum] = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_PLAYER_BG_POS[playerNum],
		CHARASELECT_PUSH_BG_SIZE.x, CHARASELECT_PUSH_BG_SIZE.y,
		TEXTURE_RESULT_PUSH, TYPE_PRIORITY_GOAL);
	// �v���C���̐F�ɍ��킹��
	m_pPush2DBG[playerNum]->SetColorPolygon(CHARASELECT_PUSH_BG_COLOR);
	// �o�t�r�g�{�^��
	m_pPush2D[playerNum] = CScene2D::Create(m_pD3DDevice,
		CHARASELECT_PLAYER_BG_POS[playerNum],
		CHARASELECT_PUSH_SIZE.x, CHARASELECT_PUSH_SIZE.y,
		TEXTURE_RESULT_PUSH, TYPE_PRIORITY_GOAL);
	// �v���C���̐F�ɍ��킹��
	m_pPush2D[playerNum]->SetColorPolygon(PLAYER_COLOR[playerNum] + CHARASELECT_PUSH_ADDCOLOR);

	// ����ł���v���C���l��
	m_nPlayerManualNum--;

	SAFE_RELEASE(m_pRimo[playerNum]);
	SAFE_RELEASE(m_pJoin2D[playerNum]);
	SAFE_RELEASE(m_pCursol2D[playerNum]);
}

//-----------------------------------------------------------------------------
// �����̏��҃A�j���[�V�����X�V
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateRimoAnimation(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		// ���������Ȃ���Δ�����
		if (!m_pRimo[i])	continue;

		if (m_nRimoTextureIndex[i] > m_nRimoTextureMax[i]){
			continue;
		}

		m_nRimoTextureCount[i]++;
		if (m_nRimoTextureCount[i] > PLAYER_ANIME_SPEED){
			m_nRimoTextureCount[i] = 0;

			m_nRimoTextureIndex[i]++;
			if (m_nRimoTextureIndex[i] > m_nRimoTextureMax[i]){
				m_nRimoTextureIndex[i] = PLAYER_STOP_TEXTURE_MIN;
				m_nRimoTextureMax[i] = PLAYER_STOP_TEXTURE_MAX;
			}
			m_pRimo[i]->SetIndex(m_nRimoTextureIndex[i]);
		}
	}

}

//-----------------------------------------------------------------------------
// ���͂ŎQ�킷��X�V
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateInputJoin(void){

	// �e�R���g���[���[�ŎQ�������� 0�͂��łɎQ�����Ă���
	if (CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 1)){
		Join(1);
	}
	if (CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 2)){
		Join(2);
	}
	if (CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 3)){
		Join(3);
	}
	// �f�o�b�O����
	if (CInputKeyboard::GetKeyboardTrigger(DIK_1))	Join(1);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_2))	Join(2);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_3))	Join(3);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_4))	UnJoin(1);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_5))	UnJoin(2);
	if (CInputKeyboard::GetKeyboardTrigger(DIK_6))	UnJoin(3);
}

//-----------------------------------------------------------------------------
// PUSH�̓_�ōX�V
//-----------------------------------------------------------------------------
void CCharaSelect::UpdatePushAnimation(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		// PUSH��������Δ�����
		if (!m_pPush2D[i])	continue;
		// PUSH������΃A�j���[�V����
		m_nPushFlashCount[i]++;
		if (m_nPushFlashCount[i] > CHARASELECT_PUSH_FLASH_INTERVAL){
			m_nPushFlashCount[i] = 0;
			m_bPushFlash[i] = !m_bPushFlash[i];
			m_pPush2DBG[i]->SetDrawFlag(m_bPushFlash[i]);
			if (m_bPush[i])	m_pPush2D[i]->SetDrawFlag(true);
		}
	}
}

//-----------------------------------------------------------------------------
// READY�̓_�ōX�V
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateReadyAnimation(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		// PUSH��������Δ�����
		if (!m_pReady2D[i])	continue;
		// PUSH������΃A�j���[�V����
		m_nReadyFlashCount[i]++;
		if (m_nReadyFlashCount[i] > CHARASELECT_PUSH_FLASH_INTERVAL){
			m_nReadyFlashCount[i] = 0;
			m_bReadyFlash[i] = !m_bReadyFlash[i];
			m_pReady2D[i]->SetDrawFlag(m_bReadyFlash[i]);
			if (m_bReady[i])	m_pReady2D[i]->SetDrawFlag(true);
		}
	}
}

//-----------------------------------------------------------------------------
// �J�[�\���̍X�V�@�A�j���[�V��������͂ňړ��Ȃ�
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateCursol(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		if (m_pCursol2D[i]){
			// �p�x�X�V
			if (m_bCursolFlg[i]){
				m_fCursolRad[i] += CHARASELECT_CURSOL_ADDRAD;
				if (m_fCursolRad[i] > CHARASELECT_CURSOL_RAD_MAX){
					m_fCursolRad[i] = CHARASELECT_CURSOL_RAD_MAX;
					m_bCursolFlg[i] = !m_bCursolFlg[i];
				}
			}
			else{
				m_fCursolRad[i] -= CHARASELECT_CURSOL_ADDRAD;
				if (m_fCursolRad[i] < 0){
					m_fCursolRad[i] = 0;
					m_bCursolFlg[i] = !m_bCursolFlg[i];
				}
			}
			// �p�x���Z�b�g
			m_pCursol2D[i]->SetRot(D3DXVECTOR3(0, 0, m_fCursolRad[i]));

			// ���L�[

			// ��������΂߈ړ�
			if (CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_LEFT_UP, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x -= CHARASELECT_CURSOL_VELO * 0.75f;
				pos.y -= CHARASELECT_CURSOL_VELO * 0.75f;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_LEFT_DOWN, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x -= CHARASELECT_CURSOL_VELO * 0.75f;
				pos.y += CHARASELECT_CURSOL_VELO * 0.75f;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_RIGHT_DOWN, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x += CHARASELECT_CURSOL_VELO * 0.75f;
				pos.y += CHARASELECT_CURSOL_VELO * 0.75f;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_RIGHT_UP, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x += CHARASELECT_CURSOL_VELO * 0.75f;
				pos.y -= CHARASELECT_CURSOL_VELO * 0.75f;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_W) ||
				CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_UP, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.y -= CHARASELECT_CURSOL_VELO;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_S) ||
				CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_DOWN, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.y += CHARASELECT_CURSOL_VELO;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_A) ||
				CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_LEFT, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x -= CHARASELECT_CURSOL_VELO;
				m_pCursol2D[i]->SetPos(pos);
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_D) ||
				CInputGamePad::GetGamePadPress(CInputGamePad::LEFT_STICK_RIGHT, i)){
				D3DXVECTOR3 pos = m_pCursol2D[i]->GetPos();
				pos.x += CHARASELECT_CURSOL_VELO;
				m_pCursol2D[i]->SetPos(pos);
			}
		}
		// ����{�^������������
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, i)){
			
			// �Q�����Ă��ăJ�[�\��������ꍇ
			if (m_pCursol2D[i]){
				// �J�[�\���ƃ{�^���̓����蔻��
				ButtonNumber hitButtonNum = CollisionButton(i, m_pCursol2D[i]->GetPos());

				// ���������{�^��
				if (hitButtonNum == BUTTON_READY){
					// ����������Ԃ̐؂�ւ��i�Q�킵�Ă���v���C���̂݁j
					if (m_bPush[i]){
						if (m_bReady[i]){
							UnReady(i);
						}
						else{
							Ready(i);
							CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_ZYUNBIKANRYOU);
							continue;
						}
					}
				}
				// �R���t�B�O�{�^��
				else if (hitButtonNum == BUTTON_CONFIG){
					// �t�F�[�Y�ړ����Ă��Ȃ����
					if (!m_bNextPhaseOnece){
						// ���Q�킵�Ă���l����ۑ�
						CManager::SetJoinNum(m_nPlayerManualNum);
						// �t�F�[�h�A�E�g�J�n
						m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
						// �R���t�B�O�w
						m_pManager->SetNextPhase(MODE_PHASE_OPTION);

						CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
					}
				}
				// �L�����N�^��I���{�^��
				else if (hitButtonNum == BUTTON_CHARA){
					// �Q�킷�邵�Ȃ��̐؂�ւ�
					if (m_bPush[i]){
						UnJoin(i);
						continue;
					}
				}
			}
			// �s�Q���ŃJ�[�\�����Ȃ��ꍇ
			else{
				if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
					CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, i)){
					Join(i);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// ���͂Ńe�N�X�`���A�j���[�V����
//-----------------------------------------------------------------------------
void CCharaSelect::UpdateInput(void){

	for (int i = 0; i < PLAYER_MAX; i++){

		// �J�[�\���������̎��ȊO�͔�����
		if (m_nCursol[i] != 0){
			continue;
		}
		// �����̊G��������Δ�����
		if (!m_pRimo[i]){
			continue;
		}

		// L�L�[�@��
		if (CInputKeyboard::GetKeyboardTrigger(DIK_4) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_L, i)){
			// �ʏ�
			if (m_PlayerMode[i] == PLAYER_MODE_NONE){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_THREAD_FRONT_MIN, PLAYER_TEXTURE_THREAD_FRONT_MAX);
			}
			// �U��
			else if (m_PlayerMode[i] == PLAYER_MODE_ATTACK){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_THREAD_FRONT_MIN, PLAYER_TEXTURE_THREAD_FRONT_MAX);
			}
			// �ړ�
			else if (m_PlayerMode[i] == PLAYER_MODE_SPEED){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_THREAD_FRONT_MIN, PLAYER_TEXTURE_THREAD_FRONT_MAX);
			}
			// �
			else if (m_PlayerMode[i] == PLAYER_MODE_TRAP){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_THREAD_FRONT_MIN, PLAYER_TEXTURE_THREAD_FRONT_MAX);
			}
			m_nRimoTextureIndex[i] = m_nRimoTextureMin[i];
			m_nRimoTextureCount[i] = 0;
		}
		// R�L�[�@�U��
		if (CInputKeyboard::GetKeyboardTrigger(DIK_5) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_R, i)){
			// �ʏ�
			if (m_PlayerMode[i] == PLAYER_MODE_NONE){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_ATTACK_FRONT_MIN, PLAYER_TEXTURE_ATTACK_FRONT_MAX);
			}
			// �U��
			else if (m_PlayerMode[i] == PLAYER_MODE_ATTACK){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_ATTACK_FRONT_MIN, PLAYER_TEXTURE_ATTACK_FRONT_MAX);
			}
			// �ړ�
			else if (m_PlayerMode[i] == PLAYER_MODE_SPEED){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_ATTACK_FRONT_MIN, PLAYER_TEXTURE_ATTACK_FRONT_MAX);
			}
			// �
			else if (m_PlayerMode[i] == PLAYER_MODE_TRAP){
				SetRimoTextureIndex(i, PLAYER_TEXTURE_ATTACK_FRONT_MIN, PLAYER_TEXTURE_ATTACK_FRONT_MAX);
			}
			m_nRimoTextureIndex[i] = m_nRimoTextureMin[i];
			m_nRimoTextureCount[i] = 0;
		}
	}
}

//-----------------------------------------------------------------------------
// ������pos�Ɠ������Ă���{�^���ԍ���Ԃ�
//-----------------------------------------------------------------------------
CCharaSelect::ButtonNumber CCharaSelect::CollisionButton(int playerNum, D3DXVECTOR3 vCursolPos){
	// �f�t�H���g
	CCharaSelect::ButtonNumber num = BUTTON_NONE;

	// �w��ɓ����蔻��𒲐�
	vCursolPos += CHARASELECT_CURSOL_COLI_DIST;

	// �L�����{�^���Ƃ̓����蔻��
	if (
		((CHARASELECT_PLAYER_BG_POS[playerNum].x - CHARASELECT_PLAYER_BG_SIZE_HALF.x) < vCursolPos.x) &&
		((CHARASELECT_PLAYER_BG_POS[playerNum].x + CHARASELECT_PLAYER_BG_SIZE_HALF.x) > vCursolPos.x) &&
		((CHARASELECT_PLAYER_BG_POS[playerNum].y - CHARASELECT_PLAYER_BG_SIZE_HALF.y) < vCursolPos.y) &&
		((CHARASELECT_PLAYER_BG_POS[playerNum].y + CHARASELECT_PLAYER_BG_SIZE_HALF.y) > vCursolPos.y)){
		num = BUTTON_CHARA;
	}
	// ���������{�^���Ƃ̓����蔻��
	else if (
		((CHARASELECT_START_POS.x - CHARASELECT_BUTTON_SIZE_HALF.x) < vCursolPos.x) &&
		((CHARASELECT_START_POS.x + CHARASELECT_BUTTON_SIZE_HALF.x) > vCursolPos.x) &&
		((CHARASELECT_START_POS.y - CHARASELECT_BUTTON_SIZE_HALF.y) < vCursolPos.y) &&
		((CHARASELECT_START_POS.y + CHARASELECT_BUTTON_SIZE_HALF.y) > vCursolPos.y)){
		num = BUTTON_READY;
	}
	// �R���t�B�O�{�^���Ƃ̓����蔻��
	else if (
		(CHARASELECT_CONFIG_POS.x - CHARASELECT_BUTTON_SIZE_HALF.x < vCursolPos.x) &&
		(CHARASELECT_CONFIG_POS.x + CHARASELECT_BUTTON_SIZE_HALF.x > vCursolPos.x) &&
		(CHARASELECT_CONFIG_POS.y - CHARASELECT_BUTTON_SIZE_HALF.y < vCursolPos.y) &&
		(CHARASELECT_CONFIG_POS.y + CHARASELECT_BUTTON_SIZE_HALF.y > vCursolPos.y)){
		num = BUTTON_CONFIG;
	}

	// �{�^���ԍ�
	return num;
}

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
void CCharaSelect::Ready(int playerNum){
	if (m_pReady2D[playerNum])
		m_pReady2D[playerNum]->SetDrawFlag(true);
	m_bReady[playerNum] = true;
	m_nPlayerReadyNum++;
}
//-----------------------------------------------------------------------------
// ������������Ȃ�
//-----------------------------------------------------------------------------
void CCharaSelect::UnReady(int playerNum){
	if (m_pReady2D[playerNum])
		m_pReady2D[playerNum]->SetDrawFlag(false);
	m_bReady[playerNum] = false;
	m_nPlayerReadyNum--;
}

//----EOF----