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
#include "CCommandChartManager.h"
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
static const float COMMAND_NAME_ADD_NUM = 7.0f;	// �R�}���h�l�[���̕ω��̒l�̐�
static const float FADE_UI_OUT_POS_X_ID_1 = -50.0f;					//�t�F�[�h�A�E�g���̖ڕW���W������ID�P
static const float FADE_UI_OUT_POS_X_ID_2 = SCREEN_WIDTH + 50.0f;	//�t�F�[�h�A�E�g���̖ڕW���W������ID�Q
static const float BACK_POLYGON_X_SIZE = (COMMAND_POLYGON_WIDTH*MAX_COMMAND_KEEP) + (UI_X_POS_ADD*MAX_COMMAND_KEEP);	// �R�}���h�`���[�gUI�̔w��ɕ\������|���S����X�̃T�C�Y
static const float BACK_POLYGON_Y_SIZE = 150.0f;	// �R�}���h�`���[�gUI�̔w��ɕ\������|���S����Y�̃T�C�Y
static const int COMMAND_DEATH_COUNT = 60;		// �R�}���h�����܂ł̃J�E���g
static const int COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM = 0;	// ��U���̃R�}���h�̔z��ԍ�
static const int COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM = 1;	// ���U���̃R�}���h�̔z��ԍ�
static const int COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM = 2;	// ��U���̃R�}���h�̔z��ԍ�
static const D3DXVECTOR3 BACK_POLYGON_POS_1 = D3DXVECTOR3((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// �w��̃|���S���̍��WID1
static const D3DXVECTOR3 BACK_POLYGON_POS_2 = D3DXVECTOR3(SCREEN_WIDTH - ((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION - (COMMAND_POLYGON_WIDTH - 2.0f)), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// �w��̃|���S���̍��W
static const float ANIME_SPEED = 0.04f;			// �o�����������̃A�j���[�V�������x
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
static const COMMAND_INFO COMMAND_ROLLING_ELBOW = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_ROLLING, COMMAND_BUTTON_ROLLING_ELBOW };

//�t���C���O�G���{�[	RU RU LU RD
static const BUTTON_TYPE COMMAND_BUTTON_FLYING_ELBOW[COMMAND_INPUT_NUM_MIDDLE] = { BUTTON_TYPE_3,
																			BUTTON_TYPE_3, 
																			BUTTON_TYPE_1,
																			BUTTON_TYPE_2 };
static const COMMAND_INFO COMMAND_FLYING_ELBOW = { COMMAND_INPUT_NUM_MIDDLE, COMMAND_TYPE_SHOULDER, COMMAND_BUTTON_FLYING_ELBOW };

//�h���b�v�L�b�N		RU LU RU RU RD
static const BUTTON_TYPE COMMAND_BUTTON_DROP_KICK[COMMAND_INPUT_NUM_LARGE] = { BUTTON_TYPE_3,
																		BUTTON_TYPE_1,
																		BUTTON_TYPE_3,
																		BUTTON_TYPE_3,
																		BUTTON_TYPE_4 };
static const COMMAND_INFO COMMAND_DROP_KICK = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_DROPKICK, COMMAND_BUTTON_DROP_KICK };

//�r���^			LD LU RU
static const BUTTON_TYPE COMMAND_BUTTON_FACE_SLAPPING[COMMAND_INPUT_NUM_SMALL] = { BUTTON_TYPE_4,
																			BUTTON_TYPE_3,
																			BUTTON_TYPE_1 };
static const COMMAND_INFO COMMAND_FACE_SLAPPING = { COMMAND_INPUT_NUM_SMALL, COMMAND_TYPE_SLAP, COMMAND_BUTTON_FACE_SLAPPING };

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
static const COMMAND_INFO COMMAND_STANER = { COMMAND_INPUT_NUM_LARGE, COMMAND_TYPE_STUNNER, COMMAND_BUTTON_STANER };

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
static const COMMAND_INFO* COMMAND_SMALL_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_CHOP,
																			  &COMMAND_ROLLING_ELBOW,
																			  &COMMAND_FACE_SLAPPING};
// ���Z
// �G���{�[
// �t���C���O�G���{�[
// �o�b�N�h���b�v
static const COMMAND_INFO* COMMAND_MIDDLE_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_ELBOW,
																			  &COMMAND_FLYING_ELBOW,
																			  &COMMAND_BACKDROP };

// ��Z
// �����A�b�g
// �h���b�v�L�b�N
// �X�^�i�[
static const COMMAND_INFO* COMMAND_LARGE_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_LARIAT,
																			  &COMMAND_DROP_KICK,
																			  &COMMAND_STANER };

