//=============================================================================
//
// CResult�N���X [CResult.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CResult.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneAnime.h"
#include "../GAME/PLAYER/CPlayer.h"
#include "../GAME/PLAYER/AI/mersenne_twister.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// ���� �s��
enum PlayerResult{
	WIN,
	LOSE,
	DRAW,
	PLAYER_RESULT_MAX
};
// �����A�j���[�V�����̑傫��
static const D3DXVECTOR2 RESULT_RIMO_SIZE[PLAYER_RESULT_MAX] = {
	// ���҂̃T�C�Y
	D3DXVECTOR2(150, 300),
	// �s�҂̃T�C�Y
	D3DXVECTOR2(100, 200),
	// ���������̃T�C�Y
	D3DXVECTOR2(75, 150),
};
// �����A�j���[�V�����̍��W
static const D3DXVECTOR3 RESULT_RIMO_POS[PLAYER_MAX] = {
	// ���҂̈ʒu
	D3DXVECTOR3(250, 400, 0),
	D3DXVECTOR3(500, 400, 0),
	D3DXVECTOR3(750, 400, 0),
	D3DXVECTOR3(1000, 400, 0),
};
// ���j���[�̍��W
static const D3DXVECTOR3 RESULT_MENU_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);
// ���j���[�̑傫��
static const D3DXVECTOR2 RESULT_MENU_SIZE = D3DXVECTOR2(500, 600);
// PUSH2D�̑傫��
static const D3DXVECTOR2 RESULT_PUSH_SIZE = D3DXVECTOR2(180, 80);
// PUSH2D�̒����ʒu
static const D3DXVECTOR3 RESULT_PUSH_ADDPOS = D3DXVECTOR3(0, 250, 0);
// �v���C���̃A�j���[�V�����̃X�s�[�h
static const int RESULT_RIMO_ANIME_SPEED = 10;
// �v���C���̕����A�j���[�V�����̃C���f�b�N�X
static const int RESULT_RIMO_LOSE_INDEX_MAX = PLAYER_TEXTURE_THREAD_FRONT_MAX;
static const int RESULT_RIMO_LOSE_INDEX_MIN = PLAYER_TEXTURE_THREAD_FRONT_MAX;
// �v���C���̏����A�j���[�V�����̍ŏ��C���f�b�N�X
static const int RESULT_RIMO_WIN_INDEX_MAX = 9;
static const int RESULT_RIMO_WIN_INDEX_MIN = 6;
// �����ło�t�r�g���o��܂ł̃J�E���g
static const int RESULT_PUSH_INTERVAL = 100;
// PUSH�̓_�ŊԊu
static const int RESULT_PUSH_FLASH_INTERVAL = 5;
// ���j���[�̃e�N�X�`���@�J�[�\���ԍ�����
static const TEXTURE_TYPE RESULT_MENU_TEXTURE[] = {
	TEXTURE_RESULT_MENU_BACK_0, TEXTURE_RESULT_MENU_BACK_1, TEXTURE_RESULT_MENU_BACK_2
};
static const TEXTURE_TYPE RESULT_MENU_MOJI_TEXTURE[] = {
	TEXTURE_RESULT_MENU_MOJI_0, TEXTURE_RESULT_MENU_MOJI_1, TEXTURE_RESULT_MENU_MOJI_2
};
// ���j���[�̕����A�j���[�V�����̊g��X�s�[�h
static const D3DXVECTOR2 RESULT_MENU_MOJI_ADDSIZE = D3DXVECTOR2(2.5f, 2.5f);
static const float RESULT_MENU_MOJI_SIZE_MAX = 550;
static const float RESULT_MENU_MOJI_SIZE_MIN = 480;
// ���j���[�����̍��W
static const D3DXVECTOR3 RESULT_MENU_MOJI_POS[RESULT_MENU_MAX] = {
	D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 180),	// ���g���C
	D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 360),	// �L�����Z���N�g
	D3DXVECTOR2(SCREEN_WIDTH * 0.5f, 550),	// �X�e�[�W�Z���N�g
};

