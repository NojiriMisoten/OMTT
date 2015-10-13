//=============================================================================
//
// CTimeManager�N���X [CTimeManager.h]
// Author : �˖{�@�r�F
//
// �Q�[���̎c�莞�Ԃ�\���A�Ǘ�����N���X
// �X�R�A�N���X���ÓI�ň�������Ȃ������׎c�莞�ԕ\���p�Ɏg�p�B
// ���̒萔�̊֌W��R���܂ł����ݒ�ł��Ȃ��B
// �����̕\���ɉ��o��ǉ��\��
// 
// �� ����
// ������texture,pos,size�͂��̃N���X��cpp�̃w�b�_�������邱�ƁB
//
//=============================================================================
#ifndef _CTIMEMANAGER_H_
#define _CTIMEMANAGER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../RENDERER/CRenderer.h"

//=============================================================================
// �O���錾
//=============================================================================
class CSceneScore;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTimeManager
{
	// ���J�����o
public:
	CTimeManager(LPDIRECT3DDEVICE9* pDevice, short nGameTime);
	~CTimeManager(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	// �c�艽�b���Q�b�^�[
	int GetRemaining(){
		return m_nGameTime - m_nGameTimer;
	}


	// ����J�����o
private:
	// �Q�[���̐�������
	short m_nGameTime;
	// �Q�[���^�C�}�̃J�E���g
	int m_nGameTimer;
	// ���ԃJ�E���g
	unsigned int m_nTimeCur;
	unsigned int m_TimeLast;
	// �Q�[���^�C�}�̐���
	CSceneScore *m_pTimeNumber;
};

#endif
//----EOF----