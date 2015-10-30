//=============================================================================
//
// CController�N���X [ CController.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CCONTROLLER_H_
#define _CCONTROLLER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../INPUT/CInputGamePad.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CController
{
public:

	// �R���X�g���N�^
	CController(void);

	// �f�X�g���N�^
	~CController(void);

	// �I��
	void Uninit(void);

	//======================================
	// �N���G�C�g
	// �����F�h�c, �L�[�R���t�B�O�t�@�C��
	// �߂�l�F�A�h���X
	//======================================
	static CController* Create(int ID, const char* keyCofingFile);

	//======================================
	// �g���K�[�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: ������Ă邩
	//======================================
	bool GetTriggerKey(CInputGamePad::CONTROLLER_BUTTON key);

	//======================================
	// �v���X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: �����ꂽ�u�Ԃ�
	//======================================
	bool GetPressKey(CInputGamePad::CONTROLLER_BUTTON key);

	//======================================
	// �����[�X�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: �����ꂽ�u�Ԃ�
	//======================================
	bool GetReleaseKey(CInputGamePad::CONTROLLER_BUTTON key);

	//======================================
	// ���s�[�g�L�[�擾
	// ����: ���ׂ����L�[
	// �߂�l: �����ꂽ�u�Ԃ�
	//======================================
	bool GetRepeatKey(CInputGamePad::CONTROLLER_BUTTON key);

	//======================================
	// �L�[�R�t�B���O�t�@�C���̃Z�[�u
	// ����: �t�@�C���l�[��
	//======================================
	void SaveKeyCofingFile(const char* keyCofingFile);

	//======================================
	// �L�[�R�t�B���O�t�@�C���̃Z�b�g
	// ����: �ǂ̃{�^�����A�L�[�R�[�h
	//======================================
	void SetKeyCord(CInputGamePad::CONTROLLER_BUTTON key, int cord){ m_nButtonKeyCord[key] = cord; };

	//======================================
	// �L�[�R�t�B���O�t�@�C���̃Q�b�g
	// ����: �ǂ̃{�^����
	// �߂�l�F�L�[�R�[�h
	//======================================
	int GetKeyCord(CInputGamePad::CONTROLLER_BUTTON key){ return m_nButtonKeyCord[key]; };

	//======================================
	// �L�[�R�t�B���O���̍X�V
	//======================================
	void UpdateKeyCord(void);

private:
	//======================================
	// ������
	// �����F�h�c, �L�[�R���t�B�O�t�@�C��
	//======================================
	void Init(int ID, const char* keyCofingFile);

	//======================================
	// �L�[�R�t�B���O�t�@�C���̃��[�h
	// ����: �t�@�C���l�[��
	//======================================
	void LoadKeyCofingFile(const char* keyCofingFile);

	int		m_nID;										// �R���g���[���[��ID
	int		m_nButtonKeyCord[CInputGamePad::CONTROLLER_BUTTON_MAX];	// �{�^���̃L�[�R�[�h
};



#endif
//-----EOF-----