// ���S�̑傫��
static const D3DXVECTOR2 RESULT_LOGO_SIZE = D3DXVECTOR2(800, 150);
// ���S�̍��W
static const D3DXVECTOR3 RESULT_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 75, 0);
// ���҂ɓ��Ă郉�C�g�̑傫��
static const D3DXVECTOR2 RESULT_LIGHT_SIZE = D3DXVECTOR2(320, 640);
// ���҂ɓ��Ă郉�C�g�̍��W
static const D3DXVECTOR3 RESULT_LIGHT_POS[PLAYER_MAX] = {
	D3DXVECTOR3(250, 350, 0),
	D3DXVECTOR3(500, 350, 0),
	D3DXVECTOR3(750, 350, 0),
	D3DXVECTOR3(1000, 350, 0),
};
// ���邭�郉�C�g�̑傫��
static const float RESULT_CIRCLE_LIGHT_SIZE_MIN = 25;
static const float RESULT_CIRCLE_LIGHT_SIZE_MAX = 50;
// ���邭�郉�C�g�̑���
static const float RESULT_CIRCLE_LIGHT_VELO_MIN = -3;
static const float RESULT_CIRCLE_LIGHT_VELO_MAX = 3;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CResult::CResult(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CResult ::~CResult(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CResult::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �ϐ��̏�����
	m_nWinPlayerNum = 0;
	m_nCount = 0;
	m_pMenuBack = NULL;
	m_nMenuCursol = 0;
	m_vMojiSize = RESULT_MENU_SIZE;
	m_bMojiScale = true;
	for (int i = 0; i < PLAYER_MAX; i++){
		m_nTextureIndex[i] = 0;
		m_nTextureCount[i] = 0;
		m_pPlayerAnime[i] = NULL;
		m_pPush2D[i] = NULL;
		m_bPush[i] = false;
		m_nPushFlashCount[i] = 0;
		m_bPushFlash[i] = true;
	}
	for (int i = 0; i < RESULT_MENU_MAX; i++){
		m_pMenuMoji[i] = NULL;
	}

	// �������v���C���ԍ��擾
	m_nWinPlayerNum = CManager::GetWinPlayerNum();

	// CPU�l�����擾
	m_nCpuNum = PLAYER_MAX - CManager::GetPlayerManualNum();

	// �w�i
	InitializeBG();

	// �����̏��҃A�j���[�V����������
	InitializeWinAnimation();

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// �a�f�l�Đ�
	CManager::PlaySoundA(SOUND_LABEL_ZINGLE_RESULT);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CResult::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CResult::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	// �A�j���̍X�V
	UpdateWinAnimation();

	// �v�b�V�����ł�܂ŃJ�E���g
	if (m_nCount < RESULT_PUSH_INTERVAL){
		m_nCount++;
	}
	// �v�b�V�����ł�
	else if (m_nCount == RESULT_PUSH_INTERVAL){
		// �������x�ƂƂ���Ȃ�
		m_nCount++;
		// PUSH�Q�c������
		for (int i = 0; i < PLAYER_MAX; i++){
			m_pPush2D[i] = CScene2D::Create(m_pD3DDevice,
				RESULT_RIMO_POS[i] + RESULT_PUSH_ADDPOS,
				RESULT_PUSH_SIZE.x, RESULT_PUSH_SIZE.y,
				TEXTURE_RESULT_PUSH, TYPE_PRIORITY_THREAD_OF_FOOTHOLD);
			// �v���C���̐F�ɍ��킹��
			m_pPush2D[i]->SetColorPolygon(PLAYER_COLOR[i]);
		}
		// CPU����������PUSH������
		if (m_nCpuNum > 0){
			for (int i = 0; i < m_nCpuNum; i++){
				m_bPush[PLAYER_MAX - 1 - i] = true;
			}
		}
	}
	// �v�b�V�����łĂ�
	else{
		// �_�ŃA�j���[�V����
		for (int i = 0; i < PLAYER_MAX; i++){
			m_nPushFlashCount[i]++;
			if (m_nPushFlashCount[i] > RESULT_PUSH_FLASH_INTERVAL){
				m_nPushFlashCount[i] = 0;
				m_bPushFlash[i] = !m_bPushFlash[i];
				m_pPush2D[i]->SetDrawFlag(m_bPushFlash[i]);
				if (m_bPush[i])	m_pPush2D[i]->SetDrawFlag(true);
			}
		}

		// ���͎󂯎�
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0)){
			m_bPush[0] = true;
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 1)){
			m_bPush[1] = true;
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 2)){
			m_bPush[2] = true;
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 3)){
			m_bPush[3] = true;
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}

		// ���ׂĂo�t�r�g���ꂽ + ���j���[���o�Ă��Ȃ���ԁi��񂾂��ʂ�悤�Ɂj
		if ((m_bPush[0] && m_bPush[1] && m_bPush[2] && m_bPush[3]) && !m_pMenuBack){
			m_pMenuBack = CScene2D::Create(m_pD3DDevice,
				RESULT_MENU_POS,
				RESULT_MENU_SIZE.x, RESULT_MENU_SIZE.y,
				RESULT_MENU_TEXTURE[m_nMenuCursol], TYPE_PRIORITY_UI);
			// ���j���[�̕���
			for (int n = 0; n < RESULT_MENU_MAX; n++){
				m_pMenuMoji[n] = CScene2D::Create(m_pD3DDevice,
					RESULT_MENU_MOJI_POS[n],
					RESULT_MENU_SIZE.x, RESULT_MENU_SIZE.y,
					RESULT_MENU_MOJI_TEXTURE[n], TYPE_PRIORITY_PAUSE);
			}

			// ���̑I�����j���[���o�Ă����Ԃ̕���ɘA���t���[���Œʂ�Ȃ��悤�ɒE�o
			return;
		}
	}

	// �I�����j���[���o�Ă�����
	if (m_pMenuBack){
		// �I�𒆂̕������g��A�j���[�V����
		if (m_bMojiScale){
			m_vMojiSize += RESULT_MENU_MOJI_ADDSIZE;
		}
		else{
			m_vMojiSize -= RESULT_MENU_MOJI_ADDSIZE;
		}
		if (m_vMojiSize.x > RESULT_MENU_MOJI_SIZE_MAX){
			m_bMojiScale = false;
		}
		else if (m_vMojiSize.x < RESULT_MENU_MOJI_SIZE_MIN){
			m_bMojiScale = true;
		}
		m_pMenuMoji[m_nMenuCursol]->SetWidth(m_vMojiSize.x);
		m_pMenuMoji[m_nMenuCursol]->SetHeight(m_vMojiSize.y);

		// ���L�[
		if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_UP, 0)){
			m_nMenuCursol--;
			if (m_nMenuCursol < 0){
				m_nMenuCursol = RESULT_MENU_MAX - 1;
			}
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_DOWN, 0)){
			m_nMenuCursol++;
			if (m_nMenuCursol >= RESULT_MENU_MAX){
				m_nMenuCursol = 0;
			}
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}
		// ���̓��͂���������
		if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
			CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_UP, 0) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_DOWN, 0)){
			// ���j���[�̃e�N�X�`����ύX
			m_pMenuBack->ChangeTexture(RESULT_MENU_TEXTURE[m_nMenuCursol]);
			// ���j���[�����̑傫�����Z�b�g
			m_pMenuMoji[m_nMenuCursol]->SetHeight(RESULT_MENU_SIZE.y);
			m_pMenuMoji[m_nMenuCursol]->SetWidth(RESULT_MENU_SIZE.x);
			m_bMojiScale = true;
			m_vMojiSize = RESULT_MENU_SIZE;
		}
		// ����L�[
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
			CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0)){
			// �t�F�[�h�A�E�g�J�n
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
			// ���ꂼ��̃V�[���֔��
			if (m_nMenuCursol == RESULT_MENU_RETRY){
				m_pManager->SetNextPhase(MODE_PHASE_GAME);
			}
			else if (m_nMenuCursol == RESULT_MENU_CHARA_SELECT){
				m_pManager->SetNextPhase(MODE_PHASE_CHARA_SELECT);
			}
			else
				m_pManager->SetNextPhase(MODE_PHASE_STAGE_SELECT);
			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
		}
	}
	// ���邭�郉�C�g�̂�����
	for (int i = 0; i < RESULT_CIRCLE_LIGHT_MAX; i++){
		// �ړ�
		D3DXVECTOR3 pos = m_pCircleLight[i]->GetPos();
		pos.x += m_vCircleVelo[i].x;
		pos.y += m_vCircleVelo[i].y;
		m_pCircleLight[i]->SetPos(pos);
		// �ړ��ʔ��]
		if ((pos.y - m_vCircleSizeHalf[i] < 0) ||
			(pos.y + m_vCircleSizeHalf[i] > SCREEN_HEIGHT)){
			m_vCircleVelo[i].y *= -1.0f;
		}
		if ((pos.x - m_vCircleSizeHalf[i] < 0) ||
			(pos.x + m_vCircleSizeHalf[i] > SCREEN_WIDTH)){
			m_vCircleVelo[i].x *= -1.0f;
		}
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CResult* CResult::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CResult* pTitle = NULL;

	// �쐬
	pTitle = new CResult;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//-----------------------------------------------------------------------------
// �����̏��҃A�j���[�V����������
//-----------------------------------------------------------------------------
void CResult::InitializeWinAnimation(void){

	// ���������̃A�j��
	if (m_nWinPlayerNum == -1){
		for (int i = 0; i < PLAYER_MAX; i++){
			m_pPlayerAnime[i] = CSceneAnime::Create(m_pD3DDevice,
				RESULT_RIMO_POS[i],
				RESULT_RIMO_SIZE[DRAW].x, RESULT_RIMO_SIZE[DRAW].y,
				PLAYER_TEXTURE[i], PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
				RESULT_RIMO_ANIME_SPEED, -1, TYPE_PRIORITY_PLAYER);
		}

	}
	// �N�����������A�j��
	else{
		for (int i = 0; i < PLAYER_MAX; i++){
			// �������l�̃A�j��
			if (i == m_nWinPlayerNum){
				m_pPlayerAnime[i] = CSceneAnime::Create(m_pD3DDevice,
					RESULT_RIMO_POS[i],
					RESULT_RIMO_SIZE[WIN].x, RESULT_RIMO_SIZE[WIN].y,
					PLAYER_TEXTURE[i], PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
					RESULT_RIMO_ANIME_SPEED, -1, TYPE_PRIORITY_PLAYER);
			}
			// �������l�̃A�j��
			else{
				m_pPlayerAnime[i] = CSceneAnime::Create(m_pD3DDevice,
					RESULT_RIMO_POS[i],
					RESULT_RIMO_SIZE[LOSE].x, RESULT_RIMO_SIZE[LOSE].y,
					PLAYER_TEXTURE[i], PLAYER_WALK_TEXTURE_SEP_X, PLAYER_WALK_TEXTURE_SEP_Y,
					RESULT_RIMO_ANIME_SPEED, -1, TYPE_PRIORITY_PLAYER);
			}
		}
	}
	// �����ōX�V���Ȃ�
	for (int i = 0; i < PLAYER_MAX; i++){
		m_pPlayerAnime[i]->SetAutoUpdate(false);
		// �������l�̃C���f�b�N�X
		if (i == m_nWinPlayerNum){
			m_nTextureIndex[i] = RESULT_RIMO_WIN_INDEX_MIN;
		}
		// �������l�̃C���f�b�N�X
		else{
			m_nTextureIndex[i] = RESULT_RIMO_LOSE_INDEX_MIN;
		}
		m_pPlayerAnime[i]->SetIndex(m_nTextureIndex[i]);
	}

}

//-----------------------------------------------------------------------------
// �����̏��҃A�j���[�V�����X�V
//-----------------------------------------------------------------------------
void CResult::UpdateWinAnimation(void){

	for (int i = 0; i < PLAYER_MAX; i++){
		m_nTextureCount[i]++;
		if (m_nTextureCount[i] > RESULT_RIMO_ANIME_SPEED){
			m_nTextureCount[i] = 0;

			m_nTextureIndex[i]++;
			// �������l�̃C���f�b�N�X
			if (i == m_nWinPlayerNum){
				if (m_nTextureIndex[i] > RESULT_RIMO_WIN_INDEX_MAX){
					m_nTextureIndex[i] = RESULT_RIMO_WIN_INDEX_MIN;
				}
			}
			// �������lor���������̃C���f�b�N�X
			else{
				if (m_nTextureIndex[i] > RESULT_RIMO_LOSE_INDEX_MAX){
					m_nTextureIndex[i] = RESULT_RIMO_LOSE_INDEX_MIN;
				}
			}
		}
		m_pPlayerAnime[i]->SetIndex(m_nTextureIndex[i]);
	}

}

//-----------------------------------------------------------------------------
// �w�i������
//-----------------------------------------------------------------------------
void CResult::InitializeBG(void){
	// �w�i
	m_pBG = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_RESULT, TYPE_PRIORITY_BG);

	// �L�����I�����Ă������S�\��
	m_pLogo = CScene2D::Create(m_pD3DDevice,
		RESULT_LOGO_POS,
		RESULT_LOGO_SIZE.x, RESULT_LOGO_SIZE.y,
		TEXTURE_RESULT_LOGO, TYPE_PRIORITY_GOAL);

	// ���邭�郉�C�g�̍쐬
	for (int i = 0; i < RESULT_CIRCLE_LIGHT_MAX; i++){
		// �T�C�Y����
		float size = mersenne_twister_float(RESULT_CIRCLE_LIGHT_SIZE_MIN, RESULT_CIRCLE_LIGHT_SIZE_MAX);
		m_pCircleLight[i] = CScene2D::Create(m_pD3DDevice,
			D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
			size, size,
			TEXTURE_RESULT_CIRCLE_LIGHT, TYPE_PRIORITY_EFFECT);
		// �����蔻��p�ɔ����̑傫����ۑ�
		m_vCircleSizeHalf[i] = size * 0.5f;
		// �ړ��ʌ���
		m_vCircleVelo[i].x = mersenne_twister_float(RESULT_CIRCLE_LIGHT_VELO_MIN, RESULT_CIRCLE_LIGHT_VELO_MAX);
		m_vCircleVelo[i].y = mersenne_twister_float(RESULT_CIRCLE_LIGHT_VELO_MIN, RESULT_CIRCLE_LIGHT_VELO_MAX);
		// �x������z���Ȃ���
		(m_vCircleVelo[i].x < 0) ? m_vCircleVelo[i].x -= 1.0f : m_vCircleVelo[i].x += 1.0f;
		(m_vCircleVelo[i].y < 0) ? m_vCircleVelo[i].y -= 1.0f : m_vCircleVelo[i].y += 1.0f;
		// �F��t����
		m_pCircleLight[i]->SetColorPolygon(
			PLAYER_COLOR[m_nWinPlayerNum] + D3DXCOLOR(
			mersenne_twister_float(0.2f, 0.7f),
			mersenne_twister_float(0.2f, 0.7f),
			mersenne_twister_float(0.2f, 0.7f),
			mersenne_twister_float(0.2f, 0.7f)));
	}

	// ���̂������������Ȃ炢��Ȃ�
	if (m_nWinPlayerNum == -1){
		return;
	}

	// ���҂ɓ��Ă郉�C�g
	m_pLight = CScene2D::Create(m_pD3DDevice,
		RESULT_LIGHT_POS[m_nWinPlayerNum],
		RESULT_LIGHT_SIZE.x, RESULT_LIGHT_SIZE.y,
		TEXTURE_RESULT_LIGHT, TYPE_PRIORITY_FIELD);

}

//----EOF----