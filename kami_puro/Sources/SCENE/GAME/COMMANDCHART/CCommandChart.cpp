//=============================================================================
//
//	CCommandChart�N���X�̏���(CCommandChart.cpp)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
// �C���N���[�h
#include "CCommandChart.h"
#include "CCommandName.h"
#include "../../../CONTROLLER/CControllerManager.h"
#include "../../../INPUT/CInputKeyboard.h"

//-----------------------------------------------------------------------------
//	�}�N����`
//-----------------------------------------------------------------------------
static const float UI_X_POSITION = 50.0f;		// UI�̏����\�����W(x���W)
static const float UI_Y_POSITION = 550.0f;		// UI�̏����\�����W(y���W)
static const float UI_X_POS_ADD = 30.0f;		// �P�\�����UI�̍��W�̕ω��̒l
static const float NEXT_UI_X_POS = 50.0f;		// ���ɓ��͂���R�}���h�̈�ԏ��UI�̍��W
static const float NEXT_UI_Y_POS = 550.0f;		// ���ɓ��͂���R�}���h�̈�ԏ��UI�̍��W
static const float NEXT_UI_X_POS_ADD = 30.0f;	// ���ɓ��͂���R�}���h��UI�̍��W�̕ω��̒l
static const float NEXT_UI_Y_POS_ADD = 30.0f;	// ���ɓ��͂���R�}���h��UI�̍��W�̕ω��̒l
static const float FADE_UI_OUT_POS_X_ID_1 = -50.0f;					//�t�F�[�h�A�E�g���̖ڕW���W������ID�P
static const float FADE_UI_OUT_POS_X_ID_2 = SCREEN_WIDTH + 50.0f;	//�t�F�[�h�A�E�g���̖ڕW���W������ID�Q
static const float BACK_POLYGON_X_SIZE = (COMMAND_POLYGON_WIDTH*MAX_COMMAND_KEEP) + (UI_X_POS_ADD*MAX_COMMAND_KEEP);	// �R�}���h�`���[�gUI�̔w��ɕ\������|���S����X�̃T�C�Y
static const float BACK_POLYGON_Y_SIZE = 150.0f;	// �R�}���h�`���[�gUI�̔w��ɕ\������|���S����Y�̃T�C�Y
static const int COMMAND_DEATH_COUNT = 60;		// �R�}���h�����܂ł̃J�E���g
static const int COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM = 0;	// ��U���̃R�}���h�̔z��ԍ�
static const int COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM = 1;	// ���U���̃R�}���h�̔z��ԍ�
static const int COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM = 2;	// ��U���̃R�}���h�̔z��ԍ�
static const D3DXVECTOR3 BACK_POLYGON_POS_1 = D3DXVECTOR3((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// �w��̃|���S���̍��WID1
static const D3DXVECTOR3 BACK_POLYGON_POS_2 = D3DXVECTOR3(SCREEN_WIDTH - ((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f)), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// �w��̃|���S���̍��WID2

// �R�}���h�̎��
// �`���b�v LU LU RU
static const BUTTON_TYPE COMMAND_BUTTON_CHOP[COMMAND_INPUT_NUM_SMALL] = { BUTTON_TYPE_3,
																   BUTTON_TYPE_3,
																   BUTTON_TYPE_1 };
static const COMMAND_INFO COMMAND_CHOP = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_CHOP, COMMAND_BUTTON_CHOP };

//�G���{�[	LU LD RU RD
static const BUTTON_TYPE COMMAND_BUTTON_ELBOW[COMMAND_INPUT_NUM_MIDDLE] = { BUTTON_TYPE_3,
																	 BUTTON_TYPE_4,
																	 BUTTON_TYPE_1,
																	 BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_ELBOW = { COMMAND_INPUT_NUM_MIDDLE, COMMAND_TYPE_ELBOW, COMMAND_BUTTON_ELBOW };

//�����A�b�g	LU LU LU RD LD
static const BUTTON_TYPE COMMAND_BUTTON_LARIAT[COMMAND_INPUT_NUM_LARGE] = { BUTTON_TYPE_3,
																	 BUTTON_TYPE_3,
																	 BUTTON_TYPE_3,
																	 BUTTON_TYPE_2,
																	 BUTTON_TYPE_4 };
static const COMMAND_INFO COMMAND_LARIAT = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_LARIAT, COMMAND_BUTTON_LARIAT };

//���[�����O�G���{�[	RU LU LD
static const BUTTON_TYPE COMMAND_BUTTON_ROLLING_ELBOW[COMMAND_INPUT_NUM_SMALL] = { BUTTON_TYPE_3,
																			BUTTON_TYPE_1,
																			BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_ROLLING_ELBOW = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_ROLLING_ELBOW, COMMAND_BUTTON_ROLLING_ELBOW };

//�t���C���O�G���{�[	RU RU LU RD
static const BUTTON_TYPE COMMAND_BUTTON_FLYING_ELBOW[COMMAND_INPUT_NUM_MIDDLE] = { BUTTON_TYPE_3,
																			BUTTON_TYPE_3, 
																			BUTTON_TYPE_1,
																			BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_FLYING_ELBOW = { COMMAND_INPUT_NUM_MIDDLE, COMMAND_TYPE_FLYING_ELBOW, COMMAND_BUTTON_FLYING_ELBOW };

//�h���b�v�L�b�N		RU LU RU RU RD
static const BUTTON_TYPE COMMAND_BUTTON_DROP_KICK[COMMAND_INPUT_NUM_LARGE] = { BUTTON_TYPE_3,
																		BUTTON_TYPE_1,
																		BUTTON_TYPE_3,
																		BUTTON_TYPE_3,
																		BUTTON_TYPE_4 };
static const COMMAND_INFO COMMAND_DROP_KICK = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_DROP_KICK, COMMAND_BUTTON_DROP_KICK };

//�r���^			LD LU RU
static const BUTTON_TYPE COMMAND_BUTTON_FACE_SLAPPING[COMMAND_INPUT_NUM_SMALL] = { BUTTON_TYPE_4,
																			BUTTON_TYPE_3,
																			BUTTON_TYPE_1 };
static const COMMAND_INFO COMMAND_FACE_SLAPPING = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_FACE_SLAPPING, COMMAND_BUTTON_FACE_SLAPPING };

//�o�b�N�h���b�v		LD LD RU RD
static const BUTTON_TYPE COMMAND_BUTTON_BACKDROP[COMMAND_INPUT_NUM_MIDDLE] = { BUTTON_TYPE_4,
																		BUTTON_TYPE_4,
																		BUTTON_TYPE_1,
																		BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_BACKDROP = { COMMAND_INPUT_NUM_MIDDLE, COMMAND_TYPE_BACKDROP, COMMAND_BUTTON_BACKDROP };

//�X�^�i�[		LD RU RU LU RD
static const BUTTON_TYPE COMMAND_BUTTON_STANER[COMMAND_INPUT_NUM_LARGE] = { BUTTON_TYPE_4,
																	 BUTTON_TYPE_1,
																	 BUTTON_TYPE_1,
																	 BUTTON_TYPE_3,
																	 BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_STANER = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_STANER, COMMAND_BUTTON_STANER };

//���[�v	RD
static const BUTTON_TYPE COMMAND_BUTTON_ROPE = BUTTON_TYPE_2;
static const COMMAND_INFO COMMAND_ROPE = { COMMAND_INPUT_NUM_MONO, COMMAND_TYPE_ROPE, (BUTTON_TYPE*)&COMMAND_BUTTON_ROPE };

//Finish�@LU + RU RU LU RU LU LD + RD
static const BUTTON_TYPE COMMAND_BUTTON_FINISHER[COMMAND_INPUT_NUM_FINISHER] = { BUTTON_TYPE_5,
																	      BUTTON_TYPE_1,
																	      BUTTON_TYPE_3,
																	      BUTTON_TYPE_1,
																	      BUTTON_TYPE_3,
																	      BUTTON_TYPE_6 };
static const COMMAND_INFO COMMAND_FINISHER = { COMMAND_INPUT_NUM_FINISHER, COMMAND_TYPE_FINISHER, COMMAND_BUTTON_FINISHER };


// �Z�̑傫���ł܂Ƃ߂��z��
// ���Z
// �`���b�v
// ���[�����O�G���{�[
// �r���^
static const COMMAND_INFO* COMMAND_SMALL_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_CHOP,
																			  &COMMAND_ROLLING_ELBOW,
																			  &COMMAND_FACE_SLAPPING};
// ���Z
// �G���{�[
// �t���C���O�G���{�[
// �o�b�N�h���b�v
static const COMMAND_INFO* COMMAND_MIDDLE_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_ELBOW,
																			  &COMMAND_FLYING_ELBOW,
																			  &COMMAND_BACKDROP };

