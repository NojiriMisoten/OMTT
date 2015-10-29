//=============================================================================
//
// CPlayerManagerクラス [CPlayerManager.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CPLAYERMANAGER_H_
#define _CPLAYERMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../SKINMESH/CSkinMesh.h"
#include "CPlayer.h"
//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayerManager
{
public:
	// コンストラクタ
	CPlayerManager(CManager* pManager);

	// デストラクタ
	~CPlayerManager(void);

	// 作成
	void CreatePlayer(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type);

	// 終了
	void Uninit(void);

	D3DXVECTOR3& GetPlayerPos(int ID);

	int GetPlayerHP(int ID);

	CPlayer::PLAYER_ANIM_TYPE GetPlayerState(int ID);

private:
	static const int PLAYER_NUM = 2;

	CPlayer*	m_pPlayer[PLAYER_NUM];
	CManager*	m_pManager;
};

#endif
//----EOF----