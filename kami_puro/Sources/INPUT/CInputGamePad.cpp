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
static const int	JUDGE_LEFT_MIN_STICK = 225 * 100;				// ������̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_LEFT_MAX_STICK = 315 * 100;				// ������̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_RIGHT_MIN_STICK = 45 * 100;				// �E����̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_RIGHT_MAX_STICK = 135 * 100;				// �E����̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_UP_MIN_STICK = 0 * 100;					// �㔻��̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_UP_MAX_STICK = 45 * 100;					// �㔻��̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_DOWN_MIN_STICK = 135 * 100;				// ������̃X�e�B�b�N�̍ŏ��X��
static const int	JUDGE_DOWN_MAX_STICK = 225 * 100;				// ������̃X�e�B�b�N�̍ő�X��
static const int	JUDGE_MAX_STICK = 360 * 100;					// �X�e�B�b�N�̍ő�X��

//**********************************************
// �X�^�e�B�b�N�����o�ϐ�
//**********************************************
GUID	CInputGamePad::m_pad_discrimination;
GAMEPAD	CInputGamePad::m_GamePad;
bool	CInputGamePad::m_aKeyStateGamePad[KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateTriggerGamePad[KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateReleaseGamePad[KEY_MAX_NUM];
bool	CInputGamePad::m_aKeyStateRepeatGamePad[KEY_MAX_NUM];
int		CInputGamePad::m_aKeyStateRepeatCntGamePad[KEY_MAX_NUM];
unsigned short	CInputGamePad::m_nKeepDemoKey[MAX_KEEP_KEY_FOR_DEMO];
int		CInputGamePad::m_nKeyFrameForDemo;
//==============================================
// �R���X�g���N�^
//==============================================
CInputGamePad::CInputGamePad(void)
{
	m_nKeyFrameForDemo = 0;
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
	ed.ppPadDevice = &m_GamePad.pPadDevice;

	hr = m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL	// �Q�[���R���g���[���[���Ώ�
								, EnumGamePad			// �񋓂���֐�
								, &ed					// �񋓊֐��ɓn�������f�[�^�͂����ɓ����
								, DIEDFL_ATTACHEDONLY);	// �C���X�g�[���E�ڑ��ς݂̃f�o�C�X�̂ݎ擾

	// �Q�[���p�b�h�̓��͏���DIJOYSTATE2�Ɋi�[�����̂�
	// �f�[�^�t�H�[�}�b�g�ɂ�c_dfDIJoystick2���w��
	if(m_GamePad.pPadDevice != NULL)
	{
		// �A�i���O�L�[�̃f�[�^��ݒ�
		// �Q�Ԗڂ̈�����pPadDevice���w�肵�Ă����ƁA
		// EnumObject�֐���pvRef�ɁA����pPadDevice���i�[�����悤�ɂȂ邽�߁A
		// pPadDevice���X�^�e�B�b�N��O���[�o���ϐ��ɂ��Ȃ��čςށB
		m_GamePad.pPadDevice->EnumObjects(EnumObject, m_GamePad.pPadDevice, DIDFT_AXIS);

		m_GamePad.pPadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		hr = m_GamePad.pPadDevice->SetDataFormat(&c_dfDIJoystick2);
				
		// ���s������J��
		if(FAILED(hr))
		{
			if(m_GamePad.pPadDevice != NULL)
			{
				m_GamePad.pPadDevice->Release();
				m_GamePad.pPadDevice = NULL;
			}
		}

		hr = m_GamePad.pPadDevice->SetDataFormat(&c_dfDIJoystick2);

		// ���s������J��
		if(FAILED(hr))
		{
			if(m_GamePad.pPadDevice != NULL)
			{
				m_GamePad.pPadDevice->Release();
				m_GamePad.pPadDevice = NULL;
			}
		}

		hr = m_GamePad.pPadDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		// ���s������J��
		if(FAILED(hr))
		{
			MessageBox(NULL,"�������[�h�̐ݒ莸�s","�������[�h�̐ݒ莸�s",MB_OK);
			return hr;
		}

		// �R���g���[���[�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_GamePad.pPadDevice->Acquire();
	}
	m_GamePad.DirectryData = m_GamePad.OldDirectryData = PP_NONE;

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
	if(m_GamePad.pPadDevice)
	{
		m_GamePad.pPadDevice->Unacquire();
		m_GamePad.pPadDevice->Release();
		m_GamePad.pPadDevice = NULL;
	}
}

//==============================================
// �X�V
//==============================================
void CInputGamePad::Update(void)
{
	HRESULT hr;
	bool aKeyStateOld[KEY_MAX];

	if(!m_GamePad.pPadDevice)
	{
		return;
	}

	// �O��̃f�[�^��ۑ�
	for(int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
	{
		aKeyStateOld[nCntKey] = m_aKeyStateGamePad[nCntKey];
	}

	// �f�o�C�X����f�[�^���擾
	hr = m_GamePad.pPadDevice->GetDeviceState(sizeof(m_GamePad.PadData), &m_GamePad.PadData);
	if(SUCCEEDED(hr))
	{
		// �L�[���ݒ�
		SetKeyStateGamePad();
		for(int nCntKey = 0; nCntKey < KEY_MAX; nCntKey++)
		{
			// �g���K�[�L�[�X�V
			m_aKeyStateTriggerGamePad[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateGamePad[nCntKey]) & m_aKeyStateGamePad[nCntKey];
			
			// �����[�X�L�[�X�V
			m_aKeyStateReleaseGamePad[nCntKey] = (aKeyStateOld[nCntKey] ^ m_aKeyStateGamePad[nCntKey]) & !m_aKeyStateGamePad[nCntKey];
			
			// ���s�[�g�L�[�X�V
			m_aKeyStateRepeatGamePad[nCntKey] = m_aKeyStateTriggerGamePad[nCntKey];
			if(m_aKeyStateGamePad[nCntKey])
			{
				m_aKeyStateRepeatCntGamePad[nCntKey]++;
				if(m_aKeyStateRepeatCntGamePad[nCntKey] >= JUDGE_TIME_REPEAT_KEY)
				{
					m_aKeyStateRepeatGamePad[nCntKey] = m_aKeyStateGamePad[nCntKey];
				}
			}
			else
			{
				m_aKeyStateRepeatCntGamePad[nCntKey] = 0;
				m_aKeyStateRepeatGamePad[nCntKey] = 0;
			}
		}
	}
	else
	{
		// �����l��
		m_GamePad.pPadDevice->Acquire();
	}
#ifdef _DEBUG
	for(int pos = 0; pos < 127; pos++)
	{
		if(m_GamePad.PadData.rgbButtons[pos] != 0)
		{
			CDebugProc::Print("�{�^���R�[�h�F%d",pos);
		}
	}
#endif
}

//==============================================
// �R�[���o�b�N�֐�
//==============================================
BOOL CALLBACK CInputGamePad::EnumGamePad(const DIDEVICEINSTANCE* pInstance, LPVOID pContext)
{
	STR_ENUMDATA *ed = (STR_ENUMDATA*)pContext;
	DIDEVCAPS diDevCaps;							// �W���C�p�b�h�̔\�͏��

	// ���Ɏ擾�ς݂̃f�o�C�X�������ꍇ�͂��������s����
	if(m_pad_discrimination == pInstance->guidInstance)
	{
		return DIENUM_CONTINUE;
	}
	m_pad_discrimination = pInstance->guidInstance;

	HRESULT hr;
	
	// �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾
	hr = ed->pInput->CreateDevice(pInstance->guidInstance, ed->ppPadDevice, NULL);
	if(FAILED(hr))
	{
		return DIENUM_CONTINUE;  // �f�o�C�X���쐬�ł��Ȃ��̂ŗ񋓂𑱂���
	}
	
	// �W���C�p�b�h�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	
	// �f�o�C�X�̎��ʎq��ۑ�
	m_pad_discrimination = pInstance->guidInstance;

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
	// �Q�[���p�b�h�{�^�����X�e�B�b�N[���L�[ & �X�e�B�b�N��]
	if(m_GamePad.PadData.lX <= -m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_LEFT_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_LEFT_MAX_STICK))
	{
		m_aKeyStateGamePad[LEFT_STICK_LEFT] = true;
	}
	else
	{
		m_aKeyStateGamePad[LEFT_STICK_LEFT] = false;
	}
	// �Q�[���p�b�h�{�^�����X�e�B�b�N[�E�L�[ & �X�e�B�b�N�E]
	if(m_GamePad.PadData.lX >= m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_RIGHT_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_RIGHT_MAX_STICK))
	{
		m_aKeyStateGamePad[LEFT_STICK_RIGHT] = true;
	}
	else
	{
		m_aKeyStateGamePad[LEFT_STICK_RIGHT] = false;
	}
	// �Q�[���p�b�h�{�^�����X�e�B�b�N[��L�[ & �X�e�B�b�N��]
	if(m_GamePad.PadData.lY <= -m_kInputDeep
	|| ((m_GamePad.PadData.rgdwPOV[0] >= JUDGE_LEFT_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_LEFT_MAX_STICK)
		|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_UP_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_UP_MAX_STICK)))
	{
		m_aKeyStateGamePad[LEFT_STICK_UP] = true;
	}
	else
	{
		m_aKeyStateGamePad[LEFT_STICK_UP] = false;
	}
	// �Q�[���p�b�h�{�^�����X�e�B�b�N[���L�[ & �X�e�B�b�N��]
	if(m_GamePad.PadData.lY >= m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_DOWN_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_DOWN_MAX_STICK))
	{
		m_aKeyStateGamePad[LEFT_STICK_DOWN] = true;
	}
	else
	{
		m_aKeyStateGamePad[LEFT_STICK_DOWN] = false;
	}


	// �Q�[���p�b�h�{�^���E�X�e�B�b�N[�E�L�[ & �X�e�B�b�N��]
	if(m_GamePad.PadData.lRx <= -m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[1] >= JUDGE_LEFT_MAX_STICK && m_GamePad.PadData.rgdwPOV[1] <= JUDGE_LEFT_MAX_STICK))
	{
		m_aKeyStateGamePad[RIGHT_STICK_LEFT] = true;
	}
	else
	{
		m_aKeyStateGamePad[RIGHT_STICK_LEFT] = false;
	}
	// �Q�[���p�b�h�{�^���E�X�e�B�b�N[�E�L�[ & �X�e�B�b�N�E]
	if(m_GamePad.PadData.lRx >= m_kInputDeep
		|| (m_GamePad.PadData.rgdwPOV[1] >= JUDGE_RIGHT_MIN_STICK && m_GamePad.PadData.rgdwPOV[1] <= JUDGE_RIGHT_MAX_STICK))
	{
		m_aKeyStateGamePad[RIGHT_STICK_RIGHT] = true;
	}
	else
	{
		m_aKeyStateGamePad[RIGHT_STICK_RIGHT] = false;
	}
	// �Q�[���p�b�h�{�^���E�X�e�B�b�N[��L�[ & �X�e�B�b�N��]
	if(m_GamePad.PadData.lRy <= -m_kInputDeep
		|| ((m_GamePad.PadData.rgdwPOV[1] >= JUDGE_LEFT_MAX_STICK && m_GamePad.PadData.rgdwPOV[1] <= JUDGE_MAX_STICK)
		|| (m_GamePad.PadData.rgdwPOV[0] >= JUDGE_UP_MIN_STICK && m_GamePad.PadData.rgdwPOV[0] <= JUDGE_UP_MAX_STICK)))
	{
		m_aKeyStateGamePad[RIGHT_STICK_UP] = true;
	}
	else
	{
		m_aKeyStateGamePad[RIGHT_STICK_UP] = false;
	}
	// �Q�[���p�b�h�{�^���E�X�e�B�b�N[���L�[ & �X�e�B�b�N��]
	if(m_GamePad.PadData.lRy >= m_kInputDeep
	|| (m_GamePad.PadData.rgdwPOV[1] >= JUDGE_DOWN_MIN_STICK && m_GamePad.PadData.rgdwPOV[1] <= JUDGE_DOWN_MAX_STICK))
	{
		m_aKeyStateGamePad[RIGHT_STICK_DOWN] = true;
	}
	else
	{
		m_aKeyStateGamePad[RIGHT_STICK_DOWN] = false;
	}

	// �Q�[���p�b�h�{�^��[1]
	if(m_GamePad.PadData.rgbButtons[0])
	{
		m_aKeyStateGamePad[KEY_1] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_1] = false;
	}
	// �Q�[���p�b�h�{�^��[2]
	if(m_GamePad.PadData.rgbButtons[1])
	{
		m_aKeyStateGamePad[KEY_2] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_2] = false;
	}
	// �Q�[���p�b�h�{�^��[3]
	if(m_GamePad.PadData.rgbButtons[2])
	{
		m_aKeyStateGamePad[KEY_3] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_3] = false;
	}
	// �Q�[���p�b�h�{�^��[4]
	if(m_GamePad.PadData.rgbButtons[3])
	{
		m_aKeyStateGamePad[KEY_4] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_4] = false;
	}
	// �Q�[���p�b�h�{�^��[5]
	if(m_GamePad.PadData.rgbButtons[4])
	{
		m_aKeyStateGamePad[KEY_5] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_5] = false;
	}
	// �Q�[���p�b�h�{�^��[6]
	if(m_GamePad.PadData.rgbButtons[5])
	{
		m_aKeyStateGamePad[KEY_6] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_6] = false;
	}
	// �Q�[���p�b�h�{�^��[7]
	if(m_GamePad.PadData.rgbButtons[6])
	{
		m_aKeyStateGamePad[KEY_7] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_7] = false;
	}
	// �Q�[���p�b�h�{�^��[8]
	if(m_GamePad.PadData.rgbButtons[7])
	{
		m_aKeyStateGamePad[KEY_8] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_8] = false;
	}
	// �Q�[���p�b�h�{�^��[9]
	if(m_GamePad.PadData.rgbButtons[8])
	{
		m_aKeyStateGamePad[KEY_9] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_9] = false;
	}
	// �Q�[���p�b�h�{�^��[10]
	if(m_GamePad.PadData.rgbButtons[9])
	{
		m_aKeyStateGamePad[KEY_10] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_10] = false;
	}
	// �Q�[���p�b�h�{�^��[11]
	if(m_GamePad.PadData.rgbButtons[10])
	{
		m_aKeyStateGamePad[KEY_11] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_11] = false;
	}
	// �Q�[���p�b�h�{�^��[12]
	if(m_GamePad.PadData.rgbButtons[11])
	{
		m_aKeyStateGamePad[KEY_12] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_12] = false;
	}

	// �Q�[���p�b�h�{�^��[1][2][3][4]�̂����ꂩ
	if(m_GamePad.PadData.rgbButtons[0]
	|| m_GamePad.PadData.rgbButtons[1]
	|| m_GamePad.PadData.rgbButtons[2]
	|| m_GamePad.PadData.rgbButtons[3])
	{
		m_aKeyStateGamePad[KEY_DECIDE] = true;
	}
	else
	{
		m_aKeyStateGamePad[KEY_DECIDE] = false;
	}
}