// ����L�[�J�n�R�}���h���܂Ƃ߂��z��
// �`���b�v
// �G���{�[
// �����A�b�g
static const COMMAND_INFO* COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_MAX] = {&COMMAND_CHOP,
																				&COMMAND_ELBOW,
																				&COMMAND_LARIAT};

// �E��L�[�J�n�R�}���h���܂Ƃ߂��z��
// ���[�����O�G���{�[
// �t���C���O�G���{�[
// �h���b�v�L�b�N
static const COMMAND_INFO* COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_ROLLING_ELBOW,
																				&COMMAND_FLYING_ELBOW,
																				&COMMAND_DROP_KICK};

// �����L�[�J�n�R�}���h���܂Ƃ߂��z��
// �r���^
// �o�b�N�h���b�v
// �X�^�i�[
static const COMMAND_INFO* COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_FACE_SLAPPING,
																					&COMMAND_BACKDROP,
																					&COMMAND_STANER };

// �E���L�[�J�n�R�}���h���܂Ƃ߂��z��
// �����A�b�g
// �X�^�i�[
// �h���b�v�L�b�N
static const COMMAND_INFO* COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_MAX] = { &COMMAND_LARIAT,
																					&COMMAND_STANER,
																					&COMMAND_DROP_KICK };

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
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* pDevice, int nID, CCommandChartManager* pCommandManager)
{
	// �f�o�C�X�̃|�C���^��ۑ�
	m_pD3DDevice = pDevice;

	// ���͂��ꂽ�R�}���h�ێ��p�ϐ��̏�����
	m_aCommandKeep = BUTTON_TYPE_NONE;

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
	m_pBackPolygon->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBackPolygon->SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f));

	m_CommandChartMode = MODE_INPUT;

	m_CompleteCommand = COMMAND_TYPE_NONE;

	m_pCommandManager = pCommandManager;
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
CCommandChart* CCommandChart::Create(LPDIRECT3DDEVICE9* pDevice, int nID, CCommandChartManager* pCommandManager)
{
	// �R�}���h�`���[�g�̐���
	CCommandChart* temp = new CCommandChart(pDevice, nID, pCommandManager);
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

	//*******************�ǋL�J�n11/23�@��K **************************************
	// �ŏ��̃R�}���h�쐬 �\���͂��Ȃ�
	InitCreateBeginCommand();

	// �ŏ��ȊO�̃R�}���h�쐬 �\���͂��Ȃ�
	InitCreateCommandList();

	// �ŏ��̃R�}���h�̂ݕ\��
	ResetAllCommand();
	//*******************�ǋL�I��11/23�@��K***************************************

	// �J�����������A�j���̏����������̂܂Ƃ�
	InitAnime();
}

//-----------------------------------------------------------------------------
//	�X�V����
//-----------------------------------------------------------------------------
void CCommandChart::Update(void)
{
#ifdef _DEBUG
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_COMMAND_ANIME_CLOSE))
	{
		m_CommandChartMode = MODE_VANISH;
	}
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_COMMAND_ANIME_OPEN))
	{
		m_CommandChartMode = MODE_APPEAR;
	}
#endif
	// �R�}���h���Z�b�g �Ƃ肠�����A���Ԃ񂠂ƂŔ�����@CCommandChart::MODE_RESET
	m_CompleteCommand = COMMAND_TYPE_NONE;

	// �A�j���[�V�����X�V�܂Ƃ�
	UpdateAnime();

	switch (m_CommandChartMode)
	{
	case MODE_APPEAR:
		// �R�}���h�`���[�g�o��
		AppearanceCommandChart();
		break;

	case MODE_INPUT:
		// �R�}���h���͉\
		if (m_isCommandInput)
		{
			// �R�}���h����
			InputCommand();

			// �R�}���h�̃`�F�b�N
			CheckCommand();
			m_aCommandKeep = BUTTON_TYPE_NONE;
		}
		break;

	case MODE_COMPLETE_COMMAND:
		// �Z���͊����̉��o�@����������

		// �W���b�W�ɃZ�b�g������

		// TODO
		// �����Ƀ��Z�b�g���Ȃ��őҋ@���Ԃ�݂���
		//m_CommandChartMode = MODE_RESET;
		break;

	case MODE_VANISH:
		// �R�}���h�`���[�g������
		VanishCommandChart();
		break;

	case MODE_RESET:
		// ���Z�b�g
		ResetCommandList();
		m_CommandChartMode = MODE_INPUT;
		break;
	
	case MODE_ROPE:
		SetRopeCommand();
		m_CommandChartMode = MODE_INPUT;
		break;

	default:
		break;
	}
