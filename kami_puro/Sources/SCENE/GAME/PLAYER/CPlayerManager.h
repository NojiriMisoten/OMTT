//=============================================================================
//
// CPlayerManager�N���X [CPlayerManager.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CPLAYERMANAGER_H_
#define _CPLAYERMANAGER_H_
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../SKINMESH/CSkinMesh.h"
#include "CPlayer.h"
//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayerManager
{
public:
	// �R���X�g���N�^
	CPlayerManager(CManager* pManager);

	// �f�X�g���N�^
	~CPlayerManager(void);

	// �쐬
	static void CreatePlayer(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type);

	// �I��
	static void Uninit(void);

	static D3DXVECTOR3& GetPlayerPos(int ID);

	static int GetPlayerHP(int ID);

	static CPlayer::PLAYER_ANIM_TYPE GetPlayerState(int ID);

private:
	static const int PLAYER_NUM = 2;
	static CPlayer*	m_pPlayer[PLAYER_NUM];
	static CManager*	m_pManager;
};

#endif
//----EOF----