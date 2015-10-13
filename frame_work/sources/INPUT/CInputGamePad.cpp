//=============================================================================
//
// �R���g���[���[���͏��� [CInputGamePad.cpp]
// Author : ��K�@����
//
//=============================================================================
//**********************************************
// �C���N���[�h
//**********************************************
#include "CInputGamePad.h"
#include "../DEBUG_PROC/CDebugProc.h"
#include <stdio.h>

//**********************************************
// �}�N��
//**********************************************
static const char*	DEMO_FILE_PATH = "data/DEMO_KEY/demo_key.txt";	// �f���p�̃L�[�̕ۑ��p�X
static const int	JUDGE_TIME_REPEAT_KEY = 20;						// ���s�[�g�L�[�̔��莞��
static const int	JUDGE_UP_MIN_STICK = 0 * 100;					// �㔻��̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_UP_MAX_STICK = 45 * 100;					// �㔻��̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_RIGHT_MIN_STICK = 45 * 100;				// �E����̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_RIGHT_MAX_STICK = 135 * 100;				// �E����̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_DOWN_MIN_STICK = 135 * 100;				// ������̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_DOWN_MAX_STICK = 225 * 100;				// ������̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_LEFT_MIN_STICK = 225 * 100;				// ������̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_LEFT_MAX_STICK = 315 * 100;				// ������̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_RIGHT_UP_MIN_STICK = 5 * 100;				// �E�㔻��̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_RIGHT_UP_MAX_STICK = 85 * 100;			// �E�㔻��̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_RIGHT_DOWN_MIN_STICK = 95 * 100;			// �E������̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_RIGHT_DOWN_MAX_STICK = 175 * 100;			// �E������̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_LEFT_DOWN_MIN_STICK = 185 * 100;			// ��������̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_LEFT_DOWN_MAX_STICK = 265 * 100;			// ��������̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_LEFT_UP_MIN_STICK = 275 * 100;			// ���㔻��̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_LEFT_UP_MAX_STICK = 355 * 100;			// ���㔻��̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_MAX_STICK = 360 * 100;					// �X�e�B�b�N�̍ő�X��

//**********************************************
// �X�^�e�B�b�N�����o�ϐ�
//**********************************************
GUID	CInputGamePad::m_pad_discrimination[MAX_PAD_NUM];
GAMEPAD	CInputGamePad::m_GamePad[MAX_PAD_NUM];
bool	CInputGamePad::m_aKeyStateGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateTriggerGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateReleaseGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateRepeatGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
int		CInputGamePad::m_aKeyStateRepeatCntGamePad[MAX_PAD_NUM][KEY_MAX_NUM];
unsigned short	CInputGamePad::m_nKeepDemoKey[MAX_KEEP_KEY_FOR_DEMO];
int		CInputGamePad::m_nKeyFrameForDemo;
int		CInputGamePad::m_aKeyConfig[MAX_PAD_NUM][PAD_KEY_MAX];
//==============================================
// �R���X�g���N�^
//==============================================
CInputGamePad::CInputGamePad(void)
{
	m_nKeyFrameForDemo = 0;

	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		for (int cnt = 0; cnt < KEY_MAX_NUM; ++cnt)
		{
			m_aKeyStateGamePad[idx][cnt] = false;
			m_aKeyStateTriggerGamePad[idx][cnt] = false;
			m_aKeyStateReleaseGamePad[idx][cnt] = false;
			m_aKeyStateRepeatGamePad[idx][cnt] = false;
		}
		for (int i = 0; i < PAD_KEY_MAX; ++i)
		{
			m_aKeyConfig[idx][i] = 0;
		}
	}
}

//==============================================
// �f�X�g���N�^
//==============================================
CInputGamePad::~CInputGamePad(void)
{
}

