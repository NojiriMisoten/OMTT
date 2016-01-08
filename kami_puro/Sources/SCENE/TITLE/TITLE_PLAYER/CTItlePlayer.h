//=============================================================================
//
// CTitlePlayerクラス [CTitlePlayer.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CTITLE_PLAYER_H_
#define _CTITLE_PLAYER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../BASE_OBJECT/CSceneX.h"
#include "../../../SKINMESH/CSkinMesh.h"
#include "../../GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitlePlayer : public CPlayer
{
public:
	typedef enum
	{
		DRAW_MODE_GRAY = 0,
		DRAW_MODE_HALF,
		DRAW_MODE_NORMAL,
		DRAW_MODE_MAX,
	}DRAW_MODE;
	// コンストラクタ
	CTitlePlayer(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_X);

	// デストラクタ
	~CTitlePlayer(void);

	// 作成
	static CTitlePlayer* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void DrawNormalRender(void);

	// スキンメッシュで計算したワールドマトリクスをもとにセットする
	void SetWorldMtx(D3DXMATRIX* worldMtx, PLAYER_RENDERER_TYPE type, int blendBoneNum);
	void SetWorldMtxForNormalRender(D3DXMATRIX* worldMtx, int blendBoneNum);

	void SetDrawMode(DRAW_MODE mode){ m_DrawMode = mode; };
private:
	// 初期化
	void Init(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, D3DXVECTOR3& rot, SKIN_MESH_ANIM_MODEL type, CManager* pManager, int ID);

	int m_DrawMode;
};

#endif
//----EOF----