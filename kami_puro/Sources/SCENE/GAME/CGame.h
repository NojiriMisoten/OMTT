//=============================================================================
//
// CGame�N���X [CGame.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CGAME_H_
#define _CGAME_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"

//=============================================================================
// �O���錾
//=============================================================================
class CPlayerManager;
class CStaminaBar;
class CCountTime;
class CCrowdBar;
class CHpBar;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CPhase
{
	// ���J�����o
public:
	// �R���X�g���N�^
	CGame(void);

	// �f�X�g���N�^
	~CGame(void);

	//======================================================
	// ������
	// ����: �t�F�[�Y���[�h�A�f�o�C�X
	//======================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	//======================================================
	// �N���G�C�g
	// ����: �t�F�[�Y���[�h�A�f�o�C�X
	// �߂�l: �������̃|�C���^
	//======================================================
	static CGame* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	CManager* GetManager(void){ return m_pManager; };

private:
	CCountTime *m_Timer;
	CStaminaBar *m_StaminaBarL;
	CStaminaBar *m_StaminaBarR;
	CCrowdBar *m_CrowdBar;
	CHpBar *m_HpBarL;
	CHpBar *m_HpBarR;
};

#endif
//----EOF----