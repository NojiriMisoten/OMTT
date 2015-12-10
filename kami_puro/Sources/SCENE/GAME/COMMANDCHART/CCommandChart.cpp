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
static const float UI_Y_POSITION = 350.0f;		// UI�̏����\�����W(y���W)
static const float UI_X_POS_ADD = 30.0f;		// �P�\�����UI�̍��W�̕ω��̒l
static const float NEXT_UI_X_POS = 50.0f;		// ���ɓ��͂���R�}���h�̈�ԏ��UI�̍��W
static const float NEXT_UI_Y_POS = 350.0f;		// ���ɓ��͂���R�}���h�̈�ԏ��UI�̍��W
static const float NEXT_UI_X_POS_ADD = COMMAND_POLYGON_WIDTH * 1.0f;	// ���ɓ��͂���R�}���h��UI�̍��W�̕ω��̒l
static const float NEXT_UI_Y_POS_ADD = 33.0f * (SCREEN_HEIGHT * 0.5f / 150.f);	// ���ɓ��͂���R�}���h��UI�̍��W�̕ω��̒l
static const float COMMAND_NAME_ADD_NUM = 7.0f;	// �R�}���h�l�[���̕ω��̒l�̐�
static const float FADE_UI_OUT_POS_X_ID_1 = -50.0f;					//�t�F�[�h�A�E�g���̖ڕW���W������ID�P
static const float FADE_UI_OUT_POS_X_ID_2 = SCREEN_WIDTH + 50.0f;	//�t�F�[�h�A�E�g���̖ڕW���W������ID�Q
static const float BACK_POLYGON_X_SIZE = ((COMMAND_POLYGON_WIDTH * MAX_COMMAND_KEEP) + (UI_X_POS_ADD * MAX_COMMAND_KEEP));	// �R�}���h�`���[�gUI�̔w��ɕ\������|���S����X�̃T�C�Y
static const float BACK_POLYGON_Y_SIZE = SCREEN_HEIGHT * 0.6f;	// �R�}���h�`���[�gUI�̔w��ɕ\������|���S����Y�̃T�C�Y
static const int COMMAND_DEATH_COUNT = 60;		// �R�}���h�����܂ł̃J�E���g
static const int COMMAND_WEAK_ATTACK_COMMAND_ARRAY_NUM = 0;	// ��U���̃R�}���h�̔z��ԍ�
static const int COMMAND_NORMAL_ATTACK_COMMAND_ARRAY_NUM = 1;	// ���U���̃R�}���h�̔z��ԍ�
static const int COMMAND_STRONG_ATTACK_COMMAND_ARRAY_NUM = 2;	// ��U���̃R�}���h�̔z��ԍ�
static const D3DXVECTOR3 BACK_POLYGON_POS_1 = D3DXVECTOR3((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION * 1.3f - (COMMAND_POLYGON_WIDTH - 2.0f), UI_Y_POSITION + (40.0f * (SCREEN_HEIGHT * 0.5f / 150.f)*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// �w��̃|���S���̍��WID1
static const D3DXVECTOR3 BACK_POLYGON_POS_2 = D3DXVECTOR3(SCREEN_WIDTH - ((BACK_POLYGON_X_SIZE / 2.0f) + UI_X_POSITION * 1.3f - (COMMAND_POLYGON_WIDTH - 2.0f)), UI_Y_POSITION + (40.0f * (SCREEN_HEIGHT * 0.5f / 150.f)*(MAX_NEXT_COMMAND_VIEW / 2.0f)) - (COMMAND_POLYGON_HEIGHT / 2.0f), 0.0f);	// �w��̃|���S���̍��W
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

// �n���R�}���h�̃e�N�X�`���̔z��
static const TEXTURE_TYPE BEGIN_COMMAND_TEXTURE_TYPE[CCommandChart::MAX_BEGIN_COMAND_NUM]
{
	TEXTURE_NUMBER,
	TEXTURE_NUMBER,
	TEXTURE_NUMBER,
	TEXTURE_NUMBER,
	TEXTURE_NUMBER,
};

// ����n���R�}���h�̃e�N�X�`���̔z��
static const TEXTURE_TYPE LEFT_UP_COMMAND_TEXTURE_TYPE[SKILL_MAX]
{
	TEXTURE_SKILL_NAME_CHOP,
	TEXTURE_SKILL_NAME_ELBOW,
	TEXTURE_SKILL_NAME_LARIAT,
};
// �E��n���R�}���h�̃e�N�X�`���̔z��
static const TEXTURE_TYPE RIGHT_UP_COMMAND_TEXTURE_TYPE[SKILL_MAX]
{
	TEXTURE_SKILL_NAME_ROLLING,
	TEXTURE_SKILL_NAME_SHOULDER,
	TEXTURE_SKILL_NAME_DROPKICK,
};

// �����n���R�}���h�̃e�N�X�`���̔z��
static const TEXTURE_TYPE LEFT_DOWN_COMMAND_TEXTURE_TYPE[SKILL_MAX]
{
	TEXTURE_SKILL_NAME_SLAP,
	TEXTURE_SKILL_NAME_BACKDROP,
	TEXTURE_SKILL_NAME_STUNNER,
};

// �E���n���R�}���h�̃e�N�X�`���̔z��
static const TEXTURE_TYPE RIGHT_DOWN_COMMAND_TEXTURE_TYPE[SKILL_MAX]
{
	TEXTURE_SKILL_NAME_LARIAT,
	TEXTURE_SKILL_NAME_STUNNER,
	TEXTURE_SKILL_NAME_DROPKICK,
};

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* pDevice, PLAYER_ID nID, CCommandChartManager* pCommandManager)
{
	// �f�o�C�X�̃|�C���^��ۑ�
	m_pD3DDevice = pDevice;

	// ���͂��ꂽ�R�}���h�ێ��p�ϐ��̏�����
	m_aCommandKeep = BUTTON_TYPE_NONE;

	// ���g�̃v���C���[�ԍ���ݒ�
	m_MyID = nID;
	
	// �w��ɕ\������|���S���̍쐬
	m_pBackPolygon = new CScene2D(pDevice);
	if (m_MyID == PLAYER_1)
	{
		m_pBackPolygon->Init((D3DXVECTOR3 &)BACK_POLYGON_POS_1, BACK_POLYGON_X_SIZE, BACK_POLYGON_Y_SIZE, TEXTURE_COMMAND_CHART_BACK);
	}
	else if (m_MyID == PLAYER_2)
	{
		m_pBackPolygon->Init((D3DXVECTOR3 &)BACK_POLYGON_POS_2, BACK_POLYGON_X_SIZE, BACK_POLYGON_Y_SIZE, TEXTURE_COMMAND_CHART_BACK);
	}
	m_pBackPolygon->AddLinkList(CRenderer::TYPE_RENDER_UI);
	m_pBackPolygon->SetDrawFlag(false);

	m_CommandChartMode = MODE_MAX;

	m_DestCompleteCommand = m_CompleteCommand = COMMAND_TYPE_NONE;

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
CCommandChart* CCommandChart::Create(LPDIRECT3DDEVICE9* pDevice, PLAYER_ID nID, CCommandChartManager* pCommandManager)
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
	for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
	{
		if (m_MyID == PLAYER_1)
		{
			pos = D3DXVECTOR3(UI_X_POSITION * 5.f + UI_X_POS_ADD + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}
		else if (m_MyID == PLAYER_2)
		{
			pos = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION * 5.f - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}

		// �������̋Z���\���pUI���쐬
		m_apCommandName[i] = CCommandName::Create(m_pD3DDevice, pos, BEGIN_COMMAND_TEXTURE_TYPE[i]);
		m_apCommandName[i]->SetDrawFlag(false);
	}

	// �ŏ��̃R�}���h�쐬 �\���͂��Ȃ�
	InitCreateBeginCommand();

	// �ŏ��ȊO�̃R�}���h�쐬 �\���͂��Ȃ�
	InitCreateCommandList();

	// �J�����������A�j���̏����������̂܂Ƃ�
	InitAnime();

	for (int i = 0; i < MAX_KEEP_COMMAND_NUM; i++)
	{
		m_KeepButton[i].buttonType = BUTTON_TYPE_NONE;
		m_KeepButton[i].isUse = false;
	}
	m_WiatCompleteCommandTimer = 0;
	m_CompleteSkill = SKILL_MAX;
}

//-----------------------------------------------------------------------------
//	�X�V����
//-----------------------------------------------------------------------------
void CCommandChart::Update(void)
{
#ifdef _DEBUG
	if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE_UI_COMMAND_ANIME_CLOSE))
	{
		m_CommandChartMode = MODE_RESET;
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

			// FINISH�Z�n���{�^���\��(���ŕ\�����邩�̔��肵�Ă�)
			isAppearFinishBeginCommand();

			// FINISH�Z���
			bool isCreateFinishCommand = (!m_CommandInfo.beginCommand.firstCommand[4].pCommandUI->GetInputFlag())
											&& (GetSameTimePushButton(BUTTON_TYPE_5)
											&& m_pCommandManager->GetCanUseFinishSkill(m_MyID));
			if (isCreateFinishCommand)
			{
				CreateFinishTechnicCommand();
			}
			else
			{
				// �R�}���h�̃`�F�b�N
				CheckCommand();
			}
		}
		break;

	case MODE_PRE_COMPLETE_COMMAND:
		if (m_WiatCompleteCommandTimer < 0)
		{
			return;
		}
		if (m_WiatCompleteCommandTimer >= MAX_KEEP_COMMAND_NUM)
		{
			m_CommandChartMode = MODE_COMPLETE_COMMAND;
			break;
		}
		else
		{
			// ���͊m�F
			CheckPushCommand();

			// FINISH�����m�F
			CheckPushForFinishCommand();

			if (isCompleteFinishSkill())
			{
				m_DestCompleteCommand = COMMAND_TYPE_FINISHER;
			}
		}
		m_WiatCompleteCommandTimer++;
		break;

	case MODE_COMPLETE_COMMAND:

		if (m_WiatCompleteCommandTimer >= MAX_KEEP_COMMAND_NUM)
		{
			VanishOtherSkill(m_CompleteSkill);
			m_CompleteCommand = m_DestCompleteCommand;
			m_DestCompleteCommand = COMMAND_TYPE_NONE;
			m_CompleteSkill = SKILL_MAX;
			m_WiatCompleteCommandTimer = -1;
			// �R�}���h�`���[�g�̌��̃|���S��������
			AllCandidateInputBackPolygonVanish();
		}

		break;
		
	case MODE_VANISH:
		// �R�}���h�`���[�g������
		VanishCommandChart();
		m_CompleteCommand = COMMAND_TYPE_NONE;
		m_CommandChartMode = MODE_MAX;
		break;

	case MODE_RESET:
		// ���Z�b�g
		ResetCommandList();
		m_CommandChartMode = MODE_INPUT;
		m_WiatCompleteCommandTimer = 0;
		m_CompleteCommand = COMMAND_TYPE_NONE;
		break;
	
	case MODE_ROPE:
		SetRopeCommand();
		m_CommandChartMode = MODE_INPUT;
		break;

	default:
		break;
	}

	m_aCommandKeep = BUTTON_TYPE_NONE;
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
	CheckPushCommand();

	// ����������ĂȂ��Ȃ�I��
	if (m_aCommandKeep == BUTTON_TYPE_NONE)
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
		for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
		{
			if (m_MyID == PLAYER_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3((UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) + UI_X_POSITION * 4.5f, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == PLAYER_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - (UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) - UI_X_POSITION * 4.f - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
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
	SKILL_TYPE type = SKILL_MAX;
	bool isRope = false;

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

	// FINISH�Z�̔���
	if (isCompleteFinishSkill())
	{
		type = SKILL_FINISH_ATTACK;
	}

	// �����������Ă��Ȃ��Ȃ�Ԃ�
	if (type == SKILL_MAX)
	{
		return;
	}

	// FINISH�Z�ŗD��
	if (type == SKILL_FINISH_ATTACK)
	{
		// ��Ԃ�������Ԃ�
		m_CommandChartMode = MODE_PRE_COMPLETE_COMMAND;
		m_DestCompleteCommand = COMMAND_TYPE_FINISHER;
		m_CompleteSkill = type;
		return;
	}

	// ������Ă�n���Z�����ޔ���
	int idx = -1;
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
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
		else if (buttonType == BUTTON_TYPE_4)
		{
			idx = 2;
		}
		else if (buttonType == BUTTON_TYPE_2)
		{
			idx = 0;
			isRope = true;
		}
		break;
	}
	if (idx == -1)
	{
		return;
	}

	if (!isRope)
	{
		// �R�}���h����
		switch (type)
		{
		case SKILL_SMALL_ATTACK:
			m_DestCompleteCommand = COMMAND_SMALL_TECHNIQUE_LIST[idx]->m_CommandType;
			break;

		case SKILL_MIDDLE_ATTACK:
			m_DestCompleteCommand = COMMAND_MIDDLE_TECHNIQUE_LIST[idx]->m_CommandType;
			break;

		case SKILL_BIG_ATTACK:
			m_DestCompleteCommand = COMMAND_LARGE_TECHNIQUE_LIST[idx]->m_CommandType;
			break;

		default:
			return;
			break;
		}
	}
	else
	{
		if (type < SKILL_MAX)
		{
			m_DestCompleteCommand = COMMAND_RIGHT_DOWN_TECHNIQUE_LIST[type]->m_CommandType;
		}
	}
	m_CompleteSkill = type;

	// ��Ԃ�������Ԃ�
	m_CommandChartMode = MODE_PRE_COMPLETE_COMMAND;
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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
			m_CommandInfo.commandList.largeAttack[j].isEndList = true;
		}

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}

	// �Z���ύX
	for (int j = 0; j < SKILL_MAX; j++)
	{
		m_apCommandName[j]->ChangeTexture(RIGHT_UP_COMMAND_TEXTURE_TYPE[j]);
	}

	// 4�߂͂Ȃ��̂Ŗ��O����
	m_apCommandName[MAX_NEXT_COMMAND_VIEW - 1]->SetDrawFlag(false);
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
		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}

	// �Z���ύX
	for (int j = 0; j < SKILL_MAX; j++)
	{
		m_apCommandName[j]->ChangeTexture(LEFT_UP_COMMAND_TEXTURE_TYPE[j]);
	}
	// 4�߂͂Ȃ��̂Ŗ��O����
	m_apCommandName[MAX_NEXT_COMMAND_VIEW - 1]->SetDrawFlag(false);
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}

	// �Z���ύX
	for (int j = 0; j < SKILL_MAX; j++)
	{
		m_apCommandName[j]->ChangeTexture(LEFT_DOWN_COMMAND_TEXTURE_TYPE[j]);
	}

	// �S�߂͂Ȃ��̂Ŗ��O����
	m_apCommandName[MAX_NEXT_COMMAND_VIEW - 1]->SetDrawFlag(false);
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.smallAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.middleAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
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

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.largeAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}

	// �Z���ύX
	for (int j = 0; j < SKILL_MAX; j++)
	{
		m_apCommandName[j]->ChangeTexture(RIGHT_DOWN_COMMAND_TEXTURE_TYPE[j]);
	}

	// �S�߂͂Ȃ��̂Ŗ��O����
	m_apCommandName[MAX_NEXT_COMMAND_VIEW - 1]->SetDrawFlag(false);
	//*******************�ύX�I��11/23�@��K **************************************
}

