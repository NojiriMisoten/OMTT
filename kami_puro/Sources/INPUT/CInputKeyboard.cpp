//=============================================================================
//
// CInputKeyboard�N���X [CInputKeyboard.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CInputKeyboard.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const BYTE KEY_CORD_INFO[KEYBOARD_CODE_MAX] =
{
	DIK_RETURN,
	DIK_M,
	DIK_RIGHT,
	DIK_LEFT,
	DIK_UP,
	DIK_DOWN,
	DIK_F3,			// �R�}���h�`���[�g�̃A�j���[�V����OPEN
	DIK_F4,			// �R�}���h�`���[�g�̃A�j���[�V����CLOSE
	DIK_LEFT,		// �J�b�g�C�� ���̃v���C��
	DIK_RIGHT,		// �J�b�g�C�� �E�̃v���C��
	DIK_F5,			// �t�F�[�h��UI
	DIK_Y,
	DIK_H,
	DIK_N,
	DIK_U,
	DIK_J,
	DIK_T,
	DIK_SEMICOLON,
	DIK_COLON,
	DIK_Q,
	DIK_A,
	DIK_W,
	DIK_S,
	DIK_I,
	DIK_K,
	DIK_O,
	DIK_L,
	DIK_1,
	DIK_2,
	DIK_3,
	DIK_4,
	DIK_5,
	DIK_SPACE,
	DIK_F1,
	DIK_F2,
	DIK_Z,				// �v���C���[1����p�i�f�o�b�O�j
	DIK_X,				// �v���C���[1����p�i�f�o�b�O�j
	DIK_E,				// �v���C���[1����p�i�f�o�b�O�j
	DIK_D,				// �v���C���[1����p�i�f�o�b�O�j
	DIK_C,				// �v���C���[1����p�i�f�o�b�O�j
	DIK_COMMA,			// �v���C���[2����p�i�f�o�b�O�j
	DIK_PERIOD,			// �v���C���[2����p�i�f�o�b�O�j
	DIK_P,				// �v���C���[2����p�i�f�o�b�O�j
	DIK_SEMICOLON,		// �v���C���[2����p�i�f�o�b�O�j
	DIK_SLASH,			// �v���C���[2����p�i�f�o�b�O�j
	DIK_F6,				// �I�[�o�[���C(Ready�Ƃ��̕\��)
	DIK_F,
	DIK_F7,				// ���[�v�^�C�}�[
	DIK_F8,				// ���v���~�߂�

};

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
BYTE	CInputKeyboard :: m_aKeyState[KEY_NUM];
BYTE	CInputKeyboard :: m_TrigKeyState[KEY_NUM];
BYTE	CInputKeyboard :: m_ReleaseKeyState[KEY_NUM];
BYTE	CInputKeyboard :: m_RepeatKeyState[KEY_NUM];
int		CInputKeyboard :: m_RepeatKeyCount[KEY_NUM];
bool	CInputKeyboard :: m_Repeatflag[KEY_NUM];
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CInputKeyboard::CInputKeyboard(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CInputKeyboard::~CInputKeyboard(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �e�N���X������
	CInput ::Init(hInstance, hWnd);

	HRESULT hr;
	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard
								, &m_pDIDevice
								, NULL);
	if(FAILED(hr))
	{
		MessageBox(NULL,"�f�o�C�X�I�u�W�F�N�g�̍쐬���s","�f�o�C�X�I�u�W�F�N�g�̍쐬���s",MB_OK);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(NULL,"�f�[�^�t�H�[�}�b�g�̐ݒ莸�s","�f�[�^�t�H�[�}�b�g�̐ݒ莸�s",MB_OK);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(NULL,"�������[�h�̐ݒ莸�s","�������[�h�̐ݒ莸�s",MB_OK);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevice->Acquire();

	// �L�[�̏�����
	for(int i = 0; i < KEY_NUM; i++)
	{
		m_RepeatKeyCount[i] = 0;
		m_Repeatflag[i] = false;
		m_aKeyState[i] = 0;
		m_ReleaseKeyState[i] = 0;
		m_TrigKeyState[i] = 0;
		m_RepeatKeyState[i] = 0;
	}

	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CInputKeyboard::Uninit(void)
{
	if(m_pDIDevice != NULL)
	{
		m_pDIDevice->Unacquire();
		m_pDIDevice->Release();
		m_pDIDevice = NULL;
	}
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[KEY_NUM];			// �ŐV�̃L�[�����󂯎��
	
	// �f�[�^���擾
	if(SUCCEEDED(m_pDIDevice->GetDeviceState(sizeof(aKeyState),&aKeyState[0])))
	{
		for(int nKey = 0; nKey < KEY_NUM; nKey++)
		{
			// �g���K�[�L�[�擾
			m_TrigKeyState[nKey] = ((m_aKeyState[nKey] ^ aKeyState[nKey]) & aKeyState[nKey]);

			// �����[�X�L�[�擾
			m_ReleaseKeyState[nKey] = ((m_aKeyState[nKey] ^ aKeyState[nKey]) & m_aKeyState[nKey]);

			// �L�[���X�V
			m_aKeyState[nKey] = aKeyState[nKey];
			
			m_RepeatKeyState[nKey] = (BYTE)false;
			// ���s�[�g�L�[�t���O�n�m�Ȃ�
			if(m_Repeatflag[(int)nKey] == true)
			{
				// �J�E���g�A�b�v
				m_RepeatKeyCount[(int)nKey]++;
			}
			// �t���OOFF�Ȃ�
			else if(m_Repeatflag[(int)nKey] == false)
			{
				// �J�E���g������
				m_RepeatKeyCount[(int)nKey] = 0;
				if(GetKeyboardTrigger(nKey))
				{
					// �t���O�n�m
					m_Repeatflag[(int)nKey] = true;
					m_RepeatKeyState[nKey] = (BYTE)true;
				}
			}

			// ������ĂȂ�������
			if(!m_aKeyState[(int)nKey])
			{
				// �t���OOFF
				m_Repeatflag[(int)nKey] = false;
			}
	
			// ��莞�ԉ����Ă���
			if(m_RepeatKeyCount[(int)nKey] >= REPEAT_TIME)
			{
				// �t���OON
				m_RepeatKeyState[nKey] = (BYTE)true;
			}
		}
	}
	// ���s������
	else
	{
		// �����擾
		m_pDIDevice->Acquire();
	}
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetKeyboardPress(BYTE key)
{
	for(int nKey = 0; nKey < KEY_NUM; nKey++)
	{
		if(m_aKeyState[(int)key])
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetKeyboardTrigger(BYTE key)
{
	int nKey;
	for(nKey = 0; nKey < KEY_NUM; nKey++)
	{
		if(m_TrigKeyState[(int)key])
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetKeyboardRepeat(BYTE key)
{
	if(m_RepeatKeyState[(int)key])
	{
		return true;
	}
	return false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetKeyboardRelease(BYTE key)
{
	for(int nKey = 0; nKey < KEY_NUM; nKey++)
	{
		if(m_ReleaseKeyState[(int)key])
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetKeyboardPress(KEYBOARD_CODE key)
{
#ifdef _DEBUG
	assert(key >= 0 && key < KEYBOARD_CODE_MAX && "�L�[�R�[�h���s��");
#endif
	if (m_aKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}

	return false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetKeyboardTrigger(KEYBOARD_CODE key)
{
#ifdef _DEBUG
	assert(key >= 0 && key < KEYBOARD_CODE_MAX && "�L�[�R�[�h���s��");
#endif
	if (m_TrigKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}
	
	return false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetKeyboardRepeat(KEYBOARD_CODE key)
{
#ifdef _DEBUG
	assert(key >= 0 && key < KEYBOARD_CODE_MAX && "�L�[�R�[�h���s��");
#endif
	if (m_RepeatKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}
	return false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool CInputKeyboard::GetKeyboardRelease(KEYBOARD_CODE key)
{
#ifdef _DEBUG
	assert(key >= 0 && key < KEYBOARD_CODE_MAX && "�L�[�R�[�h���s��");
#endif
	if (m_ReleaseKeyState[(int)KEY_CORD_INFO[key]])
	{
		return true;
	}
	
	return false;
}
//----EOF----