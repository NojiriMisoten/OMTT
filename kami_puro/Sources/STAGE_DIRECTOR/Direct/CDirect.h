//=============================================================================
//
// CDirect�N���X [CDirect.h]
// Author : �r���@���
//
//=============================================================================
#ifndef _CDIRECT_H_
#define _CDIRECT_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../MANAGER/CManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../../SCENE/GAME/UI/CUiManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
const D3DXVECTOR3 INVERSE_XZ[PLAYER_MAX] = {
	D3DXVECTOR3( +1.0f, +1.0f, +1.0f ),
	D3DXVECTOR3( -1.0f, +1.0f, -1.0f )
};

const float DAMAGE_AMP = 2;			// DEFAULT : 2
const float TENSION_AMP = 8;		// DEFAULT : 8


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CCameraManager;
class CPlayerManager;
class CUIManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CDirect
{
public:
	// �R���X�g���N�^
	CDirect( CManager *pManager, CGame *pGame );

	// �f�X�g���N�^
	~CDirect( void );

	// ������
	virtual void Init( PLAYER_ID playerID ) = 0;

	// �I��
	virtual void Uninit( void ) = 0;

	// �X�V
	virtual void Update( void ) = 0;

	// ���o���I���������`�F�b�N
	void CheckEnd( void );

	void SetPlayerID( PLAYER_ID playerID );

	D3DXVECTOR3 TranslateCoord( PLAYER_ID playerID, D3DXVECTOR3 vec );

protected:
	CManager		*m_pManager;
	CPlayerManager	*m_pPlayerManager;
	CCameraManager	*m_pCameraManager;
	CUiManager		*m_pUIManager;
	CGame			*m_pGame;

	PLAYER_ID m_Player;				// ���C���ƂȂ�v���C���[
	PLAYER_ID m_Enemy;				// ����ƂȂ�v���C���[
	int m_FrameCount;				// ���o�̌��݃t���[��
	int m_TotalFrame;				// ���o�̑��t���[����
};

#endif
//----EOF----