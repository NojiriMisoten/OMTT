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

	static D3DXVECTOR3 GetPlayerPos(void);

private:
	static CPlayer*	m_pPlayer;
	static CManager*	m_pManager;
};

#endif
//----EOF----