//==============================================
// ������
//==============================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance,HWND hWnd)
{

	// �C���^�[�t�F�C�X�̎擾
	HRESULT hr;  
	hr = DirectInput8Create(hInstance				// �\�t�g�̃C���X�^���X�n���h��
							, DIRECTINPUT_VERSION	// DirectInput�̃o�[�W����
							, IID_IDirectInput8		// �擾����C���^�[�t�F�C�X�̃^�C�v
							, (LPVOID*)&m_pDInput	// �C���^�[�t�F�C�X�̊i�[��
							, NULL);				// COM�W���̐���I�u�W�F�N�g�i�g��Ȃ��̂�NULL�j
	if(FAILED(hr))
	{
		MessageBox(NULL,"DirectInput�I�u�W�F�N�g�̍쐬���s","DirectInput�I�u�W�F�N�g�̍쐬���s",MB_OK);
		return hr;
	}

	// �W���C�p�b�h�̍쐬
	// �R�[���o�b�N�֐��ɓ]���������f�[�^���i�[
	STR_ENUMDATA ed;
	ed.pInput = m_pDInput;

	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		ed.ppPadDevice = &m_GamePad[idx].pPadDevice;
		ed.padID = idx;

		hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL	// �Q�[���R���g���[���[���Ώ�
			, EnumGamePad			// �񋓂���֐�
			, &ed					// �񋓊֐��ɓn�������f�[�^�͂����ɓ����
			, DIEDFL_ATTACHEDONLY);	// �C���X�g�[���E�ڑ��ς݂̃f�o�C�X�̂ݎ擾

		// �Q�[���p�b�h�̓��͏���DIJOYSTATE2�Ɋi�[�����̂�
		// �f�[�^�t�H�[�}�b�g�ɂ�c_dfDIJoystick2���w��
		if (m_GamePad[idx].pPadDevice != NULL)
		{
			// �A�i���O�L�[�̃f�[�^��ݒ�
			// �Q�Ԗڂ̈�����pPadDevice���w�肵�Ă����ƁA
			// EnumObject�֐���pvRef�ɁA����pPadDevice���i�[�����悤�ɂȂ邽�߁A
			// pPadDevice���X�^�e�B�b�N��O���[�o���ϐ��ɂ��Ȃ��čςށB
			m_GamePad[idx].pPadDevice->EnumObjects(EnumObject, m_GamePad[idx].pPadDevice, DIDFT_AXIS);

			m_GamePad[idx].pPadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			hr = m_GamePad[idx].pPadDevice->SetDataFormat(&c_dfDIJoystick2);

			// ���s������J��
			if (FAILED(hr))
			{
				if (m_GamePad[idx].pPadDevice != NULL)
				{
					m_GamePad[idx].pPadDevice->Release();
					m_GamePad[idx].pPadDevice = NULL;
				}
			}

			hr = m_GamePad[idx].pPadDevice->SetDataFormat(&c_dfDIJoystick2);

			// ���s������J��
			if (FAILED(hr))
			{
				if (m_GamePad[idx].pPadDevice != NULL)
				{
					m_GamePad[idx].pPadDevice->Release();
					m_GamePad[idx].pPadDevice = NULL;
				}
			}

			hr = m_GamePad[idx].pPadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

			// ���s������J��
			if (FAILED(hr))
			{
				MessageBox(NULL, "�������[�h�̐ݒ莸�s", "�������[�h�̐ݒ莸�s", MB_OK);
				return hr;
			}

			// �R���g���[���[�ւ̃A�N�Z�X�����l��(���͐���J�n)
			m_GamePad[idx].pPadDevice->Acquire();
		}
		m_GamePad[idx].DirectryData = m_GamePad[idx].OldDirectryData = PP_NONE;
	}

	// �f���p�z��̏�����
	for(int idx = 0; idx < MAX_KEEP_KEY_FOR_DEMO; idx++)
	{
		m_nKeepDemoKey[idx] = 0x0000;
	}
	return S_OK;
}

