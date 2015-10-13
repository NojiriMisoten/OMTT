//=============================================================================
//
// CCountDown�N���X [CCountDown.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "CCountDown.h"
#include "../ANIMATION/CAnimation.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �����̐����ʒu(visible)�A�o���ʒu�A�傫���A�e�N�X�`���A�e�N�X�`���̕�����
static const D3DXVECTOR3 COUNTDOWN_FIGURE_CREATE_POS = D3DXVECTOR3(0, -100, 0);
static const D3DXVECTOR3 COUNTDOWN_FIGURE_APPEAR_POS = D3DXVECTOR3(SCREEN_WIDTH*0.5f, 0, 0);
static const float COUNTDOWN_FIGURE_WIDTH = 400;
static const float COUNTDOWN_FIGURE_HEIGHT = 150;
static const TEXTURE_TYPE COUNTDOWN_FIGURE_TEXTURE = TEXTURE_GAME_START;
static const int COUNTDOWN_FIGURE_TEXTURE_SEPALATE_X = 4;
static const int COUNTDOWN_FIGURE_TEXTURE_SEPALATE_Y = 1;
// �����̗�������W
static const float COUNTDOWN_FIGURE_FALL_POSY = SCREEN_HEIGHT * 0.5f;
// �����̃t�F�[�h�X�s�[�h
static const float COUNTDOWN_FIGURE_FADE_SPEED = 0.03f;

// �Q�[�����n�܂��Ă���J�E���g�_�E�����n�܂�܂ł̃J�E���g��
static const short TIME_START = 20;
// �����������Ă���t�F�[�h�A�E�g�����n�߂�Ԃ̃J�E���g��
static const short TIME_START_TO_FADE = 60;
// �t�F�[�h�A�E�g���n�߂Ă��玟�̐�����������Ԃ̃J�E���g��
static const short TIME_NEXT_COUNT = 20;
static const short COUNT_MAX = 5;
// �����������Ă��炻�̉���炷�܂ł̃J�E���g��
static const short TIME_START_TO_SOUND = 30;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCountDown::CCountDown(LPDIRECT3DDEVICE9 *pDevice, bool *bPlayerControl)
{
	m_pDevice = pDevice;
	m_pFigure = NULL;
	m_nCount = 0;
	m_bPlayerControl = bPlayerControl;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCountDown ::~CCountDown(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CCountDown::Init()
{
	m_pFigure = CAnimation::Create(
		m_pDevice,
		TYPE_PRIORITY_UI,
		COUNTDOWN_FIGURE_CREATE_POS,
		COUNTDOWN_FIGURE_WIDTH,
		COUNTDOWN_FIGURE_HEIGHT,
		COUNTDOWN_FIGURE_TEXTURE,
		COUNTDOWN_FIGURE_TEXTURE_SEPALATE_X,
		COUNTDOWN_FIGURE_TEXTURE_SEPALATE_Y);

	for (int i = COUNT_MAX - 1; i >= 0; i--){
		TIME[i] = TIME_START + (TIME_START + TIME_START_TO_FADE + TIME_NEXT_COUNT) * (3 - i);
	}
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CCountDown::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
bool CCountDown::Update(void){

	// �v���C�����R���g���[���ł���i�J�E���g�_�E�����I�����Ă����牽�����Ȃ��j
	if (*m_bPlayerControl){
		return true;
	}

	m_nCount++;

	for (int i = COUNT_MAX - 1; i >= 0; i--){
		if (m_nCount == TIME[i]){
			m_pFigure->SetIndex(i);
			m_pFigure->SetNonFall(COUNTDOWN_FIGURE_CREATE_POS);
			m_pFigure->SetNonFadeOut();
			m_pFigure->SetFall(COUNTDOWN_FIGURE_APPEAR_POS, COUNTDOWN_FIGURE_FALL_POSY);
		}
		else if (m_nCount == TIME[i] + TIME_START_TO_SOUND){
			if (i == 0){
				CManager::PlaySoundA(SOUND_LABEL_SE_GAME_START);
			}
			else{
				CManager::PlaySoundA(SOUND_LABEL_SE_COUNTDOUN);
			}
		}
		else if (m_nCount == TIME[i] + TIME_START_TO_FADE){
			m_pFigure->SetFadeOut(1.0f, COUNTDOWN_FIGURE_FADE_SPEED);
			if (i == 0){
				*m_bPlayerControl = true;
			}
		}

	}
	return false;
}

//----EOF-------