#ifdef _DEBUG
	CDebugProc::PrintU("�Z�^�C�v:%d\n", m_CompleteCommand);
#endif
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

	}
	// �{�^���^�C�v�Q
	else if (isPushButton2)
	{
		// �L�[���͂̕ۑ�
		m_aCommandKeep = BUTTON_TYPE_2;

	}
	// �{�^���^�C�v�R
	else if (isPushButton3)
	{
		// �L�[���͂̕ۑ�
		m_aCommandKeep = BUTTON_TYPE_3;

	}
	// �{�^���^�C�v�S
	else if (isPushButton4)
	{
		// �L�[���͂̕ۑ�
		m_aCommandKeep = BUTTON_TYPE_4;

	}
	//*******************�ύX�J�n11/23�@��K **************************************
	// ����������ĂȂ��Ȃ�I��
	else
	{
		return;
	}

	// �R�}���h�ێ����̑���
	m_nKeepCommandNum++;

	// �`�悷��x���W�̍X�V
	m_fPosX += UI_X_POS_ADD;

	// �ŏ��̃R�}���h���͂̎��̏���
	if (m_nKeepCommandNum == 1)
	{
		//*******************�ύX�J�n11/23�@��K **************************************
		// ���ɓ��͂��ׂ��R�}���h�̃��Z�b�g
		ResetNextCommand();
		//*******************�ύX�I��11/23�@��K **************************************

		// �������̋Z���\���pUI�̖ڕW���W�̐ݒ�
		// �Z���\���pUI�̏������W�̐ݒ�
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_MyID == MY_ID_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3((UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) + UI_X_POSITION, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == MY_ID_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - (UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) - UI_X_POSITION - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
		}
	}
	// 2��ڂ���̃R�}���h�̓��͂̎��̏���
	else
	{
		// �R�}���h�̓��̓`�F�b�N
		CommandUIInput(m_aCommandKeep);
	}

	// �R�}���h���͂���R�}���h�������܂ł̊Ԃ̏�����
	m_nCommandDeathCnt = 0;
	//*******************�ύX�I��11/23�@��K **************************************
}


//-----------------------------------------------------------------------------
// �R�}���h�̃`�F�b�N
//-----------------------------------------------------------------------------
void CCommandChart::CheckCommand(void)
{
	// �t�B�j�b�V���̔���͂܂��Ȃ�

	SKILL_TYPE type = SKILL_MAX;


	//*******************�ύX�J�n11/23�@��K **************************************
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �I�[��������Ă邩�𒲂ׂ�
		if (!m_CommandInfo.commandList.smallAttack[j].isEndList)
		{
			continue;
		}
		if (m_CommandInfo.commandList.smallAttack[j].pCommandUI->GetInputFlag())
		{
			type = SKILL_SMALL_ATTACK;
		}
		break;
	}

	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �I�[��������Ă邩�𒲂ׂ�
		if (!m_CommandInfo.commandList.middleAttack[j].isEndList)
		{
			continue;
		}
		if (m_CommandInfo.commandList.middleAttack[j].pCommandUI->GetInputFlag())
		{
			type = SKILL_MIDDLE_ATTACK;
		}
		break;
	}

	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �I�[��������Ă邩�𒲂ׂ�
		if (!m_CommandInfo.commandList.largeAttack[j].isEndList)
		{
			continue;
		}
		if (m_CommandInfo.commandList.largeAttack[j].pCommandUI->GetInputFlag())
		{
			type = SKILL_BIG_ATTACK;
		}
		break;
	}

	// �����������Ă��Ȃ��Ȃ�Ԃ�
	if (type == SKILL_MAX)
	{
		return;
	}

	// ������Ă�n���Z�����ޔ���
	int idx = -1;
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (!m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->GetInputFlag())
		{
			continue;
		}

		BUTTON_TYPE buttonType = m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->GetButtonType();
		if (buttonType == BUTTON_TYPE_3)
		{
			idx = 0;
		}
		else if (buttonType == BUTTON_TYPE_1)
		{
			idx = 1;
		}
		else
		{
			idx = 2;
		}
		break;
	}
	if (idx == -1)
	{
		return;
	}

	// �R�}���h����
	switch (type)
	{
	case SKILL_SMALL_ATTACK:
		m_CompleteCommand = COMMAND_SMALL_TECHNIQUE_LIST[idx]->m_CommandType;
		break;

	case SKILL_MIDDLE_ATTACK:
		m_CompleteCommand = COMMAND_MIDDLE_TECHNIQUE_LIST[idx]->m_CommandType;
		break;

	case SKILL_BIG_ATTACK:
		m_CompleteCommand = COMMAND_LARGE_TECHNIQUE_LIST[idx]->m_CommandType;
		break;

	default:
		return;
		break;
	}

	// ��Ԃ�������Ԃ�
	m_CommandChartMode = MODE_COMPLETE_COMMAND;
	//*******************�ύX�I��11/23�@��K **************************************
}

