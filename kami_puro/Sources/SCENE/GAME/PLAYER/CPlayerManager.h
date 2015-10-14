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

#define PLAYER_NUM (2)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
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

	static D3DXVECTOR3 GetPlayerPos(int num);

	static int GetPlayerID(int num);

	static int GetPlayerHP(int num);

	static int GetPlayerState(int num);

private:
	static CPlayer*	m_pPlayer[PLAYER_NUM];
	static CManager*	m_pManager;
};

#endif
//----EOF----