// ��Z
// �����A�b�g
// �h���b�v�L�b�N
// �X�^�i�[
static const COMMAND_INFO* COMMAND_LARGE_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_LARIAT,
																			  &COMMAND_DROP_KICK,
																			  &COMMAND_STANER };

// ����L�[�J�n�R�}���h���܂Ƃ߂��z��
// �`���b�v
// �G���{�[
// �����A�b�g
static const COMMAND_INFO* COMMAND_LEFT_UP_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = {&COMMAND_CHOP,
																				&COMMAND_ELBOW,
																				&COMMAND_LARIAT};

// �E��L�[�J�n�R�}���h���܂Ƃ߂��z��
// ���[�����O�G���{�[
// �t���C���O�G���{�[
// �h���b�v�L�b�N
static const COMMAND_INFO* COMMAND_RIGHT_UP_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_ROLLING_ELBOW,
																				&COMMAND_FLYING_ELBOW,
																				&COMMAND_DROP_KICK};

// �����L�[�J�n�R�}���h���܂Ƃ߂��z��
// �r���^
// �o�b�N�h���b�v
// �X�^�i�[
static const COMMAND_INFO* COMMAND_LEFT_DOWN_TECHNIQUE_LIST[COMMAND_TYPE_NUM] = { &COMMAND_FACE_SLAPPING,
																					&COMMAND_BACKDROP,
																					&COMMAND_STANER };

