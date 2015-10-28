//=============================================================================
//
// CDirector�N���X [CDirector.h]
// Author : �r���@���
//
//=============================================================================
#ifndef _CDIRECTOR_H_
#define _CDIRECTOR_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �A�j���[�V����
typedef enum
{
	ATTACK_SMALL_CHOP = 0,
	ATTACK_SMALL_ELBOW,
	ATTACK_SMALL_LARIAT,
	ATTACK_BIG_ROLLING,
	ATTACK_BIG_SHOLDER,
	ATTACK_BIG_DROPKICK,
	ATTACK_THROW_SLAP,
	ATTACK_THROW_BACKDROP,
	ATTACK_THROW_STUNNER,
	ATTACK_FINISH_BODYPRESS,
	ATTACK_FINSH_ATOMICBUSTER,
	ATTACK_MAX
}ATTACK_LIST;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CDirector
{
public:
	// �R���X�g���N�^
	CDirector( void );

	// �f�X�g���N�^
	~CDirector( void );

	// ������
	void Init( void );

	// �I��
	void Uninit( void );

	// �X�V
	void Update( void );
	
	// �f�B���N�^�[�ɍĐ����鉉�o�𑗂�
	void SendDirector( ATTACK_LIST attackID, int playerID );
	
	// ���ݍĐ������ǂ������Q�b�g
	bool GetIsDirecting( void ) { return m_IsDirecting; };

private:
	bool			m_IsDirecting;			// ���ݍĐ������ǂ���
	ATTACK_LIST		m_CurAttackID;			// ���ݍĐ�����AttackID
	int				m_CurPlayerID;			// ���ݍĐ�����PlayerID
};

#endif
//----EOF----