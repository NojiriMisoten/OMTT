//=============================================================================
//
//	CCommandNameヘッダー[CComandName.h]
//
//	Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CCOMMANDNAME_H__
#define __CCOMMANDNAME_H__

//-----------------------------------------------------------------------------
//	インクルード
//-----------------------------------------------------------------------------
#include "../../../BASE_OBJECT/CScene2D.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
static const float COMMAND_NAME_POLYGON_WIDTH = 100.0f * (SCREEN_HEIGHT * 0.5f / 150.f);			// コマンドのポリゴンの横幅
static const float COMMAND_NAME_POLYGON_HEIGHT = 20.0f * (SCREEN_HEIGHT * 0.5f / 150.f);			// コマンドのポリゴンの高さ

//-----------------------------------------------------------------------------
// クラス定義
//-----------------------------------------------------------------------------
class CCommandName : public CScene2D
{
public:
	// コンストラクタ
	CCommandName(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType = OBJTYPE_2D);

	// デストラクタ
	~CCommandName();

	// 生成処理
	// 引数：デバイス、座標、テクスチャ
	static CCommandName* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, TEXTURE_TYPE Texture);

	// 初期化
	// 引数：ボタンのタイプ、座標
	void Init(D3DXVECTOR3 pos, TEXTURE_TYPE Texture);

	// 更新
	void Update(void);

	// 終了
	void Uninit(void);

	// 描画
	void Draw(void);

	// 移動させるための目標座標の設定
	// 引数：目標の座標
	void SetDestPos(D3DXVECTOR3 DestPos){ m_DestPos = DestPos; m_isMoveX = true; }

	// 移動用関数
	void Move(void);

private:
	// 色
	D3DXCOLOR m_Color;
	// 最終的な表示座標
	D3DXVECTOR3 m_DestPos;
	// x方向の移動を行うかどうかのフラグ
	bool m_isMoveX;
};

#endif

// EOF