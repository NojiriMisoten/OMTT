//=============================================================================
//
// CPause�N���X [CPause.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CPause.h"
#include "../../INPUT/CInputKeyboard.h"
#include "../../FADE/CFade.h"
#include "../../MANAGER/CManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const D3DXVECTOR3	RETURN_CURSOL_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.56f, 500.f, 0);// �u�߂�v�I�𒆂̃J�[�\���̈ʒu
static const D3DXVECTOR3	TITLE_CURSOL_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.56f, 380.f, 0);	// �u�^�C�g���w�v�I�𒆂̃J�[�\���̈ʒu
static const D3DXVECTOR3	RETRY_CURSOL_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.56f, 260.f, 0);	// �u���g���C�v�I�𒆂̃J�[�\���̈ʒu
static const float			CURSOL_WIDTH = 60.f;											// �J�[�\���̕�
static const float			CURSOL_HEIGHT = 60.f;											// �J�[�\���̍���
static const float			DEFAULT_CURSOL_LENGTH_OF_LEFT_TO_RIGHT = 550.f;					// �J�[�\���̍��E�̋���

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
bool CPause::m_bPause = false;
bool CPause::m_bRetry = false;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPause ::CPause(LPDIRECT3DDEVICE9 *pDevice, int nPriority):CScene2D(pDevice, nPriority)
{
	m_bPause = false;
	m_pCursol = NULL;
	m_bReturnTititle = false;
	m_bRetry = false;
	m_nChoiceMenu = PAUSE_RETURN;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPause ::~CPause(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CPause :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock )
{
	// �|�[�Y�̏�����
	CScene2D::Init(pos, width, height, texType);
	
	// �J�[�\���̏�����
	//m_pCursol = m_pCursol->Create(m_pD3DDevice, RETURN_CURSOL_POS, CURSOL_WIDTH, CURSOL_HEIGHT, TEXTURE_NULL, DEFAULT_CURSOL_LENGTH_OF_LEFT_TO_RIGHT);

	m_bPause = false;
	m_bRetry = false;

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CPause :: Uninit(void)
{
	CScene2D::Uninit();
	m_bPause = false;
	m_bRetry = false;
	m_nChoiceMenu = PAUSE_RETURN;
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CPause :: Update(void)
{
	if(m_bPause && !m_bRetry && !m_bReturnTititle)
	{
		// �J�[�\���X�V
		m_pCursol->SetDrawFlag(true);
		m_pCursol->Update();

		// �J�[�\����Ɉړ�
		if(CInputKeyboard::GetKeyboardTrigger(DIK_W)
			|| CInputKeyboard::GetKeyboardTrigger(DIK_UP))
		{
			m_nChoiceMenu--;

			// ��ԏゾ�������ԉ��Ɉړ�
			if(m_nChoiceMenu < PAUSE_RETRY)
			{
				m_nChoiceMenu = PAUSE_RETURN;
			}
		}

		// �J�[�\�����Ɉړ�
		if(CInputKeyboard::GetKeyboardTrigger(DIK_S)
			|| CInputKeyboard::GetKeyboardTrigger(DIK_DOWN))
		{
			m_nChoiceMenu++;

			// ��ԉ����������ԏ�Ɉړ�
			if(m_nChoiceMenu >= PAUSE_MAX)
			{
				m_nChoiceMenu = PAUSE_RETRY;
			}
		}

		switch (m_nChoiceMenu)
		{
			// ���g���C�I��
			case PAUSE_RETRY:

				// ���W�w��
				m_pCursol->SetVertexPolygon(RETRY_CURSOL_POS, CURSOL_WIDTH, CURSOL_HEIGHT);
			
				// ����L�[�����ꂽ��
				if(CInputKeyboard::GetKeyboardTrigger(DIK_Z) 
					|| CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
				{
					// ���g���C
					m_bRetry = true;
					m_pCursol->SetDrawFlag(false);
				}

			break;

			// �^�C�g���I��
			case PAUSE_TITLE:

				// ���W�w��
				m_pCursol->SetVertexPolygon(TITLE_CURSOL_POS, CURSOL_WIDTH, CURSOL_HEIGHT);

				// ����L�[�����ꂽ��
				if(CInputKeyboard::GetKeyboardTrigger(DIK_Z)
					|| CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
				{
					// �^�C�g����
					m_bReturnTititle = true;
					m_pCursol->SetDrawFlag(false);
				}

			break;

			// �߂�I��
			case PAUSE_RETURN:

				// ���W�w��
				m_pCursol->SetVertexPolygon(RETURN_CURSOL_POS, CURSOL_WIDTH, CURSOL_HEIGHT);

				// ����L�[�����ꂽ��
				if(CInputKeyboard::GetKeyboardTrigger(DIK_Z)
					|| CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
				{
					// �|�[�Y�I��
					m_bPause = false;
					m_pCursol->SetDrawFlag(false);
				}
			break;
		}
	}
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CPause ::Draw(void)
{
	if(m_bPause)
	{
		CScene2D::Draw();
		m_pCursol->Draw();
	}
}
//----EOF----