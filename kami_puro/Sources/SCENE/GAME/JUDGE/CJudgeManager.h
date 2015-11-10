//=============================================================================
//
// CJudgeManager�N���X [CJudgeManager.h]
// Author : �r���@���
//
//=============================================================================
#ifndef _CJUDGEMANAGER_H_
#define _CJUDGEMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "CJudge.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
class CJudge;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CJudgeManager
{
public:
	// �R���X�g���N�^
	CJudgeManager( void );

	// �f�X�g���N�^
	~CJudgeManager( void );

	// ������
	void Init( CManager* pManager );

	// �I��
	void Uninit( void );

	// �X�V
	void Update( void );

	// �퓬���[�h���Z�b�g
	void SetBattleMode( BATTLE_MODE battleMode ) { m_pJudge->SetBattleMode( battleMode ); };

	// �퓬���[�h���Q�b�g
	BATTLE_MODE GetBattleMode( void ) { return m_pJudge->GetBattleMode(); };

private:
	CJudge* m_pJudge;
};

#endif
//----EOF----