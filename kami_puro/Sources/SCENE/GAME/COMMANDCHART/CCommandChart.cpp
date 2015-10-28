//=============================================================================
//
//	CCommandChart�N���X�̏���(CCommandChart.cpp)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
// �C���N���[�h
#include "CCommandChart.h"
#include "CCommandChartUI.h"

//-----------------------------------------------------------------------------
//	�}�N����`
//-----------------------------------------------------------------------------
static const float UI_X_POSITION = 100.0f;		// UI�̏����\�����W(x���W)
static const float UI_Y_POSITION = 300.0f;		// UI�̏����\�����W(y���W)
static const float UI_X_POS_ADD = 50.0f;		// �P�\�����UI�̍��W�̕ω��̒l
static const float NEXT_UI_X_POS = 100.0f;		// ���ɓ��͂���R�}���h�̈�ԏ��UI�̍��W
static const float NEXT_UI_Y_POS = 300.0f;		// ���ɓ��͂���R�}���h�̈�ԏ��UI�̍��W
static const float NEXT_UI_X_POS_ADD = 100.0f;	// ���ɓ��͂���R�}���h��UI�̍��W�̕ω��̒l
static const float NEXT_UI_Y_POS_ADD = 100.0f;	// ���ɓ��͂���R�}���h��UI�̍��W�̕ω��̒l
static const int COMMAND_DETH_COUNT = 60;		// �R�}���h�����܂ł̃J�E���g

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* pDevice, PLAYER_NUMBER PlayerNumber)
{
	// �f�o�C�X�̃|�C���^��ۑ�
	m_pD3DDevice = pDevice;

	// �u���͂��ꂽ�R�}���h�ێ��p�z��v�Ɓu�\��������͂��ꂽ�R�}���hUI�̕ێ��v����������
	for (int i = 0; i < MAX_COMMAND_KEEP; i++)
	{
		// ���͂��ꂽ�R�}���h�ێ��p�z��
		m_aCommandKeep[i] = KEYBOARD_CORD_DECIDE;
		// �\��������͂��ꂽ�R�}���hUI�̕ێ�
		m_apCommandUI[i] = NULL;
	}

	// �u���ɓ��͂�����̃R�}���hUI�ێ��z��v��������
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		m_apNextCommandUI[i] = NULL;
	}

	// ���g�̃v���C���[�ԍ���ݒ�
	m_MyNumber = PlayerNumber;
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
CCommandChart* CCommandChart::Create(LPDIRECT3DDEVICE9* pDevice, PLAYER_NUMBER PlayerNumber)
{
	// �R�}���h�`���[�g�̐���
	CCommandChart* temp = new CCommandChart(pDevice, PlayerNumber);
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
	m_nCommandDethCnt = 0;

	// �R�}���h���͔��f�t���O�̏�����
	m_isCommandInput = true;

	// �ŏ��ɓ��͂��ׂ��R�}���h�̍쐬
	CreateNextCommand(m_nKeepCommandNum);
}

