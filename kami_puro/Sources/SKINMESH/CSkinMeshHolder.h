//=============================================================================
//
// スキンメッシュ処理 [CSkinMeshHolder.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSKINMESHHOLDER_H_
#define _CSKINMESHHOLDER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "CSkinMesh.h"
#include "../SCENE/GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CSkinMesh;

//*****************************************************************************
// 列挙体宣言
//*****************************************************************************
typedef enum
{
	SKIN_MESH_INFO_PLAYER_1 = 0,
	SKIN_MESH_INFO_PLAYER_2,
	SKIN_MESH_INFO_MAX
}SKIN_MESH_INFO;

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CSkinMeshHolder
{
public:
	// コンストラクタ
	CSkinMeshHolder(void);

	// デストラクタ
	~CSkinMeshHolder(void);

	// 終了
	static void Uninit(void);

	// クリエイト
	static void Create(LPDIRECT3DDEVICE9 *pDevice);

	// スキンメッシュ情報ゲット
	static CSkinMesh* GetSkinMesh(SKIN_MESH_INFO type){ return m_pSkinMeshHolder[type]; };

private:
	// コールバックタイミングの初期化
	static void InitCallBackTiming(void);

	static CSkinMesh				*m_pSkinMeshHolder[SKIN_MESH_INFO_MAX];
	static CCallBackHandlerPlayer	*m_pFunc[SKIN_MESH_INFO_MAX];			// アニメーションコールバック用
	static CALLBACK_TIMING			m_pCallBackTimiming[CPlayer::PLAYER_ANIM_MAX];	// コールバックのタイミング
};

#endif
//----EOF----