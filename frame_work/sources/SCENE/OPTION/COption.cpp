//=============================================================================
//
// COption�N���X [COption.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "COption.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneNumber.h"
//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float		 OPTION_MENU_WIDTH = SCREEN_WIDTH * 0.25f * 0.9f;	// �I�v�V�������j���[�̕�
static const float		 OPTION_MENU_HEIGHT = SCREEN_HEIGHT * 0.8f;			// �I�v�V�������j���[�̍���

// �I�v�V�������j���[�̍��W
static const D3DXVECTOR3 OPTION_MENU_POS[CControllerManager::MAX_CONTROLLER_NUM] =
{
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.375f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.625f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.875f, SCREEN_HEIGHT * 0.5f, 0),
};
static const D3DXVECTOR3 OPTION_BG_POS((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.f);// �w�i�̍��W
static const float		 MOVE_BG_POS_X = 1.5f;								// �w�i�̓������x

// �L�[�R�[�h�\���̃x�[�X���W
static const D3DXVECTOR3 BASE_KEY_CORD_POS[CInputGamePad::PAD_KEY_MAX - 2] =
{
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f - OPTION_MENU_HEIGHT * 0.38f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f - OPTION_MENU_HEIGHT * 0.22f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f - OPTION_MENU_HEIGHT * 0.09f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f + OPTION_MENU_HEIGHT * 0.05f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f + OPTION_MENU_HEIGHT * 0.21f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f + OPTION_MENU_HEIGHT * 0.37f, 0),
};
static const float		KEY_CORD_HEIGHT = OPTION_MENU_HEIGHT / 16.f;	// �L�[�R�[�h�̍���
static const float		KEY_CORD_WIDTH = OPTION_MENU_WIDTH * 0.125f;	// �L�[�R�[�h�̕�

// �J�[�\���̃f�t�H���g���W
static const D3DXVECTOR3 DEFAULT_CURSOL_POS[CControllerManager::MAX_CONTROLLER_NUM] =
{
	D3DXVECTOR3(OPTION_MENU_POS[0].x, BASE_KEY_CORD_POS[0].y, 0),
	D3DXVECTOR3(OPTION_MENU_POS[1].x, BASE_KEY_CORD_POS[0].y, 0),
	D3DXVECTOR3(OPTION_MENU_POS[2].x, BASE_KEY_CORD_POS[0].y, 0),
	D3DXVECTOR3(OPTION_MENU_POS[3].x, BASE_KEY_CORD_POS[0].y, 0),
};
static const float MOV_POW_CURSOL_ALFA = D3DX_PI * 0.06f;
static const float DEFAULT_CURSOL_ALFA = D3DX_PI * 0.5f;

static const D3DXVECTOR3 DEFAULT_RETURN_LOGO_POS(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.95f, 0.f);	// �߂�A�C�R���̈ʒu
static const float RETURN_LOGO_WIDTH = KEY_CORD_WIDTH * 4.f;									// �߂�A�C�R���̕�
static const float RETURN_LOGO_HEIGHT = KEY_CORD_HEIGHT * 4.f;										// �߂�A�C�R���̍���

// �v���C���[�ԍ��̍��W
static const D3DXVECTOR3 DEFAULT_PLAYER_NUMBER_POS[CControllerManager::MAX_CONTROLLER_NUM] =
{
	D3DXVECTOR3(OPTION_MENU_POS[0].x, SCREEN_HEIGHT * 0.04f, 0),
	D3DXVECTOR3(OPTION_MENU_POS[1].x, SCREEN_HEIGHT * 0.04f, 0),
	D3DXVECTOR3(OPTION_MENU_POS[2].x, SCREEN_HEIGHT * 0.04f, 0),
	D3DXVECTOR3(OPTION_MENU_POS[3].x, SCREEN_HEIGHT * 0.04f, 0),
};
// �v���C���[�ԍ���UV���W
static const UV_INDEX DEFAULT_PLAYER_NUMBER_UV[CControllerManager::MAX_CONTROLLER_NUM] =
{
	{ 0.f, 0.5f, 0.f, 0.5f},
	{ 0.5f, 1.f, 0.f, 0.5f},
	{ 0.f, 0.5f, 0.5f, 1.f},
	{ 0.5f, 1.f, 0.5f, 1.f},
};
static const float PLAYER_NUMBER_WIDTH = KEY_CORD_WIDTH * 2.f;				// �v���C���[�ԍ��̕�
static const float PLAYER_NUMBER_HEIGHT = KEY_CORD_HEIGHT * 2.f;				// �v���C���[�ԍ��̍���
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
COption::COption(void)
{
	m_mode = OPTION_MODE_KEYCONFIG;
	m_setButton = CInputGamePad::PAD_KEY_CHANGE_NORMAL;
	m_nControllerID = 0;
	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		m_pOptionMenu[i] = NULL;
		m_pCursol[i] = NULL;
		m_nSelectCounter[i] = 0;
	}
	m_pOptionBG = NULL;
	for (int i = 0; i < MOVE_BG_NUM; ++i)
	{
		m_pOptionBGMove[i] = NULL;
	}

	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		for (int j = 0; j < CInputGamePad::PAD_KEY_MAX; ++j)
		{
			m_KeyNumber[i][j].DigitOne = NULL;
			m_KeyNumber[i][j].DigitTen = NULL;
		}
	}
	m_pReturn = NULL;
	m_fCursolAlfaRad = DEFAULT_CURSOL_ALFA;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
