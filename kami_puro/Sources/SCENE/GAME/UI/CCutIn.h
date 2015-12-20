//=============================================================================
//
// CCutInクラス [CCutIn.h]
// Author : 塚本俊彦
//
// Start関数でカットインが始まる。
// どっちの“プレイヤ”か、どの“カットイン”かを指定してね
//
// カットインの種類はヘッダーで増やしてcppで中身を入れて
//
//=============================================================================
#ifndef _CCUTIN_H_
#define _CCUTIN_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

// いろいろなカットインを定義する
enum CutInType
{
	CUT_IN_JIJII,
	CUT_IN_MAX
};

class CCutIn2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCutIn
{
public:

	CCutIn(LPDIRECT3DDEVICE9 *pDevice);
	~CCutIn(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 高さ、Y座標、左端の座標、右端の座標、デバイス
	static CCutIn *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// カットインをさせる
	// プレイヤーID、カットインのタイプ
	void Start(int ID, CutInType type);

	void SetPos(D3DXVECTOR3& pos);
	void SetHeight(float height);
private:
	// 初期化
	void Init(LPDIRECT3DDEVICE9 *pDevice);

	// 画面内に入ってくる更新
	void In();
	// 表示している更新
	void Wait();
	// 画面外に出ていく更新
	void Out();

	CCutIn2D *m_pCutInPolygon;
	CCutIn2D *m_pCutInPolygonBack;
	// カットイン
	bool m_isIn;
	bool m_isOut;
	bool m_isWait;
	// 移動するときに必要な値
	float m_Pos;
	float m_PosDest;
	// 移動する為の保管するために必要なタイム
	float m_Time;
	// カットインを表示する時間(フレーム)
	int m_IntervalMax;
	int m_IntervalCount;
	// カットアウトするときに色を変える時用
	D3DXCOLOR m_Color;
	// カットインの種類を保存しておく（フェードアウト用）
	CutInType m_CutInType;
	// スクロールのテクスチャ
	UV_INDEX m_BackUV;
	// プレイヤIDを保存しておく
	int m_ID;

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----