//-----------------------------------------------------------------------------
//	�X�V����
//-----------------------------------------------------------------------------
void CCommandChart::Update(void)
{
	// �R�}���h���͉\
	if (m_isCommandInput)
	{
		// �L�[���͂��ꂽ�炻���ێ�
		// Q
		if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CORD_COMMAND_DEBUG_Q))
		{
			// �L�[���͂̕ۑ�
			m_aCommandKeep[m_nKeepCommandNum] = KEYBOARD_CORD_COMMAND_DEBUG_Q;

			// �L�[���͏��̍X�V
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_1)
				{
					m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
				}
			}

			// ���͂���Ă���R�}���h�������Ȃ��悤�Ƀt���O��ύX
			m_apCommandUI[m_nKeepCommandNum]->SetInputFlag(true);

			// �ڕW���W�̐ݒ�
			if (m_MyNumber == PLAYER_NUMBER_1)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}
			else if (m_MyNumber == PLAYER_NUMBER_2)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}

			// �R�}���h�ێ����̑���
			m_nKeepCommandNum++;

			// �`�悷��x���W�̍X�V
			m_fPosX += UI_X_POS_ADD;

			// ���͌��̃R�}���h��������
			DethNextCommand();

			// ���ɓ��͂��ׂ��R�}���h�̍쐬
			CreateNextCommand(m_nKeepCommandNum);
		}
		// W
		else if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CORD_COMMAND_DEBUG_W))
		{
			// �L�[���͂̕ۑ�
			m_aCommandKeep[m_nKeepCommandNum] = KEYBOARD_CORD_COMMAND_DEBUG_W;

			// �L�[���͏��̍X�V
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_2)
				{
					m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
				}
			}

			// ���͂���Ă���R�}���h�������Ȃ��悤�Ƀt���O��ύX
			m_apCommandUI[m_nKeepCommandNum]->SetInputFlag(true);

			// �ڕW���W�̐ݒ�
			if (m_MyNumber == PLAYER_NUMBER_1)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}
			else if (m_MyNumber == PLAYER_NUMBER_2)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}

			// �R�}���h�ێ����̑���
			m_nKeepCommandNum++;

			// �`�悷��x���W�̍X�V
			m_fPosX += UI_X_POS_ADD;

			// ���͌��̃R�}���h��������
			DethNextCommand();

			// ���ɓ��͂��ׂ��R�}���h�̍쐬
			CreateNextCommand(m_nKeepCommandNum);
		}
		// A
		else if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CORD_COMMAND_DEBUG_A))
		{
			// �L�[���͂̕ۑ�
			m_aCommandKeep[m_nKeepCommandNum] = KEYBOARD_CORD_COMMAND_DEBUG_A;

			// �L�[���͏��̍X�V
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_3)
				{
					m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
				}
			}

			// ���͂���Ă���R�}���h�������Ȃ��悤�Ƀt���O��ύX
			m_apCommandUI[m_nKeepCommandNum]->SetInputFlag(true);

			// �ڕW���W�̐ݒ�
			if (m_MyNumber == PLAYER_NUMBER_1)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}
			else if (m_MyNumber == PLAYER_NUMBER_2)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}

			// �R�}���h�ێ����̑���
			m_nKeepCommandNum++;

			// �`�悷��x���W�̍X�V
			m_fPosX += UI_X_POS_ADD;

			// ���͌��̃R�}���h��������
			DethNextCommand();

			// ���ɓ��͂��ׂ��R�}���h�̍쐬
			CreateNextCommand(m_nKeepCommandNum);
		}
		// S
		else if (CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CORD_COMMAND_DEBUG_S))
		{
			// �L�[���͂̕ۑ�
			m_aCommandKeep[m_nKeepCommandNum] = KEYBOARD_CORD_COMMAND_DEBUG_S;

			// �L�[���͏��̍X�V
			for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
			{
				if (m_apNextCommandUI[i]->GetButtonType() == BUTTON_TYPE_4)
				{
					m_apCommandUI[m_nKeepCommandNum] = m_apNextCommandUI[i];
				}
			}

			// ���͂���Ă���R�}���h�������Ȃ��悤�Ƀt���O��ύX
			m_apCommandUI[m_nKeepCommandNum]->SetInputFlag(true);

			// �ڕW���W�̐ݒ�
			if (m_MyNumber == PLAYER_NUMBER_1)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(UI_X_POSITION + (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}
			else if (m_MyNumber == PLAYER_NUMBER_2)
			{
				m_apCommandUI[m_nKeepCommandNum]->SetDestPos(D3DXVECTOR3(SCREEN_WIDTH - UI_X_POSITION - (UI_X_POS_ADD*m_nKeepCommandNum), UI_Y_POSITION, 0.0f));
			}

			// �R�}���h�ێ����̑���
			m_nKeepCommandNum++;

			// �`�悷��x���W�̍X�V
			m_fPosX += UI_X_POS_ADD;

			// ���͌��̃R�}���h��������
			DethNextCommand();

			// ���ɓ��͂��ׂ��R�}���h�̍쐬
			CreateNextCommand(m_nKeepCommandNum);
		}
	}
	else
	{
		// �R�}���h�����܂ł̃J�E���^�[�̑���
		m_nCommandDethCnt++;

		// COMMAND_DETH_COUNT���傫���l�ɂȂ�����s��
		if (m_nCommandDethCnt > COMMAND_DETH_COUNT)
		{
			// ���ݕ\�����Ă���UI�����X�g����폜���ďI���������s���Ă���f���[�g�����
			for (int i = 0; i < MAX_COMMAND_KEEP; i++)
			{
				m_apCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_UI);
				m_apCommandUI[i]->Uninit();
				SAFE_DELETE(m_apCommandUI[i]);
			}
			// �R�}���h�ێ����ƕێ��R�}���h�̃��Z�b�g
			Init();
		}
	}

	// �ێ����̃R�}���h�����ő�ɂȂ����珉����
	if (m_nKeepCommandNum == MAX_COMMAND_KEEP)
	{
		// �R�}���h���͔���t���O��s��
		m_isCommandInput = false;
	}
}