COption ::~COption(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void COption::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ���[�h�̏�����
	m_mode = OPTION_MODE_KEYCONFIG;

	// �w�i
	m_pOptionBG = CScene2D::Create(pDevice, OPTION_BG_POS, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, TEXTURE_OPTION_BG);
	for (int i = 0; i < MOVE_BG_NUM; ++i)
	{
		m_pOptionBGMove[i] = CScene2D::Create(pDevice, OPTION_BG_POS, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, TEXTURE_BG_1);
		m_pOptionBGMove[i]->SetPos(OPTION_BG_POS.x + (float)SCREEN_WIDTH * (float)i, OPTION_BG_POS.y, OPTION_BG_POS.z);
		m_pOptionBGMove[i]->SetColorPolygon(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
	}

	// ���j���[�̕\��
	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		m_pOptionMenu[i] = CScene2D::Create(pDevice, OPTION_MENU_POS[i], OPTION_MENU_WIDTH, OPTION_MENU_HEIGHT, TEXTURE_OPTION_MENU);
	}

	// �L�[�R�[�h�\��
	int buttonLoopMax = CInputGamePad::PAD_KEY_MAX - 2;
	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		for (int j = 0; j < buttonLoopMax; ++j)
		{
			CInputGamePad::PAD_BUTTON button = (CInputGamePad::PAD_BUTTON)j;
			int keyCord = CControllerManager::GetKeyCord(i, button);
			int ten = 0;
			int one = 0;
			CuclDigit(&ten, &one, keyCord);
			D3DXVECTOR3 pos = BASE_KEY_CORD_POS[j];
			pos.x += i * (OPTION_MENU_WIDTH + KEY_CORD_WIDTH * 0.9f);
			m_KeyNumber[i][j].DigitTen = CSceneNumber::Create(pDevice, pos, KEY_CORD_WIDTH, KEY_CORD_HEIGHT, ten, TEXTURE_NUMBER_WHITE);
			pos.x += KEY_CORD_WIDTH;
			m_KeyNumber[i][j].DigitOne = CSceneNumber::Create(pDevice, pos, KEY_CORD_WIDTH, KEY_CORD_HEIGHT, one, TEXTURE_NUMBER_WHITE);
		}
	}

	// �߂�\��
	m_pReturn = CScene2D::Create(pDevice, DEFAULT_RETURN_LOGO_POS, RETURN_LOGO_WIDTH, RETURN_LOGO_HEIGHT, TEXTURE_RETURN);

	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		// �J�[�\���\��
		m_pCursol[i] = CScene2D::Create(pDevice, DEFAULT_CURSOL_POS[i], OPTION_MENU_WIDTH * 0.95f, KEY_CORD_HEIGHT * 1.6f, TEXTURE_CURSOL, TYPE_PRIORITY_GOAL);
		m_pCursol[i]->SetColorPolygon(D3DXCOLOR(1.f, 1.f, 1.f, sinf(m_fCursolAlfaRad)));

		// �v���C���[�ԍ��̕\��
		CScene2D* p = NULL;
		p = CScene2D::Create(pDevice, DEFAULT_PLAYER_NUMBER_POS[i], PLAYER_NUMBER_WIDTH, PLAYER_NUMBER_HEIGHT, TEXTURE_PLAYER_NUM, TYPE_PRIORITY_GOAL);
		UV_INDEX uv = DEFAULT_PLAYER_NUMBER_UV[i];
		p->SetUV(&uv);
	}
}