//==============================================
// �I��
//==============================================
void CInputGamePad::Uninit(void)
{
	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		if (m_GamePad[idx].pPadDevice)
		{
			m_GamePad[idx].pPadDevice->Unacquire();
			m_GamePad[idx].pPadDevice->Release();
			m_GamePad[idx].pPadDevice = NULL;
		}
	}
}

//==============================================
// �X�V
//==============================================
void CInputGamePad::Update(void)
{
	HRESULT hr;
	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		bool aKeyStateOld[KEY_MAX];

		if (!m_GamePad[idx].pPadDevice)
		{
			continue;
		}

		// �O��̃f�[�^��ۑ�
		for (int pos = 0; pos < 127; pos++)
		{
			m_GamePad[idx].OldPadData.rgbButtons[pos] = m_GamePad[idx].PadData.rgbButtons[pos];
		}

		for (int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
		{
			aKeyStateOld[nCntKey] = m_aKeyStateGamePad[idx][nCntKey];
		}

		// �f�o�C�X����f�[�^���擾
		hr = m_GamePad[idx].pPadDevice->GetDeviceState(sizeof(m_GamePad[idx].PadData), &m_GamePad[idx].PadData);
		if (SUCCEEDED(hr))
		{
			// �L�[���ݒ�
			SetKeyStateGamePad();
			for (int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
			{
				// �g���K�[�L�[�X�V
				m_aKeyStateTriggerGamePad[idx][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateGamePad[idx][nCntKey]) & m_aKeyStateGamePad[idx][nCntKey];

				// �����[�X�L�[�X�V
				m_aKeyStateReleaseGamePad[idx][nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateGamePad[idx][nCntKey]) & !m_aKeyStateGamePad[idx][nCntKey];

				// ���s�[�g�L�[�X�V
				m_aKeyStateRepeatGamePad[idx][nCntKey] = m_aKeyStateTriggerGamePad[idx][nCntKey];
				if (m_aKeyStateGamePad[idx][nCntKey])
				{
					m_aKeyStateRepeatCntGamePad[idx][nCntKey]++;
					if (m_aKeyStateRepeatCntGamePad[idx][nCntKey] >= JUDGE_TIME_REPEAT_KEY)
					{
						m_aKeyStateRepeatGamePad[idx][nCntKey] = m_aKeyStateGamePad[idx][nCntKey];
					}
				}
				else
				{
					m_aKeyStateRepeatCntGamePad[idx][nCntKey] = 0;
					m_aKeyStateRepeatGamePad[idx][nCntKey] = 0;
				}
			}
		}
		else
		{
			// �����l��
			m_GamePad[idx].pPadDevice->Acquire();
		}
	#ifdef _DEBUG
		for (int pos = 0; pos < 127; pos++)
		{
			if (m_GamePad[idx].PadData.rgbButtons[pos] != 0)
			{
				CDebugProc::Print("�v���C���[ID�F%d\n", idx);
				CDebugProc::Print("�{�^���R�[�h�F%d\n", pos);
			}
		}
	#endif
	}
}

//==============================================
// �R�[���o�b�N�֐�
//==============================================
BOOL CALLBACK CInputGamePad::EnumGamePad(const DIDEVICEINSTANCE* pInstance, LPVOID pContext)
{
	STR_ENUMDATA *ed = (STR_ENUMDATA*)pContext;
	DIDEVCAPS diDevCaps;							// �W���C�p�b�h�̔\�͏��

	for (int idx = 0; idx < ed->padID; ++idx)
	{
		// ���Ɏ擾�ς݂̃f�o�C�X�������ꍇ�͂��������s����
		if (m_pad_discrimination[idx] == pInstance->guidInstance)
		{
			return DIENUM_CONTINUE;
		}
	}

	// �f�o�C�X�̎��ʎq��ۑ�
	m_pad_discrimination[ed->padID] = pInstance->guidInstance;
	

	HRESULT hr;
	
	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾
	hr = ed->pInput->CreateDevice(pInstance->guidInstance, ed->ppPadDevice, NULL);
	if(FAILED(hr))
	{
		return DIENUM_CONTINUE;  // �f�o�C�X���쐬�ł��Ȃ��̂ŗ񋓂𑱂���
	}
	
	// �W���C�p�b�h�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	
	// ��]����f�o�C�X���쐬�ł����̂ŗ񋓂��I������
	return DIENUM_STOP;
}

BOOL CALLBACK CInputGamePad::EnumObject(LPCDIDEVICEOBJECTINSTANCE pInstance, LPVOID pvRef)
{
	DIPROPRANGE range;
	range.diph.dwSize = sizeof(DIPROPRANGE);
	range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	range.diph.dwObj = pInstance->dwType;			// �{�^����X�e�B�b�N�̃f�[�^
	range.diph.dwHow = DIPH_BYID;					// �^�C�v�Ŏ擾���邱�Ƃ��w��
	range.lMin = -10;								// �l�̍ŏ��l
	range.lMax = +10;								// �l�̍ő�l

	LPDIRECTINPUTDEVICE8 pInputDev = (LPDIRECTINPUTDEVICE8)pvRef;
	pInputDev->SetProperty(DIPROP_RANGE, &range.diph);

	return DIENUM_CONTINUE;
}

//=============================================================================
// �W���C�p�b�h�̃L�[���ݒ�
//=============================================================================
void CInputGamePad::SetKeyStateGamePad(void)
{
	for (int idx = 0; idx < MAX_PAD_NUM; ++idx)
	{
		if (!m_GamePad[idx].pPadDevice)
		{
			continue;
		}

		// �Q�[���p�b�h�{�^�����X�e�B�b�N[���L�[ & �X�e�B�b�N��]
		if (m_GamePad[idx].PadData.lX <= -m_kInputDeep
			|| (m_GamePad[idx].PadData.rgdwPOV[0] >= JUDGE_LEFT_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[0] <= JUDGE_LEFT_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_LEFT] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_LEFT] = false;
		}
		// �Q�[���p�b�h�{�^�����X�e�B�b�N[�E�L�[ & �X�e�B�b�N�E]
		if (m_GamePad[idx].PadData.lX >= m_kInputDeep
			|| (m_GamePad[idx].PadData.rgdwPOV[0] >= JUDGE_RIGHT_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[0] <= JUDGE_RIGHT_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_RIGHT] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_RIGHT] = false;
		}
		// �Q�[���p�b�h�{�^�����X�e�B�b�N[��L�[ & �X�e�B�b�N��]
		if (m_GamePad[idx].PadData.lY <= -m_kInputDeep
			|| (m_GamePad[idx].PadData.rgdwPOV[0] >= JUDGE_UP_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[0] <= JUDGE_UP_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_UP] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_UP] = false;
		}
		// �Q�[���p�b�h�{�^�����X�e�B�b�N[���L�[ & �X�e�B�b�N��]
		if (m_GamePad[idx].PadData.lY >= m_kInputDeep
			|| (m_GamePad[idx].PadData.rgdwPOV[0] >= JUDGE_DOWN_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[0] <= JUDGE_DOWN_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_DOWN] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_DOWN] = false;
		}
		// �Q�[���p�b�h�{�^�����X�e�B�b�N[�E��]
		if ((m_GamePad[idx].PadData.lX >= m_kInputDeep * 0.5 && m_GamePad[idx].PadData.lY <= -m_kInputDeep * 0.5)
			|| (m_GamePad[idx].PadData.rgdwPOV[0] >= JUDGE_RIGHT_UP_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[0] <= JUDGE_RIGHT_UP_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_RIGHT_UP] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_RIGHT_UP] = false;
		}
		// �Q�[���p�b�h�{�^�����X�e�B�b�N[�E��]
		if ((m_GamePad[idx].PadData.lX >= m_kInputDeep * 0.5 && m_GamePad[idx].PadData.lY >= m_kInputDeep * 0.5)
			|| (m_GamePad[idx].PadData.rgdwPOV[0] >= JUDGE_RIGHT_DOWN_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[0] <= JUDGE_RIGHT_DOWN_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_RIGHT_DOWN] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_RIGHT_DOWN] = false;
		}
		// �Q�[���p�b�h�{�^�����X�e�B�b�N[����]
		if ((m_GamePad[idx].PadData.lX <= -m_kInputDeep * 0.5 && m_GamePad[idx].PadData.lY <= -m_kInputDeep * 0.5)
			|| (m_GamePad[idx].PadData.rgdwPOV[0] >= JUDGE_LEFT_UP_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[0] <= JUDGE_LEFT_UP_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_LEFT_UP] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_LEFT_UP] = false;
		}
		// �Q�[���p�b�h�{�^�����X�e�B�b�N[����]
		if ((m_GamePad[idx].PadData.lX <= -m_kInputDeep * 0.5 && m_GamePad[idx].PadData.lY >= m_kInputDeep * 0.5)
			|| (m_GamePad[idx].PadData.rgdwPOV[0] >= JUDGE_LEFT_DOWN_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[0] <= JUDGE_LEFT_DOWN_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_LEFT_DOWN] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][LEFT_STICK_LEFT_DOWN] = false;
		}


		// �Q�[���p�b�h�{�^���E�X�e�B�b�N[�E�L�[ & �X�e�B�b�N��]
		if (m_GamePad[idx].PadData.lRx <= -m_kInputDeep
			|| (m_GamePad[idx].PadData.rgdwPOV[1] >= JUDGE_LEFT_MAX_STICK && m_GamePad[idx].PadData.rgdwPOV[1] <= JUDGE_LEFT_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][RIGHT_STICK_LEFT] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][RIGHT_STICK_LEFT] = false;
		}
		// �Q�[���p�b�h�{�^���E�X�e�B�b�N[�E�L�[ & �X�e�B�b�N�E]
		if (m_GamePad[idx].PadData.lRx >= m_kInputDeep
			|| (m_GamePad[idx].PadData.rgdwPOV[1] >= JUDGE_RIGHT_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[1] <= JUDGE_RIGHT_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][RIGHT_STICK_RIGHT] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][RIGHT_STICK_RIGHT] = false;
		}
		// �Q�[���p�b�h�{�^���E�X�e�B�b�N[��L�[ & �X�e�B�b�N��]
		if (m_GamePad[idx].PadData.lRy <= -m_kInputDeep
			|| ((m_GamePad[idx].PadData.rgdwPOV[1] >= JUDGE_LEFT_MAX_STICK && m_GamePad[idx].PadData.rgdwPOV[1] <= JUDGE_MAX_STICK)
			|| (m_GamePad[idx].PadData.rgdwPOV[0] >= JUDGE_UP_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[0] <= JUDGE_UP_MAX_STICK)))
		{
			m_aKeyStateGamePad[idx][RIGHT_STICK_UP] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][RIGHT_STICK_UP] = false;
		}
		// �Q�[���p�b�h�{�^���E�X�e�B�b�N[���L�[ & �X�e�B�b�N��]
		if (m_GamePad[idx].PadData.lRy >= m_kInputDeep
			|| (m_GamePad[idx].PadData.rgdwPOV[1] >= JUDGE_DOWN_MIN_STICK && m_GamePad[idx].PadData.rgdwPOV[1] <= JUDGE_DOWN_MAX_STICK))
		{
			m_aKeyStateGamePad[idx][RIGHT_STICK_DOWN] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][RIGHT_STICK_DOWN] = false;
		}

		// �Q�[���p�b�h�{�^��[1]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_CHANGE_ATK]])
		{
			m_aKeyStateGamePad[idx][KEY_A] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][KEY_A] = false;
		}
		// �Q�[���p�b�h�{�^��[2]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_CHANGE_NORMAL]])
		{
			m_aKeyStateGamePad[idx][KEY_B] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][KEY_B] = false;
		}
		// �Q�[���p�b�h�{�^��[3]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_CHANGE_JAMMER]])
		{
			m_aKeyStateGamePad[idx][KEY_Y] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][KEY_Y] = false;
		}
		// �Q�[���p�b�h�{�^��[4]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_CHANGE_SPD]])
		{
			m_aKeyStateGamePad[idx][KEY_X] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][KEY_X] = false;
		}
		// �Q�[���p�b�h�{�^��[5]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_THREAD]])
		{
			m_aKeyStateGamePad[idx][KEY_L] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][KEY_L] = false;
		}
		// �Q�[���p�b�h�{�^��[6]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_ATTACK]])
		{
			m_aKeyStateGamePad[idx][KEY_R] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][KEY_R] = false;
		}
		// �Q�[���p�b�h�{�^��[7]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_START]])
		{
			m_aKeyStateGamePad[idx][KEY_START] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][KEY_START] = false;
		}
		// �Q�[���p�b�h�{�^��[8]
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_SELECT]])
		{
			m_aKeyStateGamePad[idx][KEY_SELECT] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][KEY_SELECT] = false;
		}

		// �Q�[���p�b�h�{�^��A,Y,X,R,L,START�̂����ꂩ
		if (m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_CHANGE_NORMAL]]
			|| m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_CHANGE_JAMMER]]
			|| m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_CHANGE_SPD]]
			|| m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_ATTACK]]
			|| m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_THREAD]]
			|| m_GamePad[idx].PadData.rgbButtons[m_aKeyConfig[idx][PAD_KEY_CHANGE_ATK]]
			)
		{
			m_aKeyStateGamePad[idx][KEY_DECIDE] = true;
		}
		else
		{
			m_aKeyStateGamePad[idx][KEY_DECIDE] = false;
		}
	}
}

