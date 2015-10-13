//=============================================================================
//
// CController�N���X [ CController.cpp]
// Author : ��K�@����
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CController.h"
#include <stdio.h>

//=============================================================================
// �}�N��
//=============================================================================
static const int DEFAULT_KEY_CORD[CInputGamePad::PAD_KEY_MAX] = { 0, 1, 2, 3, 4, 5, 6, 7 };


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CController::CController(void)
{
	m_nID = -1;

	for (int i = 0; i < CInputGamePad::PAD_KEY_MAX; ++i)
	{
		m_nButtonKeyCord[i] = DEFAULT_KEY_CORD[i];
	}
	
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CController::~CController(void)
{

}

//=============================================================================
// ������
//=============================================================================
void CController::Init(int ID, const char* keyCofingFile)
{
	if (keyCofingFile)
	{
		LoadKeyCofingFile(keyCofingFile);
	}

	m_nID = ID;

	// �L�[���Z�b�g
	UpdateKeyCord();
}

//=============================================================================
// �I��
//=============================================================================
void CController::Uninit(void)
{

}

//=============================================================================
// �N���G�C�g
//=============================================================================
CController* CController::Create(int ID, const char* keyCofingFile)
{
	CController* p = new CController;

	p->Init(ID, keyCofingFile);

	return p;
}

//=============================================================================
// �g���K�[�L�[�擾
//=============================================================================
bool CController::GetTriggerKey(CInputGamePad::KEY key)
{
	return CInputGamePad::GetGamePadTrigger(key, m_nID);
}

//=============================================================================
// �v���X�L�[�擾
//=============================================================================
bool CController::GetPressKey(CInputGamePad::KEY key)
{
	return CInputGamePad::GetGamePadPress(key, m_nID);
}

//=============================================================================
// �����[�X�L�[�擾
//=============================================================================
bool CController::GetReleaseKey(CInputGamePad::KEY key)
{
	return CInputGamePad::GetGamePadRelease(key, m_nID);
}

//=============================================================================
// ���s�[�g�L�[�擾
//=============================================================================
bool CController::GetRepeatKey(CInputGamePad::KEY key)
{
	return CInputGamePad::GetGamePadRepeat(key, m_nID);
}

//=============================================================================
// �L�[�R�t�B���O�t�@�C���̃��[�h
//=============================================================================
void CController::LoadKeyCofingFile(const char* keyCofingFile)
{
	// �ǂݍ��ݗp�t�@�C��
	FILE* fpr = fopen(keyCofingFile, "r");

	// ���s������
	if (fpr == NULL)
	{
//		fclose(fpr);
		return;
		//MessageBox(NULL, "�L�[�R���t�B�O�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK | MB_ICONERROR);
	}

	// ����������
	else
	{
		for (int i = 0; i < CInputGamePad::PAD_KEY_MAX; ++i)
		{
			fscanf(fpr, "%d", &m_nButtonKeyCord[i]);
		}

	}
	fclose(fpr);
}

//=============================================================================
// �L�[�R�t�B���O�t�@�C���̃Z�[�u
//=============================================================================
void CController::SaveKeyCofingFile(const char* keyCofingFile)
{
	// �������ݗp�t�@�C��
	FILE* fpw = fopen(keyCofingFile, "w");

	// ���s������
	if (fpw == NULL)
	{
		fclose(fpw);
		return;
		//MessageBox(NULL, "�L�[�R���t�B�O�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK | MB_ICONERROR);
	}

	// ����������
	else
	{
		for (int i = 0; i < CInputGamePad::PAD_KEY_MAX; ++i)
		{
			fprintf(fpw, "%d\n", m_nButtonKeyCord[i]);
		}

	}
	fclose(fpw);
}

//======================================
// �L�[�R�t�B���O���̍X�V
//======================================
void CController::UpdateKeyCord(void)
{
	CInputGamePad::SetKeyCofingInfo(m_nID, &m_nButtonKeyCord[0]);
}

//-----EOF-----