//*****************************************************************************
// �I��
//*****************************************************************************
void COption::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void COption::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	// �w�i�̍X�V
	for (int i = 0; i < MOVE_BG_NUM; ++i)
	{
		D3DXVECTOR3 pos = m_pOptionBGMove[i]->GetPos();
		pos.x -= MOVE_BG_POS_X;
		if (pos.x < -SCREEN_WIDTH * 0.5f)
		{
			pos.x = SCREEN_WIDTH * 1.5f;
		}
		m_pOptionBGMove[i]->SetPos(pos);
	}

	// �J�[�\���̍X�V
	UpdateCursol();

	// ���̃V�[���ɍs���Ȃ�
	if (m_mode == OPTION_MODE_NEXT_SCENE)
	{
		// �L�[�R���t�B�O���̃Z�[�u
		CControllerManager::SaveKeyCofingFile();

		// �L�[�R���t�B�O���̍X�V
		CControllerManager::UpdateKeyCord();

		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �L�����I���֖߂�
		m_pManager->SetNextPhase(MODE_PHASE_CHARA_SELECT);
	}

	// �f�o�b�O��ENTER�ŃL�����I����
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN)){
		m_mode = OPTION_MODE_NEXT_SCENE;
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
COption* COption::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	COption* pTitle = NULL;

	// �쐬
	pTitle = new COption;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
// �L�[�R���t�B�O
//*****************************************************************************
void COption::KeyConfig(int padID, CInputGamePad::PAD_BUTTON button)
{
	int pCord = -1;
	if (CInputGamePad::CheckConectPad(padID))
	{
		if (CInputGamePad::CheckTriggerAnyKey(padID, &pCord))
		{
			// �L�[�R�[�h�Z�b�g
			CControllerManager::SetKeyCord(padID, button, pCord);

			// �L�[�R�[�h�ύX
			int keyCord = pCord;
			int ten = 0;
			int one = 0;
			CuclDigit(&ten, &one, keyCord);
			m_KeyNumber[padID][button].DigitTen->SetNumber(ten);
			m_KeyNumber[padID][button].DigitOne->SetNumber(one);

			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);

		}
	}
}

//=================================================================
// �����o�֐�
//=================================================================
void COption::CuclDigit(int* ten, int* one, int org)
{
	if (org > 99)
	{
		org = 99;
	}
	if (org < 0)
	{
		org = 0;
	}
	*ten = org / 10;
	*one = org - (*ten * 10);

}

//=================================================================
// �J�[�\���X�V�֐�
//=================================================================
void COption::UpdateCursol(void)
{
	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		if (!CInputGamePad::CheckConectPad(i))
		{
			continue;
		}
		if (CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_UP, i))
		{
			m_nSelectCounter[i]--;
			if (m_nSelectCounter[i] < (int)CInputGamePad::PAD_KEY_CHANGE_NORMAL)
			{
				m_nSelectCounter[i] = (int)CInputGamePad::PAD_KEY_START;
			}
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}
		else if (CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_DOWN, i))
		{
			m_nSelectCounter[i]++;
			if (m_nSelectCounter[i] > (int)CInputGamePad::PAD_KEY_START)
			{
				m_nSelectCounter[i] = (int)CInputGamePad::PAD_KEY_CHANGE_NORMAL;
			}
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}

		// �߂�I�𒆂Ȃ�
		if (m_nSelectCounter[i] == (int)CInputGamePad::PAD_KEY_START)
		{
			D3DXVECTOR3 pos(DEFAULT_RETURN_LOGO_POS);
			m_pCursol[i]->SetPos(pos);
			m_pCursol[i]->SetWidth(RETURN_LOGO_WIDTH);
			int pCord = -1;
			
			if (CInputGamePad::CheckTriggerAnyKey(i, &pCord))
			{
				// �߂鉟������
				m_mode = OPTION_MODE_NEXT_SCENE;
				CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
			}
			
		}

		// ����ȊO�Ȃ�
		else
		{
			D3DXVECTOR3 pos = m_pCursol[i]->GetPos();
			pos.y = BASE_KEY_CORD_POS[m_nSelectCounter[i]].y;
			pos.x = DEFAULT_CURSOL_POS[i].x;
			m_pCursol[i]->SetPos(pos);
			m_pCursol[i]->SetWidth(OPTION_MENU_WIDTH * 0.95f);

			// �L�[�R���t�B�O
			KeyConfig(i, (CInputGamePad::PAD_BUTTON)m_nSelectCounter[i]);
		}
	}
}

//----EOF----