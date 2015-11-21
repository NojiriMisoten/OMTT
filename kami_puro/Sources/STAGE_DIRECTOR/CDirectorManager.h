//=============================================================================
//
// CDirectorManager�N���X [CDirectorManager.h]
// Author : �r���@���
//
//=============================================================================
#ifndef _CDIRECTORMANAGER_H_
#define _CDIRECTORMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "CDirector.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
class CDirector;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CDirectorManager
{
public:
	// �R���X�g���N�^
	CDirectorManager( CManager* pManager );

	// �f�X�g���N�^
	~CDirectorManager( void );

	// ������
	void Init( void );

	// �I��
	void Uninit( void );

	// �X�V
	void Update( void );

	// �f�B���N�^�[�ɍĐ����鉉�o�������I�ɑ���		�g�������Ȃ�
	void SendDirector( DIRECT_ID directingID, PLAYER_ID playerID ) { m_pDirector->SendDirector( directingID, playerID ); };

	// �f�B���N�^�[�ɍĐ����鉉�o�𑗂�
	void Direct( DIRECT_ID cueDirectingID, PLAYER_ID cuePlayerID ) { m_pDirector->Direct( cueDirectingID, cuePlayerID); };

	// ���ݍĐ������ǂ������Q�b�g�A-1�Ȃ�Đ����Ă��Ȃ�
	DIRECT_ID GetIsDirecting( void ) { return m_pDirector->GetIsDirecting(); };

	// ���ݍĐ������ǂ������Z�b�g�A-1�Ȃ�Đ����Ă��Ȃ�
	void SetIsDirecting( DIRECT_ID directingID ) { m_pDirector->SetIsDirecting( directingID ); };

	// �Đ��I��
	void SetEndDirecting( void ) { m_pDirector->SetEndDirecting(); };

private:
	CManager*	m_pManager;
	CDirector*	m_pDirector;
};

#endif
//----EOF----