//-----------------------------------------------------------------------------
// FINISH�R�}���h�`���[�g�̐���
//-----------------------------------------------------------------------------
void CCommandChart::CreateFinishTechnicCommand(void)
{
	// �擪�͏����̂łP����n�߂�
	for (int j = 1; j < COMMAND_FINISHER.m_nCommandLength; j++)
	{
		// UV�l�ݒ�
		m_CommandInfo.commandList.finishAttack[j - 1].pCommandUI->InputUIUVChange(COMMAND_BUTTON_FINISHER[j], false);

		// �`��ON
		m_CommandInfo.commandList.finishAttack[j - 1].pCommandUI->SetDrawFlag(true);

		m_CommandInfo.commandList.finishAttack[j - 1].isEndList = false;
		if (j == COMMAND_FINISHER.m_nCommandLength - 1)
		{
			m_CommandInfo.commandList.finishAttack[j - 1].isEndList = true;
		}

		// �R�}���h�̔w�i�̃|���S���\��
		if (j == 1)
		{
			m_CommandInfo.commandList.finishAttack[j - 1].pCommandUI->CandidateInputBackPolygonDraw();
		}

	}
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(true);
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->InputUIUVChange(COMMAND_BUTTON_FINISHER[0], true);
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

			// ���X�g�̍Ō�̃R�}���h�Ŗ�����Ύ��̃R�}���h�Ƀ|���S����\������
			if (m_CommandInfo.commandList.smallAttack[j].pCommandUI->GetInputFlag() &&
				!m_CommandInfo.commandList.smallAttack[j].isEndList)

			{
				m_CommandInfo.commandList.smallAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
				if (j + 1 < MAX_COMAND_NUM)
				{
					m_CommandInfo.commandList.smallAttack[j + 1].pCommandUI->CandidateInputBackPolygonDraw();
				}
			}

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

			// ���X�g�̍Ō�̃R�}���h�Ŗ�����Ύ��̃R�}���h�Ƀ|���S����\������
			if (m_CommandInfo.commandList.middleAttack[j].pCommandUI->GetInputFlag() &&
				!m_CommandInfo.commandList.middleAttack[j].isEndList)

			{
				m_CommandInfo.commandList.middleAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
				if (j + 1 < MAX_COMAND_NUM)
				{
					m_CommandInfo.commandList.middleAttack[j + 1].pCommandUI->CandidateInputBackPolygonDraw();
				}
			}

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

			// ���X�g�̍Ō�̃R�}���h�Ŗ�����Ύ��̃R�}���h�Ƀ|���S����\������
			if (m_CommandInfo.commandList.largeAttack[j].pCommandUI->GetInputFlag() &&
				!m_CommandInfo.commandList.largeAttack[j].isEndList)
			{
				m_CommandInfo.commandList.largeAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
				if (j + 1 < MAX_COMAND_NUM)
				{
					m_CommandInfo.commandList.largeAttack[j + 1].pCommandUI->CandidateInputBackPolygonDraw();
				}
			}

		}
		break;
	}

	// FINISH�Z�̃R�}���h������Ă邩
	CheckPushForFinishCommand();
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
	if (m_MyID == PLAYER_1)
	{
		fPosX = NEXT_UI_X_POS;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == PLAYER_2)
	{
		fPosX = SCREEN_WIDTH - NEXT_UI_X_POS;
	}

	D3DXVECTOR3 pos(fPosX, UI_Y_POSITION, 0);
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
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
	if (m_MyID == PLAYER_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// �v���C���[�Q�̎��̕\��X���W
		else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// �v���C���[�Q�̎��̕\��X���W
		else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// �v���C���[�Q�̎��̕\��X���W
		else if (m_MyID == PLAYER_2)
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

	// �R�}���h��\�L���邽�߂�Y���W�̐ݒ�
	fPosY += NEXT_UI_Y_POS_ADD * 2.f;

	// FINISH�Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �ڕW�̍��W
		// �v���C���[�P�̎��̕\��X���W
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// �v���C���[�Q�̎��̕\��X���W
		else if (m_MyID == PLAYER_2)
		{
			fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
			fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
		}

		m_CommandInfo.commandList.finishAttack[j].pCommandUI = CCommandChartUI::Create(m_pD3DDevice
			, BUTTON_TYPE_1
			, D3DXVECTOR3(fPosX, fPosY, 0.0f)	// �����ʒu
			, TEXTURE_BUTTON);
		// ������ڎw�����W�̐ݒ�
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// �A�j���[�V�����p�ɍ��W��ۑ�
		m_CommandInfo.commandList.finishAttack[j].vAnimationPosDest = D3DXVECTOR3(fPosDestX, fPosY, 0.0f);

		// �\���͂��Ȃ�
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDrawFlag(false);

		// �I�[�t���O�̐ݒ�
		m_CommandInfo.commandList.finishAttack[j].isEndList = false;
		if (j == COMMAND_INPUT_NUM_FINISHER - 2)
		{
			m_CommandInfo.commandList.finishAttack[j].isEndList = true;
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
		// W�������͉E���̏�{�^���ɑΉ�
	case BUTTON_TYPE_1:
		CreateRightUpTechnicCommand();
		break;
		// S�������͉E���̉��{�^���ɑΉ�
	case BUTTON_TYPE_2:
		m_pCommandManager->SetCommandChartMode(PLAYER_1, MODE_ROPE);
		m_pCommandManager->SetCommandChartMode(PLAYER_2, MODE_ROPE);
		m_CompleteCommand = COMMAND_TYPE_ROPE;
		break;
		// Q�������͍����̏�{�^���ɑΉ�
	case BUTTON_TYPE_3:
		CreateLeftUpTechnicCommand();
		break;
		// A�������͍����̉��{�^���ɑΉ�
	case BUTTON_TYPE_4:
		CreateLeftDownTechnicCommand();
		break;
	default:
		break;
	}
	
	// �擪�R�}���h�̏��� FINISH�Z�����͊֌W�Ȃ�
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM - 1; i++)
	{
		if (i == m_aCommandKeep - 1)
		{
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(true);
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(m_aCommandKeep, true);
			D3DXVECTOR3 dest = m_CommandInfo.beginCommand.firstCommand[0].pCommandUI->GetPos();
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->PlaySelectedCommandMove(dest);
			continue;
		}
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
	}
}