// �S�Ă̋Z���܂Ƃ߂��z��
static const COMMAND_INFO* COMMAND_TECHNIQUE_LIST[COMMAND_TYPE_MAX] = { 
&COMMAND_CHOP,
&COMMAND_ELBOW,
&COMMAND_LARIAT,
&COMMAND_ROLLING_ELBOW,
&COMMAND_FLYING_ELBOW,
&COMMAND_DROP_KICK,
&COMMAND_FACE_SLAPPING,
&COMMAND_BACKDROP,
&COMMAND_STANER,
&COMMAND_ROPE,
&COMMAND_FINISHER};

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* pDevice, int nID)
{
	// �f�o�C�X�̃|�C���^��ۑ�
	m_pD3DDevice = pDevice;

	// ���͂��ꂽ�R�}���h�ێ��p�ϐ��̏�����
	m_aCommandKeep = BUTTON_TYPE_NONE;

	// �u���͂��ꂽ�R�}���h�ێ��p�z��v�Ɓu�\��������͂��ꂽ�R�}���hUI�̕ێ��v����������
	for (int i = 0; i < MAX_COMMAND_KEEP; i++)
	{
		// �\��������͂��ꂽ�R�}���hUI�̕ێ�
		//m_apCommandUI[i] = NULL;
	}

	// �u���ɓ��͂�����̃R�}���hUI�ێ��z��v�Ɓu�������̋Z���\���pUI�ێ��z��v��������
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		m_apNextCommandUI[i] = NULL;
		m_apCommandName[i] = NULL;
	}

	// ���g�̃v���C���[�ԍ���ݒ�
	m_MyID = nID;

	// �w��ɕ\������|���S���̍쐬
	m_pBackPolygon = new CScene2D(pDevice);
	if (m_MyID == MY_ID_1)
	{
		m_pBackPolygon->Init((D3DXVECTOR3 &)BACK_POLYGON_POS_1, BACK_POLYGON_X_SIZE, BACK_POLYGON_Y_SIZE, TEXTURE_MONO);
	}
	else if (m_MyID == MY_ID_2)
	{
		m_pBackPolygon->Init((D3DXVECTOR3 &)BACK_POLYGON_POS_2, BACK_POLYGON_X_SIZE, BACK_POLYGON_Y_SIZE, TEXTURE_MONO);
	}
	m_pBackPolygon->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	m_pBackPolygon->SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f));

	// �R�}���h���͏��̃|�C���^�̏�����
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		m_pCommandUI[i] = new COMMAND_UI_INFO;
		m_pCommandUI[i]->m_isInputButton = false;
		m_pCommandUI[i]->m_NextCommand = NULL;
		m_pCommandUI[i]->m_pUIType = NULL;
	}
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
CCommandChart::~CCommandChart()
{
}

//-----------------------------------------------------------------------------
//	��������
//-----------------------------------------------------------------------------
CCommandChart* CCommandChart::Create(LPDIRECT3DDEVICE9* pDevice, int nID)
{
	// �R�}���h�`���[�g�̐���
	CCommandChart* temp = new CCommandChart(pDevice, nID);
	// ���������R�}���h�`���[�g�̏�����
	temp->Init();

	return temp;
}

//-----------------------------------------------------------------------------
//	����������
//-----------------------------------------------------------------------------
void CCommandChart::Init(void)
{
	// �ێ����̃R�}���h���̏�����
	m_nKeepCommandNum = 0;

	// ���͌��UI��\������x���W�̏�����
	m_fPosX = UI_X_POSITION;

	// �R�}���h�����J�E���^�[�̏�����
	m_nCommandDeathCnt = 0;

	// �R�}���h���͔��f�t���O�̏�����
	m_isCommandInput = true;

	// �ŏ��ɓ��͂��ׂ��R�}���h�̍쐬
	CreateFirstCommand();

	D3DXVECTOR3 pos;

	// �Z���\���pUI�̏������W�̐ݒ�
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (m_MyID == MY_ID_1)
		{
			pos = D3DXVECTOR3(UI_X_POSITION + UI_X_POS_ADD + UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}
		else if (m_MyID == MY_ID_2)
		{
			pos = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}

		// �������̋Z���\���pUI���쐬
		m_apCommandName[i] = CCommandName::Create(m_pD3DDevice, pos, TEXTURE_MONO);
	}
}

//-----------------------------------------------------------------------------
//	�f�t�H���g�̏�Ԃɖ߂�����
//-----------------------------------------------------------------------------
void CCommandChart::SetDefault(void)
{
	// �ێ����̃R�}���h���̏�����
	m_nKeepCommandNum = 0;

	// ���͌��UI��\������x���W�̏�����
	m_fPosX = UI_X_POSITION;

	// �R�}���h�����J�E���^�[�̏�����
	m_nCommandDeathCnt = 0;

	// �R�}���h���͔��f�t���O�̏�����
	m_isCommandInput = true;

	// �ŏ��ɓ��͂��ׂ��R�}���h�̍쐬
	CreateFirstCommand();

	// �R�}���h���͏��̃|�C���^�̏�����
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		m_pCommandUI[i]->m_isInputButton = false;
		m_pCommandUI[i]->m_NextCommand = NULL;
		m_pCommandUI[i]->m_pUIType = NULL;
	}
}

//-----------------------------------------------------------------------------
//	�X�V����
//-----------------------------------------------------------------------------
void CCommandChart::Update(void)
{
	// �R�}���h���͉\
	if (m_isCommandInput)
	{
		// �R�}���h����
		InputCommand();
	}
	// �R�}���h���͕s��
	else
	{
		// �R�}���h�̃��Z�b�g
		ResetCommand();
	}

	if (UseTechnic() != COMMAND_TYPE_NONE)
	{
		// �R�}���h���͔���t���O��s��
		m_isCommandInput = false;
		// �R�}���h�̃��Z�b�g
		//ResetCommand();
	}
}

//-----------------------------------------------------------------------------
//	�`�揈��
//-----------------------------------------------------------------------------
void CCommandChart::Draw(void)
{
}

