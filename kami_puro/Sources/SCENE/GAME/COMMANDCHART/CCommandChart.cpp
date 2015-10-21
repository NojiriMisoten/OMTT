//=============================================================================
//
//	CCommandChart�N���X�̏���(CCommandChart.cpp)
//
//	Author : �����@�Ȉ�
//
//=============================================================================
// �C���N���[�h
#include "CCommandChart.h"
#include "../../../INPUT/CInputKeyboard.h"
#include "CCommandChartUI.h"

//-----------------------------------------------------------------------------
//	�}�N����`
//-----------------------------------------------------------------------------
static const float UI_X_POSITION = 150.0f;	// UI�̏����\�����W(x���W)
static const float UI_Y_POSITION = 300.0f;	// UI�̏����\�����W(y���W)
static const float UI_X_POS_ADD = 50.0f;	// �P�\�����UI�̍��W�̕ω��̒l
static const int COMMAND_DETH_COUNT = 60;	// �R�}���h�����܂ł̃J�E���g

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
CCommandChart::CCommandChart(LPDIRECT3DDEVICE9* device)
{
	// �f�o�C�X�̃|�C���^��ۑ�
	m_pD3DDevice = device;
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
CCommandChart::~CCommandChart()
{
}

//-----------------------------------------------------------------------------
//	����������
//-----------------------------------------------------------------------------
void CCommandChart::Init(void)
{
	// �u���͂��ꂽ�R�}���h�ێ��p�z��v�Ɓu�\��������͂��ꂽ�R�}���hUI�̕ێ��v�����
	for (int i = 0; i < MAX_COMMAND_KEEP; i++)
	{
		// ���͂��ꂽ�R�}���h�ێ��p�z��
		m_aCommandKeep[i] = BUTTON_TYPE_NONE;
		// �\��������͂��ꂽ�R�}���hUI�̕ێ�
		m_apCommandUI[i] = NULL;
	}

	// �ێ����̃R�}���h���̏�����
	m_nKeepCommandNum = 0;

	// ���͌��UI��\������x���W�̏�����
	m_fPosX = UI_X_POSITION;

	// �R�}���h�����J�E���^�[�̏�����
	m_nCommandDethCnt = 0;

	// �R�}���h���͔��f�t���O�̏�����
	m_isCommandInput = true;
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
		if (CInputKeyboard::GetKeyboardTrigger(DIK_Q))
		{
			// �L�[���͂̕ۑ�
			m_aCommandKeep[m_nKeepCommandNum] = DIK_Q;

			// UI�̍쐬
			m_apCommandUI[m_nKeepCommandNum] = CCommandChartUI::Create( m_pD3DDevice,
																		BUTTON_TYPE_1,
																		D3DXVECTOR3(m_fPosX, UI_Y_POSITION, 0.0f));

			// �R�}���h�ێ����̑���
			m_nKeepCommandNum++;

			// �`�悷��x���W�̍X�V
			m_fPosX += UI_X_POS_ADD;
		}
		// W
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_W))
		{
			// �L�[���͂̕ۑ�
			m_aCommandKeep[m_nKeepCommandNum] = DIK_W;

			// UI�̍쐬
			m_apCommandUI[m_nKeepCommandNum] = CCommandChartUI::Create(	m_pD3DDevice, 
																		BUTTON_TYPE_2, 
																		D3DXVECTOR3(m_fPosX, UI_Y_POSITION, 0.0f));

			// �R�}���h�ێ����̑���
			m_nKeepCommandNum++;

			// �`�悷��x���W�̍X�V
			m_fPosX += UI_X_POS_ADD;
		}
		// A
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_A))
		{
			// �L�[���͂̕ۑ�
			m_aCommandKeep[m_nKeepCommandNum] = DIK_A;

			// UI�̍쐬
			m_apCommandUI[m_nKeepCommandNum] = CCommandChartUI::Create(	m_pD3DDevice, 
																		BUTTON_TYPE_3, 
																		D3DXVECTOR3(m_fPosX, UI_Y_POSITION, 0.0f));

			// �R�}���h�ێ����̑���
			m_nKeepCommandNum++;

			// �`�悷��x���W�̍X�V
			m_fPosX += UI_X_POS_ADD;
		}
		// S
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_S))
		{
			// �L�[���͂̕ۑ�
			m_aCommandKeep[m_nKeepCommandNum] = DIK_S;

			// UI�̍쐬
			m_apCommandUI[m_nKeepCommandNum] = CCommandChartUI::Create(	m_pD3DDevice, 
																		BUTTON_TYPE_4, 
																		D3DXVECTOR3(m_fPosX, UI_Y_POSITION, 0.0f));

			// �R�}���h�ێ����̑���
			m_nKeepCommandNum++;

			// �`�悷��x���W�̍X�V
			m_fPosX += UI_X_POS_ADD;
		}
	}
	else
	{
		// �R�}���h�����J�E���^�[�̑���
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
		if (m_aCommandKeep[i] == DIK_Q)
		{
			CDebugProc::Print("Q");
		}
		// W
		else if (m_aCommandKeep[i] == DIK_W)
		{
			CDebugProc::Print("W");
		}
		// A
		else if (m_aCommandKeep[i] == DIK_A)
		{
			CDebugProc::Print("A");
		}
		// S
		else if (m_aCommandKeep[i] == DIK_S)
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

// EOF