//-----------------------------------------------------------------------------
// �Z�̊m��
// �߂�l�F�J��o���Z
//
// �R�}���h���������̂̕����D�悳���
//-----------------------------------------------------------------------------
COMMAND_TYPE CCommandChart::UseTechnic(void)
{
	// ���肵���Z��Ԃ�
	return m_CompleteCommand;
}

//-----------------------------------------------------------------------------
// �E��L�[�J�n�R�}���h�̐���
// �����F���ڂ̃R�}���h��
//-----------------------------------------------------------------------------
void CCommandChart::CreateRightUpTechnicCommand(void)
{
	//*******************�ύX�J�n11/23�@��K **************************************
	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_Command[j], false);
		
		// �`��ON
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.smallAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].isEndList = true;
		}
	}

	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.middleAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].isEndList = true;
		}
	}

	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.largeAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].isEndList = true;
		}
	}
	//*******************�ύX�I��11/23�@��K **************************************
}

//-----------------------------------------------------------------------------
// ����L�[�J�n�̃R�}���h�̐���
// �����F���ڂ̃R�}���h��
//-----------------------------------------------------------------------------
void CCommandChart::CreateLeftUpTechnicCommand(void)
{
	//*******************�ύX�J�n11/23�@��K **************************************
	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.smallAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].isEndList = true;
		}
	}

	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.middleAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].isEndList = true;
		}
	}

	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.largeAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_UP_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].isEndList = true;
		}
	}
	//*******************�ύX�I��11/23�@��K **************************************
}

//-----------------------------------------------------------------------------
// �����L�[�J�n�R�}���h�̐���
// �����F���ڂ̃R�}���h��
//-----------------------------------------------------------------------------
void CCommandChart::CreateLeftDownTechnicCommand(void)
{
	//*******************�ύX�J�n11/23�@��K **************************************
	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.smallAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].isEndList = true;
		}
	}

	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.middleAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].isEndList = true;
		}
	}

	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.largeAttack[j - 1].isEndList = false;
		if (j == COMMAND_LEFT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].isEndList = true;
		}
	}
	//*******************�ύX�I��11/23�@��K **************************************
}

//-----------------------------------------------------------------------------
// �E���L�[�J�n�̃R�}���h�`���[�g�̐���
//-----------------------------------------------------------------------------
void CCommandChart::CreateRightDownTechnicCommand(void)
{
	//*******************�ύX�J�n11/23�@��K **************************************
	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.smallAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_SMALL_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].isEndList = true;
		}
	}

	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.middleAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_MIDDLE_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].isEndList = true;
		}
	}

	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_Command[j], false);

		// �`��ON
		m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.largeAttack[j - 1].isEndList = false;
		if (j == COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[SKILL_BIG_ATTACK]->m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].isEndList = true;
		}
	}
	//*******************�ύX�I��11/23�@��K **************************************
}
//-----------------------------------------------------------------------------
//	�R�}���hUI�̓���
//-----------------------------------------------------------------------------
void CCommandChart::CommandUIInput(BUTTON_TYPE button)
{
	// �R�}���h�̎㒆���̃��X�g�𒲂ׂ�
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		if (m_CommandInfo.commandList.smallAttack[j].pCommandUI->GetInputFlag())
		{
			continue;
		}

		// �{�^����r
		if (m_CommandInfo.commandList.smallAttack[j].pCommandUI->GetButtonType() == button)
		{
			m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetInputFlag(true);
			m_CommandInfo.commandList.smallAttack[j].pCommandUI->InputUIUVChange(button, true);
		}
		break;
	}

	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		if (m_CommandInfo.commandList.middleAttack[j].pCommandUI->GetInputFlag())
		{
			continue;
		}

		// �{�^����r
		if (m_CommandInfo.commandList.middleAttack[j].pCommandUI->GetButtonType() == button)
		{
			m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetInputFlag(true);
			m_CommandInfo.commandList.middleAttack[j].pCommandUI->InputUIUVChange(button, true);
		}
		break;
	}

	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		if (m_CommandInfo.commandList.largeAttack[j].pCommandUI->GetInputFlag())
		{
			continue;
		}

		// �{�^����r
		if (m_CommandInfo.commandList.largeAttack[j].pCommandUI->GetButtonType() == button)
		{
			m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetInputFlag(true);
			m_CommandInfo.commandList.largeAttack[j].pCommandUI->InputUIUVChange(button, true);
		}
		break;
	}
}