//-----------------------------------------------------------------------------
//	�I������
//-----------------------------------------------------------------------------
void CCommandChart::Uninit(void)
{
	m_pD3DDevice = NULL;

	// �����̃R�}���hUI�����X�g����폜���ďI���������s���Ă���f���[�g�����
	COMMAND_UI_INFO* pCommandUITemp = NULL;
	COMMAND_UI_INFO* pCommandNextUITemp = NULL;
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		pCommandUITemp = m_pCommandUI[i];

		while (pCommandUITemp)
		{
			pCommandNextUITemp = pCommandUITemp->m_NextCommand;
			if (pCommandUITemp->m_pUIType)
			{
				pCommandUITemp->m_pUIType->Uninit();
			}
			SAFE_DELETE(pCommandUITemp);
			pCommandUITemp = pCommandNextUITemp;
		}
	}
	// ���ݕ\�����Ă���UI�����X�g����폜���ďI���������s����
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (m_apNextCommandUI[i])
		{
			m_apNextCommandUI[i]->Uninit();
		}
	}
}

//-----------------------------------------------------------------------------
//	�ŏ��ɓ��͂�����̃R�}���h�̍쐬
//-----------------------------------------------------------------------------
void CCommandChart::CreateFirstCommand(void)
{
	// ���ɕ\������R�}���h�̐������W
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	// ���ɕ\������R�}���h�̖ڕWX���W
	float fPosDestX = 0.0f;

	// �ڕW�̍��W
	// �v���C���[�P�̎��̕\��X���W
	if (m_MyID == MY_ID_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == MY_ID_2)
	{
		fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS);
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	// �ێ�����MAX�Ŗ�����Ύ��̃R�}���h��\��
	if (m_nKeepCommandNum < MAX_COMMAND_KEEP)
	{
		// ���̃R�}���h��S�쐬
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			// ����Y���W
			fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);

			// �e�����{�^���̐���
			switch (i + 1)
			{
				// Q�������͉E���̏�{�^���ɑΉ�
			case BUTTON_TYPE_1:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				break;
				// W�������͉E���̉��{�^���ɑΉ�
			case BUTTON_TYPE_2:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				break;
				// A�������͍����̏�{�^���ɑΉ�
			case BUTTON_TYPE_3:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				break;
				// S�������͉E���̉��{�^���ɑΉ�
			case BUTTON_TYPE_4:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				break;
			default:
				break;
			}
			// ������ڎw�����W�̐ݒ�
			m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
		}
	}
}

//-----------------------------------------------------------------------------
//	�ŏ��ɓ��͂�����̃R�}���h�̍ĊJ�n
//-----------------------------------------------------------------------------
void CCommandChart::RestartFirstCommandUI(void)
{
	// ���ݕ\�����Ă���UI�����X�g����폜���ďI���������s����
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (m_apNextCommandUI[i])
		{
			m_apNextCommandUI[i]->SetDrawFlag(true);
		}
	}
}

