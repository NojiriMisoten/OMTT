//=============================================================================
//
// CControllerManager�N���X [ CControllerManager.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CCONTROLLER_MANAGER_H_
#define _CCONTROLLER_MANAGER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../INPUT/CInputGamePad.h"

//=============================================================================
// �O���錾
//=============================================================================
class CController;

//=============================================================================
// �N���X��`
//=============================================================================
class CControllerManager
{
public:
	// �R���X�g���N�^
	CControllerManager(void);

	// �f�X�g���N�^
	~CControllerManager(void);

	// �I��
	static void Uninit(void);

	//======================================
	// �N���G�C�g
	// �߂�l�F�R���g���[���[�}�l�[�W���[�̃A�h���X
	//======================================
	static CControllerManager* Create(void);

	//======================================
	// �g���K�[�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	static bool GetTriggerKey(CInputGamePad::CONTROLLER_BUTTON key, int ID);

	//======================================
	// �v���X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: �����ꂽ�u�Ԃ�
	//======================================
	static bool GetPressKey(CInputGamePad::CONTROLLER_BUTTON key, int ID);

	//======================================
	// �����[�X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: �����ꂽ�u�Ԃ�
	//======================================
	static bool GetReleaseKey(CInputGamePad::CONTROLLER_BUTTON key, int ID);

	//======================================
	// ���s�[�g�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: �����ꂽ�u�Ԃ�
	//======================================
	static bool GetRepeatKey(CInputGamePad::CONTROLLER_BUTTON key, int ID);

	//======================================
	// �L�[�R�t�B���O�t�@�C���̃Z�[�u
	// ����: �t�@�C���l�[��
	//======================================
	static void SaveKeyCofingFile(void);

	//======================================
	// �L�[�R�t�B���O�t�@�C���̃Z�b�g
	// ����: �R���g���[���[�ԍ�,�ǂ̃{�^�����A�L�[�R�[�h
	//======================================
	static void SetKeyCord(int ID, CInputGamePad::CONTROLLER_BUTTON key, int cord);

	//======================================
	// �L�[�R�t�B���O���̃Q�b�g
	// ����: �R���g���[���[�ԍ�,�ǂ̃{�^����
	// �߂�l: �L�[�R�[�h
	//======================================
	static int GetKeyCord(int ID, CInputGamePad::CONTROLLER_BUTTON key);

	//======================================
	// �L�[�R�t�B���O���̍X�V
	//======================================
	static void UpdateKeyCord(void);

private:
	// ������
	static void Init(void);

	static CController* m_pController[CInputGamePad::MAX_PAD_NUM];		// �R���g���[���[
};



#endif
//-----EOF-----