//*******************�ǋL�J�n11/23�@��K **************************************
//-----------------------------------------------------------------------------
// �n���R�}���h�̍쐬 
//-----------------------------------------------------------------------------
void CCommandChart::InitCreateBeginCommand(void)
{
	// ���ɕ\������R�}���h�̐������W
	float fPosX = 0.0f;
	
	// �ڕW�̍��W
	// �v���C���[�P�̎��̕\��X���W
	if (m_MyID == MY_ID_1)
	{
		fPosX = NEXT_UI_X_POS;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == MY_ID_2)
	{
		fPosX = SCREEN_WIDTH - NEXT_UI_X_POS;
	}

	D3DXVECTOR3 pos(fPosX, UI_Y_POSITION, 0);
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
																						, type
																						, pos
																						,TEXTURE_BUTTON);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDestPos(pos);

		// �A�j���[�V�����p�ɍ��W��ۑ�
		m_CommandInfo.beginCommand.firstCommand[i].vAnimationPosDest = pos;

		pos.y += NEXT_UI_Y_POS_ADD;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
	}
}

//-----------------------------------------------------------------------------
// ���ɓ��͂�����̃R�}���h�̍쐬
//-----------------------------------------------------------------------------
void CCommandChart::InitCreateCommandList(void)
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
		fPosDestX = SCREEN_WIDTH - NEXT_UI_X_POS;
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	fPosY = UI_Y_POSITION;

	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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
	
		m_CommandInfo.commandList.smallAttack[j].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
																					, BUTTON_TYPE_1
																					, D3DXVECTOR3(fPosX, fPosY, 0.0f)	// �����ʒu
																					, TEXTURE_BUTTON);
		// ������ڎw�����W�̐ݒ�
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// �A�j���[�V�����p�ɍ��W��ۑ�
		m_CommandInfo.commandList.smallAttack[j].vAnimationPosDest = D3DXVECTOR3(fPosDestX, fPosY, 0.0f);

		// �\���͂��Ȃ�
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);

		// �I�[�t���O�̐ݒ�
		m_CommandInfo.commandList.smallAttack[j].isEndList = false;
		if (j == COMMAND_INPUT_NUM_SMALL - 2)
		{
			m_CommandInfo.commandList.smallAttack[j].isEndList = true;
		}
	}

	// �R�}���h��\�L���邽�߂�Y���W�̐ݒ�
	fPosY += NEXT_UI_Y_POS_ADD;

	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		m_CommandInfo.commandList.middleAttack[j].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
			, BUTTON_TYPE_1
			, D3DXVECTOR3(fPosX, fPosY, 0.0f)	// �����ʒu
			, TEXTURE_BUTTON);
		// ������ڎw�����W�̐ݒ�
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// �A�j���[�V�����p�ɍ��W��ۑ�
		m_CommandInfo.commandList.middleAttack[j].vAnimationPosDest = D3DXVECTOR3(fPosDestX, fPosY, 0.0f);

		// �\���͂��Ȃ�
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);

		// �I�[�t���O�̐ݒ�
		m_CommandInfo.commandList.middleAttack[j].isEndList = false;
		if (j == COMMAND_INPUT_NUM_MIDDLE - 2)
		{
			m_CommandInfo.commandList.middleAttack[j].isEndList = true;
		}
	}

	// �R�}���h��\�L���邽�߂�Y���W�̐ݒ�
	fPosY += NEXT_UI_Y_POS_ADD;

	// ��Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		m_CommandInfo.commandList.largeAttack[j].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
			, BUTTON_TYPE_1
			, D3DXVECTOR3(fPosX, fPosY, 0.0f)	// �����ʒu
			, TEXTURE_BUTTON);
		// ������ڎw�����W�̐ݒ�
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// �A�j���[�V�����p�ɍ��W��ۑ�
		m_CommandInfo.commandList.largeAttack[j].vAnimationPosDest = D3DXVECTOR3(fPosDestX, fPosY, 0.0f);

		// �\���͂��Ȃ�
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);

		// �I�[�t���O�̐ݒ�
		m_CommandInfo.commandList.largeAttack[j].isEndList = false;
		if (j == COMMAND_INPUT_NUM_LARGE - 2)
		{
			m_CommandInfo.commandList.largeAttack[j].isEndList = true;
		}
	}
}