//-----------------------------------------------------------------------------
// �n���R�}���h�����̏�ԂɂȂ�
//-----------------------------------------------------------------------------
void CCommandChart::ResetCommandList(void)
{
	m_WiatCompleteCommandTimer = 0;
	// �R�}���h�ێ����̑���
	m_nKeepCommandNum = 0;

	// �`�悷��x���W�̍X�V
	m_fPosX = 0;

	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
	{
		if (i == MAX_BEGIN_COMAND_NUM - 1)
		{
			if (!m_pCommandManager->GetCanUseFinishSkill(m_MyID))
			{
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInitPos();
				continue;
			}
		}
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(type, false);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(false);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInitPos();
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

	// FINISH�Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDrawFlag(false);

		// ���͏�����
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetInputFlag(false);
	}
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(false);

	// �Z���\���|���S�����������W��
	for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
	{
		D3DXVECTOR3 pos;
		if (m_MyID == PLAYER_1)
		{
			pos = D3DXVECTOR3(UI_X_POSITION * 5.f + UI_X_POS_ADD + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}
		else if (m_MyID == PLAYER_2)
		{
			pos = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION * 5.f - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f);
		}

		m_apCommandName[i]->SetDestPos(pos);
		m_apCommandName[i]->SetPos(pos);
		// �������̋Z���\���pUI��`��I��
		m_apCommandName[i]->SetDrawFlag(true);
		m_apCommandName[i]->ChangeTexture(BEGIN_COMMAND_TEXTURE_TYPE[i]);
	}
	
	// �ێ����Ă��R�}���h�j��
	RefleshKeepCommand();
}