//-----------------------------------------------------------------------------
//	���ɓ��͂�����̃R�}���h�̍쐬
//-----------------------------------------------------------------------------
void CCommandChart::CreateNextCommand(void)
{
	switch (m_aCommandKeep)
	{
		// Q�������͉E���̏�{�^���ɑΉ�
	case BUTTON_TYPE_1:
		CreateRightUpTechnicCommand();
		break;
		// W�������͉E���̉��{�^���ɑΉ�
	case BUTTON_TYPE_2:
		break;
		// A�������͍����̏�{�^���ɑΉ�
	case BUTTON_TYPE_3:
		CreateLeftUpTechnicCommand();
		break;
		// S�������͉E���̉��{�^���ɑΉ�
	case BUTTON_TYPE_4:
		CreateLeftDownTechnicCommand();
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//	���ɓ��͂�����̃R�}���h�̍폜
//-----------------------------------------------------------------------------
void CCommandChart::DeathNextCommand(void)
{
	// ���ݕ\�����Ă���UI�����X�g����폜���ďI���������s����
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (m_apNextCommandUI[i])
		{
			if (!m_apNextCommandUI[i]->GetInputFlag())
			{
				m_apNextCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	��ʊO�ւ̃t�F�[�h�A�E�g
//-----------------------------------------------------------------------------
void CCommandChart::ScreenOut(void)
{
}

//-----------------------------------------------------------------------------
//	��ʓ��ւ̃t�F�[�h�C��
//-----------------------------------------------------------------------------
void CCommandChart::ScreenIn(void)
{
}

//-----------------------------------------------------------------------------
//	�R�}���h����
//-----------------------------------------------------------------------------
void CCommandChart::InputCommand(void)
{
	bool isPushButton1 = false;
	bool isPushButton2 = false;
	bool isPushButton3 = false;
	bool isPushButton4 = false;

	//�L�[����
	if (m_MyID == MY_ID_1)
	{
		// �{�^���^�C�v1
		// �E��
		//bool isPushButton1 =
		isPushButton1 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_RIGHT_UP) ||			// �L�[�{�[�h����
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_UP, m_MyID);	// �R���g���[���[����
		// �{�^���^�C�v2
		// �E��
		//bool isPushButton2 =
		isPushButton2 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_RIGHT_DOWN) ||			// �L�[�{�[�h���� 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_DOWN, m_MyID);// �R���g���[���[���� 
		// �{�^���^�C�v3
		// ����
		//bool isPushButton3 =
		isPushButton3 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_LEFT_UP) ||			// �L�[�{�[�h���� 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_UP, m_MyID);	// �R���g���[���[���� 
		// �{�^���^�C�v4
		// ����
		//bool isPushButton4 =
		isPushButton4 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_1_LEFT_DOWN) ||			// �L�[�{�[�h���� 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_DOWN, m_MyID);	// �R���g���[���[���� 
		// �L�[���͂��ꂽ�炻���ێ�
	}
	else if (m_MyID == MY_ID_2)
	{
		// �{�^���^�C�v1
		// �E��
		//bool isPushButton1 =
		isPushButton1 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_RIGHT_UP) ||			// �L�[�{�[�h����
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_UP, m_MyID);	// �R���g���[���[����
		// �{�^���^�C�v2
		// �E��
		//bool isPushButton2 =
		isPushButton2 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_RIGHT_DOWN) ||			// �L�[�{�[�h���� 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_RIGHT_DOWN, m_MyID);// �R���g���[���[���� 
		// �{�^���^�C�v3
		// ����
		//bool isPushButton3 =
		isPushButton3 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_LEFT_UP) ||			// �L�[�{�[�h���� 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_UP, m_MyID);	// �R���g���[���[���� 
		// �{�^���^�C�v4
		// ����
		//bool isPushButton4 =
		isPushButton4 =
			CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_PLAYER_2_LEFT_DOWN) ||			// �L�[�{�[�h���� 
			CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_LEFT_DOWN, m_MyID);	// �R���g���[���[���� 
		// �L�[���͂��ꂽ�炻���ێ�
	}

	// �{�^���^�C�v�P
	if (isPushButton1)
	{
		// �L�[���͂̕ۑ�
		m_aCommandKeep = BUTTON_TYPE_1;

		if (m_nKeepCommandNum == 0)
		{
			m_apNextCommandUI[BUTTON_TYPE_1 - 1]->SetInputFlag(true);
		}
	}
	// �{�^���^�C�v�Q
	else if (isPushButton2)
	{
		// �L�[���͂̕ۑ�
		m_aCommandKeep = BUTTON_TYPE_2;

		if (m_nKeepCommandNum == 0)
		{
			m_apNextCommandUI[BUTTON_TYPE_2 - 1]->SetInputFlag(true);
		}
	}
	// �{�^���^�C�v�R
	else if (isPushButton3)
	{
		// �L�[���͂̕ۑ�
		m_aCommandKeep = BUTTON_TYPE_3;

		if (m_nKeepCommandNum == 0)
		{
			m_apNextCommandUI[BUTTON_TYPE_3 - 1]->SetInputFlag(true);
		}
	}
	// �{�^���^�C�v�S
	else if (isPushButton4)
	{
		// �L�[���͂̕ۑ�
		m_aCommandKeep = BUTTON_TYPE_4;

		if (m_nKeepCommandNum == 0)
		{
			m_apNextCommandUI[BUTTON_TYPE_4 - 1]->SetInputFlag(true);
		}
	}
	// �������͂���Ă����ꍇ�̋��ʏ���
	if (isPushButton1 || isPushButton2 || isPushButton3 || isPushButton4)
	{
		// �������̋Z���\���pUI�̖ڕW���W�̐ݒ�
		// �Z���\���pUI�̏������W�̐ݒ�
		if (m_nKeepCommandNum < MAX_COMMAND_KEEP - 1)
		{
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_MyID == MY_ID_1)
				{
					m_apCommandName[i]->SetDestPos(D3DXVECTOR3(m_fPosX + UI_X_POS_ADD + UI_X_POS_ADD + UI_X_POSITION, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
				}
				else if (m_MyID == MY_ID_2)
				{
					m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POS_ADD - UI_X_POSITION - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
				}
			}
		}

		// �R�}���h�ێ����̑���
		m_nKeepCommandNum++;

		// �`�悷��x���W�̍X�V
		m_fPosX += UI_X_POS_ADD;

		// �ŏ��̃R�}���h���͂̎��̏���
		if (m_nKeepCommandNum == 1)
		{
			// ���͌��̃R�}���h��������
			DeathNextCommand();

			// ���ɓ��͂��ׂ��R�}���h�̍쐬
			CreateNextCommand();
		}
		// 2��ڂ���̃R�}���h�̓��͂̎��̏���
		else
		{
			// �R�}���h�̓��̓`�F�b�N
			CommandUIInput(m_aCommandKeep);
		}

		// �R�}���h���͂���R�}���h�������܂ł̊Ԃ̏�����
		m_nCommandDeathCnt = 0;
	}
}