//=============================================================================
// �W���C�p�b�h���擾
//=============================================================================
bool CInputGamePad::GetGamePadPress(KEY key, int playerID)
{
	if (playerID < 0 || playerID >= MAX_PAD_NUM)
	{
		return false;
	}
	return m_aKeyStateGamePad[playerID][key];
}

//=============================================================================
// �W���C�p�b�h���擾
//=============================================================================
bool CInputGamePad::GetGamePadTrigger(KEY key, int playerID)
{
	if (playerID < 0 || playerID >= MAX_PAD_NUM)
	{
		return false;
	}
	return m_aKeyStateTriggerGamePad[playerID][key];
}

//=============================================================================
// �W���C�p�b�h���擾
//=============================================================================
bool CInputGamePad::GetGamePadRelease(KEY key, int playerID)
{
	if (playerID < 0 || playerID >= MAX_PAD_NUM)
	{
		return false;
	}
	return m_aKeyStateReleaseGamePad[playerID][key];
}

//=============================================================================
// �W���C�p�b�h���擾
//=============================================================================
bool CInputGamePad::GetGamePadRepeat(KEY key, int playerID)
{
	if (playerID < 0 || playerID >= MAX_PAD_NUM)
	{
		return false;
	}
	return m_aKeyStateRepeatGamePad[playerID][key];
}

