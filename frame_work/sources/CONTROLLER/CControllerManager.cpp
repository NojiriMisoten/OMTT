//=============================================================================
//
// CControllerManager�N���X [ CControllerManager.cpp]
// Author : ��K�@����
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CControllerManager.h"
#include "CController.h"

//=============================================================================
// �}�N��
//=============================================================================
static const char* KEY_COFING_FILE[CControllerManager::MAX_CONTROLLER_NUM] = 
{
	"data/KEY_CONFIG/controller1.txt",
	"data/KEY_CONFIG/controller2.txt",
	"data/KEY_CONFIG/controller3.txt",
	"data/KEY_CONFIG/controller4.txt",
};

//=============================================================================
// static�����o�ϐ�
//=============================================================================
CController* CControllerManager::m_pController[MAX_CONTROLLER_NUM];

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControllerManager::CControllerManager(void)
{
	for (int i = 0; i < MAX_CONTROLLER_NUM; ++i)
	{
		m_pController[i] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControllerManager::~CControllerManager(void)
{
	for (int i = 0; i < MAX_CONTROLLER_NUM; ++i)
	{
		m_pController[i] = NULL;
	}
}

//=============================================================================
// ������
//=============================================================================
void CControllerManager::Init(void)
{
	for (int i = 0; i < MAX_CONTROLLER_NUM; ++i)
	{
		m_pController[i] = CController::Create(i, KEY_COFING_FILE[i]);
	}
}

//=============================================================================
// �I��
//=============================================================================
void CControllerManager::Uninit(void)
{
	for (int i = 0; i < MAX_CONTROLLER_NUM; ++i)
	{
		if (m_pController[i])
		{
			m_pController[i]->Uninit();
			delete m_pController[i];
			m_pController[i] = NULL;
		}
	}
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CControllerManager* CControllerManager::Create(void)
{
	CControllerManager* p = new CControllerManager;

	p->Init();

	return p;
}

//=============================================================================
// �g���K�[�L�[�擾
//=============================================================================
bool CControllerManager::GetTriggerKey(CInputGamePad::KEY key, int ID)
{
	return m_pController[ID]->GetTriggerKey(key);
}

//=============================================================================
// �v���X�L�[�擾
//=============================================================================
bool CControllerManager::GetPressKey(CInputGamePad::KEY key, int ID)
{
	return m_pController[ID]->GetPressKey(key);
}

//=============================================================================
// �����[�X�L�[�擾
//=============================================================================
bool CControllerManager::GetReleaseKey(CInputGamePad::KEY key, int ID)
{
	return m_pController[ID]->GetReleaseKey(key);
}

//=============================================================================
// ���s�[�g�L�[�擾
//=============================================================================
bool CControllerManager::GetRepeatKey(CInputGamePad::KEY key, int ID)
{
	return m_pController[ID]->GetRepeatKey(key);
}

//=============================================================================
// �L�[�R�t�B���O�t�@�C���̃Z�[�u
//=============================================================================
void CControllerManager::SaveKeyCofingFile(void)
{
	for (int i = 0; i < MAX_CONTROLLER_NUM; ++i)
	{
		m_pController[i]->SaveKeyCofingFile(KEY_COFING_FILE[i]);
	}
}

//======================================
// �L�[�R�t�B���O�t�@�C���̃Z�b�g
//======================================
void CControllerManager::SetKeyCord(int ID, CInputGamePad::PAD_BUTTON key, int cord)
{
	m_pController[ID]->SetKeyCord(key, cord);
}

//======================================
// �L�[�R�t�B���O���̍X�V
//======================================
void CControllerManager::UpdateKeyCord(void)
{
	for (int i = 0; i < MAX_CONTROLLER_NUM; ++i)
	{
		m_pController[i]->UpdateKeyCord();
	}
}

//======================================
// �L�[�R�t�B���O���̃Q�b�g
//======================================
int CControllerManager::GetKeyCord(int ID, CInputGamePad::PAD_BUTTON key)
{
	return m_pController[ID]->GetKeyCord(key);
}

//-----EOF-----