//-----------------------------------------------------------------------------
//	�`�揈��
//-----------------------------------------------------------------------------
void CCommandChart::Draw(void)
{
#ifdef _DEBUG
	// ���݂̓��͏󋵂��f�o�b�N�v���V�[�W���ŕ\��
	CDebugProc::Print("���݂̓��͏�");
	// ���͂���Ă���񐔉񂷂�
	for (int i = 0; i < m_nKeepCommandNum; i++)
	{
		// D
		if (m_aCommandKeep[i] == KEYBOARD_CORD_COMMAND_DEBUG_Q)
		{
			CDebugProc::Print("Q");
		}
		// W
		else if (m_aCommandKeep[i] == KEYBOARD_CORD_COMMAND_DEBUG_W)
		{
			CDebugProc::Print("W");
		}
		// A
		else if (m_aCommandKeep[i] == KEYBOARD_CORD_COMMAND_DEBUG_A)
		{
			CDebugProc::Print("A");
		}
		// S
		else if (m_aCommandKeep[i] == KEYBOARD_CORD_COMMAND_DEBUG_S)
		{
			CDebugProc::Print("S");
		}
	}
	CDebugProc::Print("\n");
#endif
}

//-----------------------------------------------------------------------------
//	�I������
//-----------------------------------------------------------------------------
void CCommandChart::Uninit(void)
{
}

//-----------------------------------------------------------------------------
//	���ɓ��͂�����̃R�}���h�̍쐬
//-----------------------------------------------------------------------------
void CCommandChart::CreateNextCommand(int nNumCommand)
{
	// ���ɕ\������R�}���h�̐������W
	float fPosX = 0.0f;
	float fPosY = 0.0f;
	// ���ɕ\������R�}���h�̖ڕWX���W
	float fPosDestX = 0.0f;

	// �ڕW�̍��W
	// �v���C���[�P�̎��̕\��X���W
	if (m_MyNumber == PLAYER_NUMBER_1)
	{
		fPosDestX = NEXT_UI_X_POS + (NEXT_UI_X_POS_ADD * nNumCommand);
		fPosX = fPosDestX - NEXT_UI_X_POS_ADD;
	}
	// �v���C���[�Q�̎��̕\��X���W
	else if (m_MyNumber == PLAYER_NUMBER_2)
	{
		fPosDestX = (SCREEN_WIDTH - NEXT_UI_X_POS) - (NEXT_UI_X_POS_ADD * nNumCommand);
		fPosX = fPosDestX + NEXT_UI_X_POS_ADD;
	}

	// �ێ�����MAX�Ŗ�����Ύ��̃R�}���h��\��
	if (m_nKeepCommandNum < MAX_COMMAND_KEEP)
	{
		// ���̃R�}���h��S�쐬
		for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
		{
			fPosY = NEXT_UI_Y_POS + (NEXT_UI_Y_POS_ADD * i);
			switch (i + 1)
			{
			case BUTTON_TYPE_1:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_1,
					D3DXVECTOR3(fPosX, fPosY, 0.0f));
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY,0.0f));
				break;
			case BUTTON_TYPE_2:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_2,
					D3DXVECTOR3(fPosX, fPosY, 0.0f));
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			case BUTTON_TYPE_3:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_3,
					D3DXVECTOR3(fPosX, fPosY, 0.0f));
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			case BUTTON_TYPE_4:
				m_apNextCommandUI[i] = CCommandChartUI::Create(m_pD3DDevice,
					BUTTON_TYPE_4,
					D3DXVECTOR3(fPosX, fPosY, 0.0f));
				m_apNextCommandUI[i]->SetDestPos(D3DXVECTOR3(fPosDestX, fPosY, 0.0f));
				break;
			default:
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	���ɓ��͂�����̃R�}���h�̍폜
//-----------------------------------------------------------------------------
void CCommandChart::DethNextCommand(void)
{
	// ���ݕ\�����Ă���UI�����X�g����폜���ďI���������s���Ă���f���[�g�����
	for (int i = 0; i < MAX_NEXT_COMMAND_VIEW; i++)
	{
		if (!m_apNextCommandUI[i]->GetInputFlag())
		{
			m_apNextCommandUI[i]->UnLinkList(CRenderer::TYPE_RENDER_UI);
			m_apNextCommandUI[i]->Uninit();
			SAFE_DELETE(m_apNextCommandUI[i]);
		}
	}
}

// EOF