//=============================================================================
// �f���p�̃L�[���͕ۑ�
//=============================================================================
void CInputGamePad::KeepKeyForDemo(int frame)
{
	for(int idx = 0; idx < KEY_MAX_NUM; ++idx)
	{
		if(m_aKeyStateGamePad[idx])
		{
			SaveKeyForDemo((KEY)idx, frame);
		}
	}
}

//=============================================================================
// �f���p�̃L�[���͕ۑ�
//=============================================================================
void CInputGamePad::SaveKeyForDemo(KEY key, int idx)
{
	//if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	//{
	//	return;
	//}
	//
	//if(key == LEFT_STICK_LEFT)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_LEFT_KEY;
	//}
	//
	//if(key == LEFT_STICK_RIGHT)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_RIGHT_KEY;
	//}
	//
	//if(key == LEFT_STICK_UP)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_UP_KEY;
	//}
	//
	//if(key == LEFT_STICK_DOWN)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_DOWN_KEY;
	//}
	//
	//if(key == RIGHT_STICK_LEFT)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_LEFT_KEY;
	//}
	//
	//if(key == RIGHT_STICK_RIGHT)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_RIGHT_KEY;
	//}
	//
	//if(key == RIGHT_STICK_UP)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_UP_KEY;
	//}
	//
	//if(key == RIGHT_STICK_DOWN)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_DOWN_KEY;
	//}
	//
	//if(key == KEY_3)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | JUMP_KEY;
	//}
	//
	//if(key == KEY_1)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | KICK_KEY;
	//}
	//
	//if(key == KEY_10)
	//{
	//	m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_STICK_KEY;
	//}
}