//-----------------------------------------------------------------------------
// ���ɓ��͂�����̃R�}���h�̃��Z�b�g
//-----------------------------------------------------------------------------
void CCommandChart::ResetNextCommand(void)
{
	switch (m_aCommandKeep)
	{
		// Q�������͉E���̏�{�^���ɑΉ�
	case BUTTON_TYPE_1:
		CreateRightUpTechnicCommand();
		break;
		// W�������͉E���̉��{�^���ɑΉ�
	case BUTTON_TYPE_2:
		m_pCommandManager->SetCommandChartMode(MY_ID_1, MODE_ROPE);
		m_pCommandManager->SetCommandChartMode(MY_ID_2, MODE_ROPE);
		break;
		// A�������͍����̏�{�^���ɑΉ�
	case BUTTON_TYPE_3:
		CreateLeftUpTechnicCommand();
		break;
		// S�������͍����̉��{�^���ɑΉ�
	case BUTTON_TYPE_4:
		CreateLeftDownTechnicCommand();
		break;
	default:
		break;
	}
	
	// �擪�R�}���h�̏���
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (i == m_aCommandKeep - 1)
		{
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(true);
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(m_aCommandKeep, true);
			continue;
		}
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
	}

	//--------
	// m_apCommandName�̍��W�ƋZ���e�N�X�`���ς���

	//--------
}

//-----------------------------------------------------------------------------
// �n���R�}���h�����̏�ԂɂȂ�
//-----------------------------------------------------------------------------
void CCommandChart::ResetCommandList(void)
{
	// �R�}���h�ێ����̑���
	m_nKeepCommandNum = 0;

	// �`�悷��x���W�̍X�V
	m_fPosX = 0;

	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(type, false);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(false);
	}

	// �R�}���h���X�g�̏�����
	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);

		// ���͏�����
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetInputFlag(false);
	}

	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);

		// ���͏�����
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetInputFlag(false);
	}

	// ��Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);

		// ���͏�����
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetInputFlag(false);
	}

	// �Z���\���|���S�����������W��
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

//-----------------------------------------------------------------------------
// �ʒu��e�N�X�`���Ȃǂ̉�����Ăׂ鏉����
//-----------------------------------------------------------------------------
void CCommandChart::ResetAllCommand(void)
{

	// �R�}���h�ێ����̑���
	m_nKeepCommandNum = 0;

	// �`�悷��x���W�̍X�V
	m_fPosX = 0;

	// �n���R�}���h�̏�����
	float fPosX = 0.0f;
	float fPosDestX = 0.0f;
	float fPosY = 0.f;

	// �ڕW�̍��W
	// �v���C���[�P�̎��̕\��X���W
	if (m_MyID == MY_ID_1)
	{
		fPosX = NEXT_UI_X_POS;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == MY_ID_2)
	{
		fPosX = SCREEN_WIDTH - NEXT_UI_X_POS;
	}

	D3DXVECTOR3 pos(fPosX, UI_Y_POSITION, 0);
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(type, false);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
// ����ȂɁ@��߂�
//		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDestPos(pos);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(false);
		pos.y += NEXT_UI_Y_POS_ADD;
	}
	
	// �R�}���h���X�g�̏�����

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

	fPosY = UI_Y_POSITION;

	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		// ������ڎw�����W�̐ݒ�
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// �\���͂��Ȃ�
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);

		// ���͏�����
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetInputFlag(false);
	}

	// �R�}���h��\�L���邽�߂�Y���W�̐ݒ�
	fPosY += NEXT_UI_Y_POS_ADD;

	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		// ������ڎw�����W�̐ݒ�
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// �\���͂��Ȃ�
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);

		// ���͏�����
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetInputFlag(false);
	}

	// �R�}���h��\�L���邽�߂�Y���W�̐ݒ�
	fPosY += NEXT_UI_Y_POS_ADD;

	// ��Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
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

		// ������ڎw�����W�̐ݒ�
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// �\���͂��Ȃ�
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);

		// ���͏�����
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetInputFlag(false);
	}

	// �Z���\���|���S�����������W��
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

