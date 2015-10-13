//=============================================================================
//
// CJudgeManager�N���X [CJudgeManager.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CJUDGEMANAGER_H_
#define _CJUDGEMANAGER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CJudge;
class CPlayerManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CJudgeManager
{
public:
	// �R���X�g���N�^
	CJudgeManager(void);

	// �f�X�g���N�^
	~CJudgeManager(void);

	// �N���G�C�g
	static CJudgeManager* Create(CPlayerManager* playerManager);

	// �I��
	void Uninit(void);

	// �X�V
	void Update(void);

	CPlayerManager* GetPlayerManager(void){ return m_pPlayerManager; };

private:
	// ������
	void Init(CPlayerManager* playerManager);

	CJudge* m_pJudge;	//	judge�N���X
	CPlayerManager* m_pPlayerManager;
};

#endif
//----EOF----