//=============================================================================
// �f���p�̃L�[���̓t�@�C���o��
//=============================================================================
void CInputGamePad::OutPutKeyForDemo(void)
{
	// �������ݗp�t�@�C��
	FILE* fpw = fopen(DEMO_FILE_PATH, "wb");

	// ���s������
	if(fpw == NULL)
	{
		printf("�t�@�C���ǂݍ��ݎ��s\n");
		rewind(stdin);
		getchar();
	}

	// ����������
	else
	{
		// �L�[���o��
		fwrite(&m_nKeepDemoKey[0], sizeof(m_nKeepDemoKey), 1, fpw);
	}
	fclose(fpw);
}

//=============================================================================
// �f���p�̃L�[���̓t�@�C�����[�h
//=============================================================================
void CInputGamePad::LoadKeyForDemo(void)
{
	// �ǂݍ��ݗp�t�@�C��
	FILE* fpr = fopen(DEMO_FILE_PATH, "rb");

	// ���s������
	if(fpr == NULL)
	{
		printf("�t�@�C���ǂݍ��ݎ��s\n");
		rewind(stdin);
		getchar();
	}

	// ����������
	else
	{
		// �L�[���擾
		fread(&m_nKeepDemoKey[0], sizeof(m_nKeepDemoKey), 1, fpr);
	}
	fclose(fpr);
}