//-----------------------------------------------------------------------------
// �ʒu��e�N�X�`���Ȃǂ̉�����Ăׂ鏉����
//-----------------------------------------------------------------------------
void CCommandChart::ResetAllCommand(void)
{
	RefleshKeepCommand();
	m_WiatCompleteCommandTimer = 0;

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
	if (m_MyID == PLAYER_1)
	{
		fPosX = NEXT_UI_X_POS;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == PLAYER_2)
	{
		fPosX = SCREEN_WIDTH - NEXT_UI_X_POS;
	}
	m_pBackPolygon->SetDrawFlag(true);

	D3DXVECTOR3 pos(fPosX, UI_Y_POSITION, 0);
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
	{
		if (i == MAX_BEGIN_COMAND_NUM - 1)
		{
			if (!m_pCommandManager->GetCanUseFinishSkill(m_MyID))
			{
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInitPos();
				continue;
			}
		}
		BUTTON_TYPE type = (BUTTON_TYPE)(i + 1);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(type, false);
		m_CommandInfo.beginCommand.firstCommand[i].isEndList = false;
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(false);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetPos(pos);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDestPos(pos);
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInitPos();
		pos.y += NEXT_UI_Y_POS_ADD;
	}
	
	// �R�}���h���X�g�̏�����

	// �ڕW�̍��W
	// �v���C���[�P�̎��̕\��X���W
	if (m_MyID == PLAYER_1)
	{
		fPosDestX = NEXT_UI_X_POS;
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// �v���C���[�Q�̎��̕\��X���W
		else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// �v���C���[�Q�̎��̕\��X���W
		else if (m_MyID == PLAYER_2)
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
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// �v���C���[�Q�̎��̕\��X���W
		else if (m_MyID == PLAYER_2)
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

	// �R�}���h��\�L���邽�߂�Y���W�̐ݒ�
	fPosY += NEXT_UI_Y_POS_ADD * 2.f;

	// FINISH�Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �ڕW�̍��W
		// �v���C���[�P�̎��̕\��X���W
		if (m_MyID == PLAYER_1)
		{
			fPosDestX = NEXT_UI_X_POS + NEXT_UI_X_POS_ADD + (NEXT_UI_X_POS_ADD * j);
			fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
		}
		// �v���C���[�Q�̎��̕\��X���W
		else if (m_MyID == PLAYER_2)
		{
			fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * j) - NEXT_UI_X_POS_ADD;
			fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
		}

		// ������ڎw�����W�̐ݒ�
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));

		// �\���͂��Ȃ�
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDrawFlag(false);

		// ���͏�����
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetInputFlag(false);
	}
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(false);

	// �Z���\���|���S�����������W��
	for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
	{
		if (m_MyID == PLAYER_1)
		{
			m_apCommandName[i]->SetDestPos(D3DXVECTOR3(UI_X_POSITION * 5.f + UI_X_POS_ADD+ (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
		}
		else if (m_MyID == PLAYER_2)
		{
			m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION * 5.f - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
		}
		m_apCommandName[i]->ChangeTexture(BEGIN_COMMAND_TEXTURE_TYPE[i]);
	}
}

//-----------------------------------------------------------------------------
// �R�}���h�`���[�g�\��
//-----------------------------------------------------------------------------
void CCommandChart::AppearanceCommandChart(void)
{
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
	RefleshKeepCommand();
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
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

	// FINISH�Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetDrawFlag(false);
	}
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(false);
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
	if (m_AnimeCount >= 1.0f)
	{
		m_AnimeCount = 1.0f;
	}
	
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
	// FINISH�U��
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		width = m_CommandFinish[j].GetEasingWidth(m_AnimeCount);
		height = m_CommandFinish[j].GetEasingHeight(m_AnimeCount);
		pos.x = m_CommandFinish[j].GetEasingPosX(m_AnimeCount);
		pos.y = m_CommandFinish[j].GetEasingPosY(m_AnimeCount);
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetVertexPolygon(pos, width, height);
	}
	// �ŏ��̃R�}���h
	for (int j = 0; j < MAX_BEGIN_COMAND_NUM; j++)
	{
		width = m_CommandFirst[j].GetEasingWidth(m_AnimeCount);
		height = m_CommandFirst[j].GetEasingHeight(m_AnimeCount);
		pos.x = m_CommandFirst[j].GetEasingPosX(m_AnimeCount);
		pos.y = m_CommandFirst[j].GetEasingPosY(m_AnimeCount);
		m_CommandInfo.beginCommand.firstCommand[j].pCommandUI->SetVertexPolygon(pos, width, height);
	}
	// �Z��
	for (int j = 0; j < MAX_COMAND_NAME_NUM; j++)
	{
		width = m_CommandName[j].GetEasingWidth(m_AnimeCount);
		height = m_CommandName[j].GetEasingHeight(m_AnimeCount);
		pos.x = m_CommandName[j].GetEasingPosX(m_AnimeCount);
		pos.y = m_CommandName[j].GetEasingPosY(m_AnimeCount);
		m_apCommandName[j]->SetVertexPolygon(pos, width, height);
	}
	
	// �A�j���I���
	if (m_AnimeCount >= 1.0f)
	{
		// �A�j���t���OOFF
		m_isAnime = false;
		// �R�}���h�`���[�g�̍X�V���[�h��ύX
		if (m_isAnimeOpen)	m_CommandChartMode = MODE_INPUT;
		else				m_CommandChartMode = MODE_MAX;
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

	// �Z���\���pUI�̏������W�̐ݒ�
	for (int i = 0; i < MAX_COMAND_NAME_NUM - 1; i++)
	{
		// �������̋Z���\���pUI��`��I��
		m_apCommandName[i]->SetDrawFlag(true);
	}

	// �A�j���[�V�����ɕK�v�ȕϐ�������
	m_isAnime = true;
	m_isAnimeOpen = true;
	m_AnimeCount = 0;
	// �ꎞ�ϐ�
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 posDest = D3DXVECTOR3(0, 0, 0);

	// �o���ʒu�͋��ʂŔw�i�|���S���̍��W
	pos = m_MyID == PLAYER_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
	// �w�i�̃|���S��
	posDest = m_MyID == PLAYER_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
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
	// FINISH�Z
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.commandList.finishAttack[j].vAnimationPosDest;
		m_CommandFinish[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// �ŏ��̃R�}���h
	for (int j = 0; j < MAX_BEGIN_COMAND_NUM; j++)
	{
		posDest = m_CommandInfo.beginCommand.firstCommand[j].vAnimationPosDest;
		m_CommandFirst[j].Init(pos, posDest, 0, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT);
	}
	// �Z��
	for (int j = 0; j < MAX_COMAND_NAME_NUM; j++)
	{
		if (m_MyID == PLAYER_1)
		{
			posDest = D3DXVECTOR3(UI_X_POSITION * 5.f + UI_X_POS_ADD + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*j), 0.0f);
		}
		else if (m_MyID == PLAYER_2)
		{
			posDest = D3DXVECTOR3(SCREEN_WIDTH - UI_X_POS_ADD - UI_X_POSITION * 5.f - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*j), 0.0f);
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
	posDest = m_MyID == PLAYER_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;

	// �w�i�̃|���S��
	pos = m_MyID == PLAYER_1 ? BACK_POLYGON_POS_1 : BACK_POLYGON_POS_2;
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
	// FINISh�Z
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.commandList.finishAttack[j].vAnimationPosDest;
		m_CommandFinish[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}

	// �ŏ��̃R�}���h
	for (int j = 0; j < MAX_BEGIN_COMAND_NUM; j++)
	{
		pos = m_CommandInfo.beginCommand.firstCommand[j].vAnimationPosDest;
		m_CommandFirst[j].Init(pos, posDest, COMMAND_POLYGON_WIDTH, 0, COMMAND_POLYGON_HEIGHT, 0);
	}
	// �Z��
	for (int j = 0; j < MAX_COMAND_NAME_NUM; j++)
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
		for (int i = 0; i < MAX_COMAND_NAME_NUM; i++)
		{
			if (m_MyID == PLAYER_1)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3((UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) + UI_X_POSITION * 4.5f, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
			else if (m_MyID == PLAYER_2)
			{
				m_apCommandName[i]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - (UI_X_POS_ADD * COMMAND_NAME_ADD_NUM) - UI_X_POSITION * 4.f - m_fPosX, UI_Y_POSITION + (NEXT_UI_Y_POS_ADD*i), 0.0f));
			}
		}
		CreateRightDownTechnicCommand();

		// �擪�R�}���h�̏���
		for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
		{
			if (i == m_aCommandKeep - 1)
			{
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetInputFlag(true);
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(true);
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->InputUIUVChange(m_aCommandKeep, true);
				D3DXVECTOR3 dest = m_CommandInfo.beginCommand.firstCommand[0].pCommandUI->GetPos();
				m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->PlaySelectedCommandMove(dest);
				continue;
			}
			m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->SetDrawFlag(false);
		}
	}
	m_aCommandKeep = BUTTON_TYPE_NONE;
}