//=============================================================================
// �W���C�p�b�h���擾
//=============================================================================
bool CInputGamePad::GetGamePadPress(KEY key)
{
	return m_aKeyStateGamePad[key];
}

//=============================================================================
// �W���C�p�b�h���擾
//=============================================================================
bool CInputGamePad::GetGamePadTrigger(KEY key)
{
	return m_aKeyStateTriggerGamePad[key];
}

//=============================================================================
// �W���C�p�b�h���擾
//=============================================================================
bool CInputGamePad::GetGamePadRelease(KEY key)
{
	return m_aKeyStateReleaseGamePad[key];
}

//=============================================================================
// �W���C�p�b�h���擾
//=============================================================================
bool CInputGamePad::GetGamePadRepeat(KEY key)
{
	return m_aKeyStateRepeatGamePad[key];
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
	if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	{
		return;
	}

	if(key == LEFT_STICK_LEFT)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_LEFT_KEY;
	}

	if(key == LEFT_STICK_RIGHT)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_RIGHT_KEY;
	}

	if(key == LEFT_STICK_UP)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_UP_KEY;
	}

	if(key == LEFT_STICK_DOWN)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | L_DOWN_KEY;
	}

	if(key == RIGHT_STICK_LEFT)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_LEFT_KEY;
	}

	if(key == RIGHT_STICK_RIGHT)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_RIGHT_KEY;
	}

	if(key == RIGHT_STICK_UP)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_UP_KEY;
	}

	if(key == RIGHT_STICK_DOWN)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_DOWN_KEY;
	}

	if(key == KEY_3)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | JUMP_KEY;
	}

	if(key == KEY_1)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | KICK_KEY;
	}

	if(key == KEY_10)
	{
		m_nKeepDemoKey[idx] = m_nKeepDemoKey[idx] | R_STICK_KEY;
	}
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
	// ���E�l����
	if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	{
		return false;
	}

	if(key == LEFT_STICK_LEFT)
	{
		if(m_nKeepDemoKey[idx] & L_LEFT_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_RIGHT)
	{
		if(m_nKeepDemoKey[idx] & L_RIGHT_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_UP)
	{
		if(m_nKeepDemoKey[idx] & L_UP_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_DOWN)
	{
		if(m_nKeepDemoKey[idx] & L_DOWN_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_LEFT)
	{
		if(m_nKeepDemoKey[idx] & R_LEFT_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_RIGHT)
	{
		if(m_nKeepDemoKey[idx] & R_RIGHT_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_UP)
	{
		if(m_nKeepDemoKey[idx] & R_UP_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_DOWN)
	{
		if(m_nKeepDemoKey[idx] & R_DOWN_KEY)
		{
			return true;
		}
	}

	if(key == KEY_3)
	{
		if(m_nKeepDemoKey[idx] & JUMP_KEY)
		{
			return true;
		}
	}

	if(key == KEY_1)
	{
		if(m_nKeepDemoKey[idx] & KICK_KEY)
		{
			return true;
		}
	}

	if(key == KEY_10)
	{
		if(m_nKeepDemoKey[idx] & R_STICK_KEY)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// �f���p�̃g���K�[�L�[�擾
//=============================================================================
bool CInputGamePad::GetTriggerKeyForDemo(KEY key, int idx)
{
	// ���E�l����
	if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx < 0)
	{
		return false;
	}

	if(key == LEFT_STICK_LEFT)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & L_LEFT_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & L_LEFT_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_RIGHT)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & L_RIGHT_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & L_RIGHT_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_UP)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & L_UP_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & L_UP_KEY)
		{
			return true;
		}
	}

	if(key == LEFT_STICK_DOWN)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & L_DOWN_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & L_DOWN_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_LEFT)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & R_LEFT_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_LEFT_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_RIGHT)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & R_RIGHT_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_RIGHT_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_UP)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & R_UP_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_UP_KEY)
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_DOWN)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & R_DOWN_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_DOWN_KEY)
		{
			return true;
		}
	}

	if(key == KEY_3)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & JUMP_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & JUMP_KEY)
		{
			return true;
		}
	}

	if(key == KEY_1)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & KICK_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & KICK_KEY)
		{
			return true;
		}
	}

	if(key == KEY_10)
	{
		if(idx - 1 >= 0)
		{
			// �O�񉟂���Ă���
			if(m_nKeepDemoKey[idx - 1] & R_STICK_KEY)
			{
				return false;
			}
		}

		if(m_nKeepDemoKey[idx] & R_STICK_KEY)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// �f���p�̃����[�X�L�[�擾
//=============================================================================
bool CInputGamePad::GetReleaseKeyForDemo(KEY key, int idx)
{
	// ���E�l����
	if(idx >= MAX_KEEP_KEY_FOR_DEMO || idx <= 0)
	{
		return false;
	}

	if(key == LEFT_STICK_LEFT)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & L_LEFT_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & L_LEFT_KEY))
		{
			return true;
		}
	}

	if(key == LEFT_STICK_RIGHT)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & L_RIGHT_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & L_RIGHT_KEY))
		{
			return true;
		}
	}

	if(key == LEFT_STICK_UP)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & L_UP_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & L_UP_KEY))
		{
			return true;
		}
	}

	if(key == LEFT_STICK_DOWN)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & L_DOWN_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & L_DOWN_KEY))
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_LEFT)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & R_LEFT_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_LEFT_KEY))
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_RIGHT)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & R_RIGHT_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_RIGHT_KEY))
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_UP)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & R_UP_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_UP_KEY))
		{
			return true;
		}
	}

	if(key == RIGHT_STICK_DOWN)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & R_DOWN_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_DOWN_KEY))
		{
			return true;
		}
	}

	if(key == KEY_3)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & JUMP_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & JUMP_KEY))
		{
			return true;
		}
	}

	if(key == KEY_1)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & KICK_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & KICK_KEY))
		{
			return true;
		}
	}

	if(key == KEY_10)
	{
		// �O�񉟂���ĂȂ��Ȃ�
		if(!(m_nKeepDemoKey[idx - 1] & R_STICK_KEY))
		{
			return false;
		}

		if(!(m_nKeepDemoKey[idx] & R_STICK_KEY))
		{
			return true;
		}
	}
	return false;
}

//----EOF----