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
	static void CreatePlayer(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, SKIN_MESH_ANIM_MODEL type);

	// 終了
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