//-----------------------------------------------------------------------------
// �Z�������I������ĂȂ��Z������
//-----------------------------------------------------------------------------
void CCommandChart::VanishOtherSkill(SKILL_TYPE completeSkill)
{
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		if (completeSkill != SKILL_BIG_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
		{
			m_CommandInfo.commandList.largeAttack[j].pCommandUI->SetDrawFlag(false);
		}
		if (completeSkill != SKILL_MIDDLE_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
		{
			m_CommandInfo.commandList.middleAttack[j].pCommandUI->SetDrawFlag(false);
		}
		if (completeSkill != SKILL_SMALL_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
		{
			m_CommandInfo.commandList.smallAttack[j].pCommandUI->SetDrawFlag(false);
		}
	}
	if (completeSkill != SKILL_BIG_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
	{
		m_apCommandName[SKILL_BIG_ATTACK]->SetDrawFlag(false);
	}
	if (completeSkill != SKILL_MIDDLE_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
	{
		m_apCommandName[SKILL_MIDDLE_ATTACK]->SetDrawFlag(false);
	}
	if (completeSkill != SKILL_SMALL_ATTACK || m_DestCompleteCommand == COMMAND_TYPE_FINISHER)
	{
		m_apCommandName[SKILL_SMALL_ATTACK]->SetDrawFlag(false);
	}
}

//-----------------------------------------------------------------------------
// �����ꂽ�{�^���ێ��֐�
//-----------------------------------------------------------------------------
void CCommandChart::KeepPushButton(bool isPushRightUp, bool isPushRightDown, bool isPushLeftUp, bool isPushLeftDown)
{
	int index = 0;

	// �S���g���Ă鎞�̏���
	if (m_KeepButton[MAX_KEEP_COMMAND_NUM - 1].isUse)
	{
		for (; index < MAX_KEEP_COMMAND_NUM; index++)
		{
			if (index >= MAX_KEEP_COMMAND_NUM - 1)
			{
				m_KeepButton[index].buttonType = BUTTON_TYPE_NONE;
				break;
			}
			m_KeepButton[index].buttonType = m_KeepButton[index + 1].buttonType;
		}
	}
	// �󂫂�����Ȃ�
	else
	{
		for (; index < MAX_KEEP_COMMAND_NUM; index++)
		{
			if (!m_KeepButton[index].isUse)
			{
				break;
			}
		}
	}

	BUTTON_TYPE keepButton = BUTTON_TYPE_NONE;
	// �i�[�R�}���h����
	if (isPushRightUp && isPushLeftUp)
	{
		keepButton = BUTTON_TYPE_5;
	}
	else if (isPushRightDown && isPushLeftDown)
	{
		keepButton = BUTTON_TYPE_6;
	}
	else if (isPushRightUp)
	{
		keepButton = BUTTON_TYPE_1;
	}
	else if (isPushRightDown)
	{
		keepButton = BUTTON_TYPE_2;
	}
	else if (isPushLeftUp)
	{
		keepButton = BUTTON_TYPE_3;
	}
	else if (isPushLeftDown)
	{
		keepButton = BUTTON_TYPE_4;
	}
	m_KeepButton[index].buttonType = keepButton;
	m_KeepButton[index].isUse = true;
}

//=================================================================
// ������������֐�
//=================================================================
bool CCommandChart::GetSameTimePushButton(BUTTON_TYPE type)
{
	if (type != BUTTON_TYPE_5 && type != BUTTON_TYPE_6)
	{
		return false;
	}

	bool isPushLeftUp = false;
	bool isPushLeftDown = false;
	bool isPushRightUp = false;
	bool isPushRightDown = false;
	bool isPushDoubleUp = false;
	bool isPushDoubleDown = false;

	for (int i = 0; i < MAX_KEEP_COMMAND_NUM; i++)
	{
		if (!m_KeepButton[i].isUse)
		{
			break;
		}
		if (m_KeepButton[i].buttonType == BUTTON_TYPE_1)
		{
			isPushRightUp = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_2)
		{
			isPushRightDown = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_3)
		{
			isPushLeftUp = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_4)
		{
			isPushLeftDown = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_5)
		{
			isPushDoubleUp = true;
		}
		else if (m_KeepButton[i].buttonType == BUTTON_TYPE_6)
		{
			isPushDoubleDown = true;
		}
	}

	// ���肵�����̏㓯�������Ȃ�
	if (type == BUTTON_TYPE_5)
	{
		if (isPushDoubleUp || (isPushLeftUp && isPushRightUp))
		{
			return true;
		}
	}

	else if (type == BUTTON_TYPE_6)
	{
		// ���肵�����̉����������Ȃ�
		if (isPushDoubleDown || (isPushLeftDown && isPushRightDown))
		{
			return true;
		}
	}

	return false;
}

//=================================================================
// �ێ����Ă��R�}���h�j��
//=================================================================
void CCommandChart::RefleshKeepCommand()
{
	// �ێ����Ă���j��
	for (int i = 0; i < MAX_KEEP_COMMAND_NUM; i++)
	{
		m_KeepButton[i].isUse = false;
		m_KeepButton[i].buttonType = BUTTON_TYPE_NONE;
	}
}

//=================================================================
// FINISH�R�}���h�̎n���{�^���\��
//=================================================================
void CCommandChart::isAppearFinishBeginCommand(void)
{
	if (!m_pCommandManager->GetCanUseFinishSkill(m_MyID))
	{
		m_apCommandName[MAX_COMAND_NAME_NUM - 1]->SetDrawFlag(false);
		m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetDrawFlag(false);
		if (m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->GetInputFlag())
		{
			for (int i = 0; i < MAX_COMAND_NUM; i++)
			{
				m_CommandInfo.commandList.finishAttack[i].pCommandUI->SetDrawFlag(false);
				m_CommandInfo.commandList.finishAttack[i].pCommandUI->SetInputFlag(false);
			}
		}
		m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetInputFlag(false);
		return;
	}
	m_apCommandName[MAX_COMAND_NAME_NUM - 1]->SetDrawFlag(true);
	m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->SetDrawFlag(true);
}

//=================================================================
// FINISH�Z�������Ă邩
//=================================================================
bool CCommandChart::isCompleteFinishSkill(void)
{
	// FINISH�Z�̔���
	if (m_pCommandManager->GetCanUseFinishSkill(m_MyID))
	{
		for (int j = 0; j < MAX_COMAND_NUM; j++)
		{
			// �I�[��������Ă邩�𒲂ׂ�
			if (!m_CommandInfo.commandList.finishAttack[j].isEndList)
			{
				continue;
			}
			if (m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetInputFlag())
			{
				return true;
			}
			break;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// �S�ẴR�}���h�̌��̃|���S��������
//-----------------------------------------------------------------------------
void CCommandChart::AllCandidateInputBackPolygonVanish(void)
{
	for (int i = 0; i < MAX_BEGIN_COMAND_NUM; i++)
	{
		m_CommandInfo.beginCommand.firstCommand[i].pCommandUI->CandidateInputBackPolygonVanish();
	}

	// �R�}���h���X�g�̏�����
	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.smallAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
	}

	// ���Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.middleAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
	}

	// ��Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.largeAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
	}

	// FINISH�Z
	// �R�}���h�̒�������
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �\���͂��Ȃ�
		m_CommandInfo.commandList.finishAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
	}
}

//-----------------------------------------------------------------------------
// FINISH�Z�̃R�}���h�̃{�^��������Ă邩
//-----------------------------------------------------------------------------
void CCommandChart::CheckPushForFinishCommand(void)
{
	for (int j = 0; j < MAX_COMAND_NUM; j++)
	{
		// �n���{�^��������ĂȂ��Ȃ画�肵�Ȃ�
		if (!m_CommandInfo.beginCommand.firstCommand[MAX_BEGIN_COMAND_NUM - 1].pCommandUI->GetInputFlag())
		{
			break;
		}

		if (m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetInputFlag())
		{
			continue;
		}

		// �{�^����r
		if (m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetButtonType() == m_aCommandKeep
			|| GetSameTimePushButton(m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetButtonType()))
		{
			m_CommandInfo.commandList.finishAttack[j].pCommandUI->SetInputFlag(true);
			m_CommandInfo.commandList.finishAttack[j].pCommandUI->InputUIUVChange(COMMAND_BUTTON_FINISHER[j + 1], true);

			// ���X�g�̍Ō�̃R�}���h�Ŗ�����Ύ��̃R�}���h�Ƀ|���S����\������
			if (m_CommandInfo.commandList.finishAttack[j].pCommandUI->GetInputFlag() &&
				!m_CommandInfo.commandList.finishAttack[j].isEndList)
			{
				m_CommandInfo.commandList.finishAttack[j].pCommandUI->CandidateInputBackPolygonVanish();
				if (j + 1 < MAX_COMAND_NUM)
				{
					m_CommandInfo.commandList.finishAttack[j + 1].pCommandUI->CandidateInputBackPolygonDraw();
				}
			}

		}
		break;
	}
}

//-----------------------------------------------------------------------------
// ���̃{�^��������Ă邩
//-----------------------------------------------------------------------------
void CCommandChart::CheckPushCommand(void)
{
	bool isPushButton1 = false;
	bool isPushButton2 = false;
	bool isPushButton3 = false;
	bool isPushButton4 = false;

	//�L�[����
	if (m_MyID == PLAYER_1)
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
	else if (m_MyID == PLAYER_2)
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

	// ������������̂��߂ɂ����Ŋi�[
	KeepPushButton(isPushButton1, isPushButton2, isPushButton3, isPushButton4);

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
	else
	{
		m_aCommandKeep = BUTTON_TYPE_NONE;
	}
}

// EOF