//-----------------------------------------------------------------------------
// �R�}���h�`���[�g�\��
//-----------------------------------------------------------------------------
void CCommandChart::AppearanceCommandChart(void)
{
	// �Z���\���pUI�̏������W�̐ݒ�
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		// �������̋Z���\���pUI��`��I��
		m_apCommandName[i]->SetDrawFlag(true);

		//--------
		// m_apCommandName�̍��W�ƋZ���e�N�X�`���ς���

		//--------
	}

	// �A�j���[�V�����̊J�n
	StartAnimeOpen();
}

//-----------------------------------------------------------------------------
// �R�}���h�`���[�g����
//-----------------------------------------------------------------------------
void CCommandChart::VanishCommandChart(void)
{
	// ����A�j���[�V�����J�n
	StartAnimeClose();
}

//-----------------------------------------------------------------------------
// �R�}���h����
//-----------------------------------------------------------------------------
void CCommandChart::VanishCommand(void)
{
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
	}

	// �R�}���h���X�g�̏�����
	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);
	}

	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);
	}

	// ��Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);
	}
}
//*******************�ǋL�I��11/23�@��K **************************************


//-----------------------------------------------------------------------------
// �A�j���[�V�����p�̏������֐� ���̃N���X��Init�ŌĂ�
//-----------------------------------------------------------------------------
void CCommandChart::InitAnime()
{
	m_isAnime = false;
	m_isAnimeOpen = false;
	m_AnimeCount = 0;
}