//-----------------------------------------------------------------------------
//	�R�}���h�̃��Z�b�g
//-----------------------------------------------------------------------------
void CCommandChart::ResetCommand(void)
{
	// �R�}���h�����܂ł̃J�E���^�[�̑���
	m_nCommandDeathCnt++;

	// COMMAND_DETH_COUNT���傫���l�ɂȂ�����s��
	//if (m_nCommandDeathCnt > COMMAND_DEATH_COUNT)
	{
		// �����̃R�}���hUI�����X�g����폜���ďI���������s���Ă���f���[�g�����
		for (int i = 0; i < COMMAND_TYPE_NUM; i++)
		{
			COMMAND_UI_INFO* pCommandUITemp = m_pCommandUI[i];

			while (pCommandUITemp)
			{
				COMMAND_UI_INFO* pCommandNextUITemp = pCommandUITemp->m_NextCommand;
				if (pCommandUITemp->m_pUIType)
				{
					pCommandUITemp->m_pUIType->UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
					pCommandUITemp->m_NextCommand = NULL;
					pCommandUITemp->m_pUIType = NULL;
					pCommandUITemp->m_isInputButton = false;
				}
				pCommandUITemp = pCommandNextUITemp;
			}
		}

		// ���ݕ\�����Ă���UI�����X�g����폜���ďI���������s����
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_apNextCommandUI[i])
			{
				m_apNextCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
			}
		}

		// �u���͂��ꂽ�R�}���h�ێ��p�ϐ��̏�����
		// ���͂��ꂽ�R�}���h�ێ��p
		m_aCommandKeep = BUTTON_TYPE_NONE;

		// �R�}���h�ێ����ƕێ��R�}���h�̃��Z�b�g
		SetDefault();
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_MyID == MY_ID_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + UI_X_POS_ADD + UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == MY_ID_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �R�}���h�̃`�F�b�N
// �����F�Z���̃A�h���X
// �߂�l�F�R�}���h���������������ۂ�
//-----------------------------------------------------------------------------
bool CCommandChart::CheckCommand(COMMAND_UI_INFO* Technic, COMMAND_INFO* pCommand)
{
	bool isMatch = false;	// ��v���Ă�����
	int nMatchNum = 0;		// �R�}���h�̐��m�F

	/*
	// ���͂���Ă���R�}���h�Ƒ����Ă����R�}���h����v���Ă��邩�m�F
	for (int i = 0; i < m_nKeepCommandNum; i++)
	{
		if (m_aCommandKeep == Technic->m_Command[i])
		{
			// ��v�̐��𑝂₷
			nMatchNum++;
		}
	}

	// ��v���Ɠ��͐��������Ȃ�Έ�v���Ă���Ƃ���
	if (nMatchNum == m_nKeepCommandNum)
	{
		isMatch = true;
	}
	*/

	// �n���ꂽ���X�g���Ō�܂Ŋm�F
	while (1)
	{
		// ���݂̓��͌��̃R�}���h��NULL���������̓��[�v�𔲂���
		if (!Technic->m_pUIType)
		{
			break;
		}

		// ���ݓ��͌��̃R�}���h�̃{�^����������Ă�����
		if (Technic->m_isInputButton)
		{
			if (!Technic->m_pUIType)
			{
				break;
			}
			// �R�}���h�̔�r
			if (Technic->m_pUIType->GetButtonType() == pCommand->m_Command[nMatchNum])
			{
				// �������̌��̃R�}���h�̃{�^�������������ꍇ���̃R�}���h�͐������Ă���
				if (!Technic->m_NextCommand->m_NextCommand)
				{
					isMatch = true;
					break;
				}
				// ���̌��̃R�}���h���������ꍇ�͎��̌��Ƀ|�C���^�����炷
				else
				{
					Technic = Technic->m_NextCommand;
				}
				nMatchNum++;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	// ���ʂ�Ԃ�
	return isMatch;
}

//-----------------------------------------------------------------------------
// �Z�̊m��
// �߂�l�F�J��o���Z
//
// �R�}���h���������̂̕����D�悳���
//-----------------------------------------------------------------------------
COMMAND_TYPE CCommandChart::UseTechnic(void)
{
	bool isUseTechnic = false;
	COMMAND_TYPE CommandType = COMMAND_TYPE_NONE;	// �ŏI�I�ɕԂ��ׂ��Z
	
	// (���Z)
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		if (!m_pCommandUI[COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			break;
		}

		// ���͂���Ă���R�}���h���ǂ̋Z�ƈ�v���Ă��邩�m�F
		// ��v���Ă����ture��v���Ă��Ȃ����false��isUseTechnic�ɓ����
		if (m_pCommandUI[COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			isUseTechnic = CheckCommand(m_pCommandUI[COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM],
				(COMMAND_INFO*)COMMAND_SMALL_TECHNIQUE_LIST[i]);
		}

		// ��v���Ă���R�}���h�����邩�m�F
		if (isUseTechnic)
		{
			// ��v���Ă����ꍇ�̃R�}���h�̃^�C�v��Ԃ�
			CommandType = COMMAND_SMALL_TECHNIQUE_LIST[i]->m_CommandType;
			break;
		}
	}
	
	// (���Z)
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		if (!m_pCommandUI[COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			break;
		}
		// ���͂���Ă���R�}���h���ǂ̋Z�ƈ�v���Ă��邩�m�F
		// ��v���Ă����ture��v���Ă��Ȃ����false��isUseTechnic�ɓ����
		if (m_pCommandUI[COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			isUseTechnic = CheckCommand(m_pCommandUI[COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM],
				(COMMAND_INFO*)COMMAND_MIDDLE_TECHNIQUE_LIST[i]);
		}

		// ��v���Ă���R�}���h�����邩�m�F
		if (isUseTechnic)
		{
			// ��v���Ă����ꍇ�̃R�}���h�̃^�C�v��Ԃ�
			CommandType = COMMAND_MIDDLE_TECHNIQUE_LIST[i]->m_CommandType;
			break;
		}
	}
	
	// (��Z)
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		if (!m_pCommandUI[COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			break;
		}

		// ���͂���Ă���R�}���h���ǂ̋Z�ƈ�v���Ă��邩�m�F
		// ��v���Ă����ture��v���Ă��Ȃ����false��isUseTechnic�ɓ����
		if (m_pCommandUI[COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM]->m_NextCommand)
		{
			isUseTechnic = CheckCommand(m_pCommandUI[COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM],
				(COMMAND_INFO*)COMMAND_LARGE_TECHNIQUE_LIST[i]);
		}

		// ��v���Ă���R�}���h�����邩�m�F
		if (isUseTechnic)
		{
			// ��v���Ă����ꍇ�̃R�}���h�̃^�C�v��Ԃ�
			CommandType = COMMAND_LARGE_TECHNIQUE_LIST[i]->m_CommandType;
			break;
		}
	}

	// ���[�v�R�}���h
	if (m_apNextCommandUI[BUTTON_TYPE_2 - 1])
	{
		if (m_apNextCommandUI[BUTTON_TYPE_2 - 1]->GetInputFlag())
		{
			CommandType = COMMAND_TYPE_ROPE;
		}
	}

	// ���肵���Z��Ԃ�
	return CommandType;
}

//-----------------------------------------------------------------------------
// �E��L�[�J�n�R�}���h�̐���
// �����F���ڂ̃R�}���h��
//-----------------------------------------------------------------------------
void CCommandChart::CreateRightUpTechnicCommand(void)
{
	// ���ɕ\������R�}���h�̐������W
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	// ���ɕ\������R�}���h�̖ڕWX���W
	float fPosDestX = 0.0f;

	// �R�}���hUI�쐬���ɃR�}���hUI�̃|�C���^�����邽�߂̃|�C���^
	COMMAND_UI_INFO* pCommandUI = NULL;

	// �ڕW�̍��W
	// �v���C���[�P�̎��̕\��X���W
	if (m_MyID == MY_ID_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == MY_ID_2)
	{
		fPosDestX = SCREEN_WIDTH - NEXT_UI_X_POS;
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		// �R�}���h��\�L���邽�߂�Y���W�̐ݒ�
		fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);

		// �㒆���U���̃R�}���h�̃��X�g�̐擪�A�h���X����
		pCommandUI = m_pCommandUI[i];

		for (int j = 0; j < COMMAND_RIGHT_UP_TECHNIQUE_LIST[i]->m_nCommandLength; j++)
		{
			// �ڕW�̍��W
			// �v���C���[�P�̎��̕\��X���W
			if (m_MyID == MY_ID_1)
			{
				fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
				fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
			}
			// �v���C���[�Q�̎��̕\��X���W
			else if (m_MyID == MY_ID_2)
			{
				fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
				fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
			}

			// �R�}���hUI�̃{�^���̎�ނɉ�����UI���쐬����
			switch (COMMAND_RIGHT_UP_TECHNIQUE_LIST[i]->m_Command[j])
			{
				// Q�������͉E���̏�{�^���ɑΉ�
			case BUTTON_TYPE_1:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// W�������͉E���̉��{�^���ɑΉ�
			case BUTTON_TYPE_2:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// A�������͍����̏�{�^���ɑΉ�
			case BUTTON_TYPE_3:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// S�������͉E���̉��{�^���ɑΉ�
			case BUTTON_TYPE_4:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			default:
				break;
			}
			// �쐬�Ώۂ����X�g�̎��̃|�C���^�ւ��炷
			pCommandUI->m_NextCommand = new COMMAND_UI_INFO;
			pCommandUI = pCommandUI->m_NextCommand;
			// ���̃|�C���^�̏�����
			pCommandUI->m_NextCommand = NULL;
			pCommandUI->m_isInputButton = false;
			pCommandUI->m_pUIType = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// ����L�[�J�n�̃R�}���h�̐���
// �����F���ڂ̃R�}���h��
//-----------------------------------------------------------------------------
void CCommandChart::CreateLeftUpTechnicCommand(void)
{
	// ���ɕ\������R�}���h�̐������W
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	// ���ɕ\������R�}���h�̖ڕWX���W
	float fPosDestX = 0.0f;

	// �R�}���hUI�쐬���ɃR�}���hUI�̃|�C���^�����邽�߂̃|�C���^
	COMMAND_UI_INFO* pCommandUI = NULL;

	// �ڕW�̍��W
	// �v���C���[�P�̎��̕\��X���W
	if (m_MyID == MY_ID_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == MY_ID_2)
	{
		fPosDestX = SCREEN_WIDTH - NEXT_UI_X_POS;
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		// �R�}���h��\�L���邽�߂�Y���W�̐ݒ�
		fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);

		// �㒆���U���̃R�}���h�̃��X�g�̐擪�A�h���X����
		pCommandUI = m_pCommandUI[i];

		// �R�}���h�̒�������
		for (int j = 0; j < COMMAND_LEFT_UP_TECHNIQUE_LIST[i]->m_nCommandLength; j++)
		{
			// �ڕW�̍��W
			// �v���C���[�P�̎��̕\��X���W
			if (m_MyID == MY_ID_1)
			{
				fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
				fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
			}
			// �v���C���[�Q�̎��̕\��X���W
			else if (m_MyID == MY_ID_2)
			{
				fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
				fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
			}

			// �R�}���hUI�̃{�^���̎�ނɉ�����UI���쐬����
			switch (COMMAND_LEFT_UP_TECHNIQUE_LIST[i]->m_Command[j])
			{
				// Q�������͉E���̏�{�^���ɑΉ�
			case BUTTON_TYPE_1:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// W�������͉E���̉��{�^���ɑΉ�
			case BUTTON_TYPE_2:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// A�������͍����̏�{�^���ɑΉ�
			case BUTTON_TYPE_3:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// S�������͉E���̉��{�^���ɑΉ�
			case BUTTON_TYPE_4:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			default:
				break;
			}
			// �쐬�Ώۂ����X�g�̎��̃|�C���^�ւ��炷
			pCommandUI->m_NextCommand = new COMMAND_UI_INFO;
			pCommandUI = pCommandUI->m_NextCommand;
			// ���̃|�C���^�̏�����
			pCommandUI->m_NextCommand = NULL;
			pCommandUI->m_isInputButton = false;
			pCommandUI->m_pUIType = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �����L�[�J�n�R�}���h�̐���
// �����F���ڂ̃R�}���h��
//-----------------------------------------------------------------------------
void CCommandChart::CreateLeftDownTechnicCommand(void)
{
	// ���ɕ\������R�}���h�̐������W
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	// ���ɕ\������R�}���h�̖ڕWX���W
	float fPosDestX = 0.0f;

	// �R�}���hUI�쐬���ɃR�}���hUI�̃|�C���^�����邽�߂̃|�C���^
	COMMAND_UI_INFO* pCommandUI = NULL;

	// �ڕW�̍��W
	// �v���C���[�P�̎��̕\��X���W
	if (m_MyID == MY_ID_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == MY_ID_2)
	{
		fPosDestX = SCREEN_WIDTH - NEXT_UI_X_POS;
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		// �R�}���h��\�L���邽�߂�Y���W�̐ݒ�
		fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);

		// �㒆���U���̃R�}���h�̃��X�g�̐擪�A�h���X����
		pCommandUI = m_pCommandUI[i];

		for (int j = 0; j < COMMAND_LEFT_DOWN_TECHNIQUE_LIST[i]->m_nCommandLength; j++)
		{
			// �ڕW�̍��W
			// �v���C���[�P�̎��̕\��X���W
			if (m_MyID == MY_ID_1)
			{
				fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
				fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
			}
			// �v���C���[�Q�̎��̕\��X���W
			else if (m_MyID == MY_ID_2)
			{
				fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
				fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
			}

			// �R�}���hUI�̃{�^���̎�ނɉ�����UI���쐬����
			switch (COMMAND_LEFT_DOWN_TECHNIQUE_LIST[i]->m_Command[j])
			{
				// Q�������͉E���̏�{�^���ɑΉ�
			case BUTTON_TYPE_1:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				// �R�}���h�̃{�^���̎�ނ̊m��
				break;
				// W�������͉E���̉��{�^���ɑΉ�
			case BUTTON_TYPE_2:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// A�������͍����̏�{�^���ɑΉ�
			case BUTTON_TYPE_3:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
				// S�������͉E���̉��{�^���ɑΉ�
			case BUTTON_TYPE_4:
				pCommandUI->m_pUIType = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f),	// �����ʒu
					TEXTURE_BUTTON);
				// ������ڎw�����W�̐ݒ�
				pCommandUI->m_pUIType->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			default:
				break;
			}
			// �쐬�Ώۂ����X�g�̎��̃|�C���^�ւ��炷
			pCommandUI->m_NextCommand = new COMMAND_UI_INFO;
			pCommandUI = pCommandUI->m_NextCommand;
			// ���̃|�C���^�̏�����
			pCommandUI->m_NextCommand = NULL;
			pCommandUI->m_isInputButton = false;
			pCommandUI->m_pUIType = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
//	�R�}���hUI�̓���
//-----------------------------------------------------------------------------
void CCommandChart::CommandUIInput(BUTTON_TYPE button)
{
	COMMAND_UI_INFO* pCommandTemp;

	// �R�}���h�̋�����̃��X�g�𒲂ׂ�
	for (int i = 0; i < COMMAND_TYPE_NUM; i++)
	{
		// ���ϐ��ɑ��
		pCommandTemp = m_pCommandUI[i];

		while (1)
		{
			// ���݂̓��͌��̃R�}���h��NULL���������̓��[�v�𔲂���
			if (!pCommandTemp->m_pUIType)
			{
				break;
			}

			// ���ݓ��͌��̃R�}���h�̃{�^���̎�ނ����͂��ꂽ���̂Ɠ�����������
			if (pCommandTemp->m_pUIType->GetButtonType() == button)
			{
				// �R�}���h�̓��͂��܂��s���Ă��Ȃ������ꍇ�R�}���h�̓���on�ɂ��Ĉ�U�o��
				if (!pCommandTemp->m_isInputButton)
				{
					pCommandTemp->m_isInputButton = true;
					pCommandTemp->m_pUIType->InputUIUVChange(button);
					break;
				}
				// �R�}���h�̓��͂��s���Ă����ꍇ���̃|�C���^��
				else
				{
					pCommandTemp = pCommandTemp->m_NextCommand;
				}
			}
			else
			{
				// ���̓R�}���h������łȂ��A���݌��̃R�}���h�����͍ς݂������ꍇ���̃R�}���h�Ƀ|�C���^�����炷
				if (pCommandTemp->m_isInputButton)
				{
					pCommandTemp = pCommandTemp->m_NextCommand;
				}
				else
				{
					break;
				}
			}
		}
	}
}

// EOF