//=============================================================================
// �f���p�̃L�[�擾
//=============================================================================
bool CInputGamePad::GetKeyForDemo(KEY key, int idx)
{
	//// ���E�l����
	//if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	//{
	//	return false;
	//}
	//
	//if(key == LEFT_STICK_LEFT)
	//{
	//	if(m_nKeepDemoKey[idx] & L_LEFT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_RIGHT)
	//{
	//	if(m_nKeepDemoKey[idx] & L_RIGHT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_UP)
	//{
	//	if(m_nKeepDemoKey[idx] & L_UP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_DOWN)
	//{
	//	if(m_nKeepDemoKey[idx] & L_DOWN_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_LEFT)
	//{
	//	if(m_nKeepDemoKey[idx] & R_LEFT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_RIGHT)
	//{
	//	if(m_nKeepDemoKey[idx] & R_RIGHT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_UP)
	//{
	//	if(m_nKeepDemoKey[idx] & R_UP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_DOWN)
	//{
	//	if(m_nKeepDemoKey[idx] & R_DOWN_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_3)
	//{
	//	if(m_nKeepDemoKey[idx] & JUMP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_1)
	//{
	//	if(m_nKeepDemoKey[idx] & KICK_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_10)
	//{
	//	if(m_nKeepDemoKey[idx] & R_STICK_KEY)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

//=============================================================================
// �f���p�̃g���K�[�L�[�擾
//=============================================================================
bool CInputGamePad::GetTriggerKeyForDemo(KEY key, int idx)
{
	//// ���E�l����
	//if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	//{
	//	return false;
	//}
	//
	//if(key == LEFT_STICK_LEFT)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & L_LEFT_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & L_LEFT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_RIGHT)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & L_RIGHT_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & L_RIGHT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_UP)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & L_UP_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & L_UP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_DOWN)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & L_DOWN_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & L_DOWN_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_LEFT)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & R_LEFT_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_LEFT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_RIGHT)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & R_RIGHT_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_RIGHT_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_UP)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & R_UP_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_UP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_DOWN)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & R_DOWN_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_DOWN_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_3)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & JUMP_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & JUMP_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_1)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & KICK_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & KICK_KEY)
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_10)
	//{
	//	if(idx - 1 >= 0)
	//	{
	//		// �O�񉟂���Ă���
	//		if(m_nKeepDemoKey[idx - 1] & R_STICK_KEY)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	if(m_nKeepDemoKey[idx] & R_STICK_KEY)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

//=============================================================================
// �f���p�̃����[�X�L�[�擾
//=============================================================================
bool CInputGamePad::GetReleaseKeyForDemo(KEY key, int idx)
{
	//// ���E�l����
	//if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx <= 0)
	//{
	//	return false;
	//}
	//
	//if(key == LEFT_STICK_LEFT)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & L_LEFT_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & L_LEFT_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_RIGHT)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & L_RIGHT_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & L_RIGHT_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_UP)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & L_UP_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & L_UP_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == LEFT_STICK_DOWN)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & L_DOWN_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & L_DOWN_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_LEFT)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & R_LEFT_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_LEFT_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_RIGHT)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & R_RIGHT_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_RIGHT_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_UP)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & R_UP_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_UP_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == RIGHT_STICK_DOWN)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & R_DOWN_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_DOWN_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_3)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & JUMP_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & JUMP_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_1)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & KICK_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & KICK_KEY))
	//	{
	//		return true;
	//	}
	//}
	//
	//if(key == KEY_10)
	//{
	//	// �O�񉟂���ĂȂ��Ȃ�
	//	if(!(m_nKeepDemoKey[idx - 1] & R_STICK_KEY))
	//	{
	//		return false;
	//	}
	//
	//	if(!(m_nKeepDemoKey[idx] & R_STICK_KEY))
	//	{
	//		return true;
	//	}
	//}
	return false;
}

//======================================
// �L�[�R�t�B���O���Z�b�g
//======================================
void CInputGamePad::SetKeyCofingInfo(int id, int* keyConfigInfo)
{
	if (id < 0 || id >= MAX_PAD_NUM)
	{
		return;
	}
	for (int i = 0; i < PAD_KEY_MAX; ++i)
	{
		m_aKeyConfig[id][i] = keyConfigInfo[i];
	}
}

//======================================
// �����g���K�[������
//======================================
bool CInputGamePad::CheckTriggerAnyKey(int ID, int* pCord)
{
	if (ID < 0 || ID >= MAX_PAD_NUM)
	{
		return false;
	}

	if (!m_GamePad[ID].pPadDevice)
	{
		return false;
	}
	for (int pos = 0; pos < 127; pos++)
	{
		if (m_GamePad[ID].OldPadData.rgbButtons[pos] != 0)
		{
			continue;
		}
		if (m_GamePad[ID].PadData.rgbButtons[pos] != 0)
		{
			*pCord = pos;
			return true;
		}
	}

	return false;
}

//======================================
// �R���g���[���[�q�����Ă邩���ׂ�
//======================================
bool CInputGamePad::CheckConectPad(int ID)
{
	if (ID >= MAX_PAD_NUM || ID < 0)
	{
		return true;
	}

	if (!m_GamePad[ID].pPadDevice)
	{
		return false;
	}
	return true;
}

//----EOF----