//-----------------------------------------------------------------------------
// �X�V�ŌĂԁB�A�j���[�V�����̍X�V�܂Ƃ�
//-----------------------------------------------------------------------------
void CCommandChart::UpdateAnime()
{
	// �A�j���[�V���������[
	if (!m_isAnime)	return;

	// �A�j���[�V�������x
	m_AnimeCount += ANIME_SPEED;

	// �A�j���I���
	if (m_AnimeCount > 1.0f)
	{
		// �A�j���t���OOFF
		m_isAnime = false;
		// �R�}���h�`���[�g�̍X�V���[�h��ύX
		if (m_isAnimeOpen)	m_CommandChartMode = MODE_INPUT;
		else				m_CommandChartMode = MODE_MAX;
	}
	else
	{
		float width, height;
		D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
		// �w�i�̃|���S��
		width = m_Back.GetEasingWidth(m_AnimeCount);
		height = m_Back.GetEasingHeight(m_AnimeCount);
		m_pBackPolygon->SetVertexPolygon(m_Back.m_Pos, width, height);

		// ���U��
		for (int j = 0; j < MAX_COMAND_NUM; j++)
		{
			width = m_CommandSmall[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandSmall[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandSmall[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandSmall[j].GetEasingPosY(m_AnimeCount);
			m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetVertexPolygon(pos, width, height);
		}
		// ���U��
		for (int j = 0; j < MAX_COMAND_NUM; j++)
		{
			width = m_CommandMiddle[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandMiddle[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandMiddle[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandMiddle[j].GetEasingPosY(m_AnimeCount);
			m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetVertexPolygon(pos, width, height);
		}
		// ��U��
		for (int j = 0; j < MAX_COMAND_NUM; j++)
		{
			width = m_CommandLarge[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandLarge[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandLarge[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandLarge[j].GetEasingPosY(m_AnimeCount);
			m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetVertexPolygon(pos, width, height);
		}
		// �ŏ��̃R�}���h
		for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
		{
			width = m_CommandFirst[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandFirst[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandFirst[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandFirst[j].GetEasingPosY(m_AnimeCount);
			m_CommandInfo.beginCommand.firstCommand[j].pCommandUI->SetVertexPolygon(pos, width, height);
		}
		// �Z��
		for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
		{
			width = m_CommandName[j].GetEasingWidth(m_AnimeCount);
			height = m_CommandName[j].GetEasingHeight(m_AnimeCount);
			pos.x = m_CommandName[j].GetEasingPosX(m_AnimeCount);
			pos.y = m_CommandName[j].GetEasingPosY(m_AnimeCount);
			m_apCommandName[j]->SetVertexPolygon(pos, width, height);
		}
	}
}

//-----------------------------------------------------------------------------
//	�J���A�j���[�V�����J�n
// �e�|���S���̃A�j���[�V�����p�̒l�����߂�
//-----------------------------------------------------------------------------
void CCommandChart::StartAnimeOpen(void)
{
	// �A�j���[�V�������Ă���Ƃ��͉������Ȃ�
	if (m_isAnime)
	{
		return;
	}

	// �Ȃ񂩏�����
	ResetAllCommand();

	// �A�j���[�V�����ɕK�v�ȕϐ�������
	m_isAnime = true;
	m_isAnimeOpen = true;
	m_AnimeCount = 0;
	// �ꎞ�ϐ�
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 posDest = D3DXVECTOR3(0, 0, 0);

	// �o���ʒu�͋��ʂŔw�i�|���S���̍��W
	pos = m_MyID == MY_ID_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
	// �w�i�̃|���S��
	posDest = m_MyID == MY_ID_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
	m_Back.Init(pos, posDest, 0, BACK_POLYGON_X_SIZE, 0, BACK_POLYGON_Y_SIZE);

	// ���Z
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.commandList.smallAttack[j].vAnimationPosDest;
		m_CommandSmall[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// ���Z
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.commandList.middleAttack[j].vAnimationPosDest;
		m_CommandMiddle[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// ��Z
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.commandList.largeAttack[j].vAnimationPosDest;
		m_CommandLarge[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// �ŏ��̃R�}���h
	for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
	{
		posDest = m_CommandInfo.beginCommand.firstCommand[j].vAnimationPosDest;
		m_CommandFirst[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// �Z��
	for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
	{
		if (m_MyID == MY_ID_1)
		{
			posDest = D3DXVECTOR3(UI_X_POSITION + UI_X_POS_ADD + UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*j), 0.0f);
		}
		else if (m_MyID == MY_ID_2)
		{
			posDest = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*j), 0.0f);
		}
		m_CommandName[j].Init(pos, posDest, 0, COMMAND_NAME_POLYGON_WIDTH, 0, COMMAND_NAME_POLYGON_HEIGHT);
	}
}

//-----------------------------------------------------------------------------
//	����A�j���[�V�����J�n
// �e�|���S���̃A�j���[�V�����p�̒l�����߂�
//-----------------------------------------------------------------------------
void CCommandChart::StartAnimeClose(void)
{
	// �A�j���[�V�������Ă���Ƃ��͉������Ȃ�
	if (m_isAnime)	return;

	// �A�j���[�V�����ɕK�v�ȕϐ�������
	m_isAnime = true;
	m_isAnimeOpen = false;
	m_AnimeCount = 0;
	// �ꎞ�ϐ�
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 posDest = D3DXVECTOR3(0, 0, 0);

	// ���ׂĔw�i�|���S���̍��W�ɏ�����
	posDest = m_MyID == MY_ID_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;

	// �w�i�̃|���S��
	pos = m_MyID == MY_ID_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
	m_Back.Init(pos, posDest, BACK_POLYGON_X_SIZE, 0, BACK_POLYGON_Y_SIZE, 0);
	// ���Z
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.commandList.smallAttack[j].vAnimationPosDest;
		m_CommandSmall[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}
	// ���Z
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.commandList.middleAttack[j].vAnimationPosDest;
		m_CommandMiddle[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}
	// ��Z
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.commandList.largeAttack[j].vAnimationPosDest;
		m_CommandLarge[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}

	// �ŏ��̃R�}���h
	for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
	{
		pos = m_CommandInfo.beginCommand.firstCommand[j].vAnimationPosDest;
		m_CommandFirst[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}
	// �Z��
	for (int j = 0; j < MAX_NEXT_COMMAND_VIEW; j++)
	{
		pos = m_apCommandName[j]->GetPos();
		m_CommandName[j].Init(pos, posDest, COMMAND_NAME_POLYGON_WIDTH, 0, COMMAND_NAME_POLYGON_HEIGHT, 0);
	}
}

//-----------------------------------------------------------------------------
// ���[�v�R�}���h���Z�b�g
//-----------------------------------------------------------------------------
void CCommandChart::SetRopeCommand(void)
{
	// ���Z�b�g
	ResetAllCommand();
	m_aCommandKeep = BUTTON_TYPE_2;

	// �R�}���h�ێ����̑���
	m_nKeepCommandNum++;

	// �`�悷��x���W�̍X�V
	m_fPosX += UI_X_POS_ADD;

	// �ŏ��̃R�}���h���͂̎��̏���
	if (m_nKeepCommandNum == 1)
	{
		// �������̋Z���\���pUI�̖ڕW���W�̐ݒ�
		// �Z���\���pUI�̏������W�̐ݒ�
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (m_MyID == MY_ID_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3((UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) + UI_X_POSITION, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == MY_ID_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - (UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) - UI_X_POSITION - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
		}
		CreateRightDownTechnicCommand();

		// �擪�R�}���h�̏���
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			if (i == m_aCommandKeep - 1)
			{
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(true);
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(m_aCommandKeep, true);
				continue;
			}
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
		}
	}
	m_aCommandKeep = BUTTON_TYPE_NONE;
}

// EOF