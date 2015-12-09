//=============================================================================
//
// CDirectList�N���X [CDirectList.h]
// Author : �r���@���
//
//=============================================================================
#ifndef _CDIRECTLIST_H_
#define _CDIRECTLIST_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"
#include "../MANAGER/CManager.h"
#include "CDirector.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �O����`
//*****************************************************************************
class CDirect;
class CDirectChop;
class CDirectElbow;
class CDirectLariat;
class CDirectRolling;
class CDirectShoulder;
class CDirectDropkick;
class CDirectSlap;
class CDirectBackdrop;
class CDirectStunner;
class CDirectFinisher;
class CDirectBattleIntro;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CDirectList
{
public:
	// �R���X�g���N�^
	CDirectList( CManager *pManager );

	// �f�X�g���N�^
	~CDirectList( void );

	// ������
	void Init( void );

	// �I��
	void Uninit( void );

	// �I��
	void Update( void );

	// �f�B���N�g�h�c����|�C���^���擾
	CDirect *GetDirectList( DIRECT_ID dirID ) { return m_pDirectList[dirID]; };

private:
	CManager		*m_pManager;

	// ���o�̔z��
	CDirect			*m_pDirectList[DIR_MAX];

	/*�����ɂ����ς�*/
	CDirectChop			*m_pDirectChop;
	CDirectElbow		*m_pDirectElbow;
	CDirectLariat		*m_pDirectLariat;
	CDirectRolling		*m_pDirectRolling;
	CDirectShoulder		*m_pDirectShoulder;
	CDirectDropkick		*m_pDirectDropkick;
	CDirectSlap			*m_pDirectSlap;
	CDirectBackdrop		*m_pDirectBackdrop;
	CDirectStunner		*m_pDirectStunner;
	CDirectFinisher		*m_pDirectFinisher;
	CDirectBattleIntro	*m_pDirectBattleIntro;
};

#endif
//----EOF----