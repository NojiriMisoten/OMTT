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
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �A�j���[�V����
typedef enum
{
	DIR_SMALL_CHOP = 0,
	DIR_SMALL_ELBOW,
	DIR_SMALL_LARIAT,
	DIR_BIG_ROLLING,
	DIR_BIG_SHOULDER,
	DIR_BIG_DROPKICK,
	DIR_THROW_SLAP,
	DIR_THROW_BACKDROP,
	DIR_THROW_STUNNER,
	DIR_FINISHER,
	DIR_ROPE,
	DIR_ROPE_LARIAT,
	DIR_ROPE_DROPKICK,
	DIR_ROPE_STUNNER,
	DIR_BATTLE_INTRO,
	DIR_BATTLE_RESULT,
	DIR_MAX
}DIRECT_ID;

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
class CGame;
class CDirectList;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CDirector
{
public:
	typedef struct{
		DIRECT_ID	directingID;
		PLAYER_ID	playerID;
	}DIRECT_PLAYER;

	// �R���X�g���N�^
	CDirector( CManager *pManager, CGame *pGame );

	// �f�X�g���N�^
	~CDirector( void );

	// ������
	void Init( void );

	// �I��
	void Uninit( void );

	// �X�V
	void Update( void );
	
	// �f�B���N�^�[�ɍĐ����鉉�o�������I�ɑ���
	void SendDirector( DIRECT_ID directingID, PLAYER_ID playerID );

	// �f�B���N�^�[�ɍĐ����鉉�o�𑗂�
	int Direct( DIRECT_ID directingID, PLAYER_ID playerID );
	
	// ���ݍĐ������ǂ������Q�b�g�A-1�Ȃ�Đ����Ă��Ȃ�
	DIRECT_PLAYER GetIsDirecting( void ) { return m_CurDirect; };

	// ���ݍĐ������ǂ������Z�b�g�A-1�Ȃ�Đ����Ă��Ȃ�
	DIRECT_PLAYER SetIsDirecting( DIRECT_PLAYER directing ) { m_CurDirect = directing; };

	// �Đ��I��
	void SetEndDirecting( void );

	// �f�t�H���g�̃J�����ݒ�
	void SetDefaultCamera( void );

private:

	CManager		*m_pManager;
	CGame			*m_pGame;
	CDirectList		*m_pDirectList;
	DIRECT_PLAYER	m_CueDirect;			// �Đ��\��
	DIRECT_PLAYER	m_CurDirect;			// �Đ���
};

#endif
//----EOF----