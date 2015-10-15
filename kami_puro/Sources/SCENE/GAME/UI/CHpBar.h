//=============================================================================
//
// CHpBarクラス [CHpBar.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CHPBAR_H_
#define _CHPBAR_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CHpBar : public CScene2D
{
public:
	// バーの座標を固定して動かすか
	enum POSITIONBASE{
		// 右側を固定
		POSITIONBASE_RIGHT,
		// 左側を固定
		POSITIONBASE_LEFT,
	};

	CHpBar(LPDIRECT3DDEVICE9 *pDevice);
	~CHpBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 作成
	// 座標,バーの変動時にどちら側を固定するか
	static CHpBar *Create(
		D3DXVECTOR2 pos,
		float width,
		float height,
		float centerDist,
		POSITIONBASE positionBase,
		LPDIRECT3DDEVICE9 *pDevice);

	void Add(float value);

	// 値を0にしてバーを消す
	// 飾りの２Dとかも伸縮させなきゃ・・・
	void Reset();

private:
	// 初期化
	void Init(D3DXVECTOR2 pos,
		float width,
		float height,
		float centerDist,
		POSITIONBASE positionBase);

	// バーの端の座標
	D3DXVECTOR2 m_Pos;
	// 固定した座標
	D3DXVECTOR2 m_PosBase;
	// バーのどちらを固定して動かすか
	POSITIONBASE m_PositionBase;
	// 現在のバーの値
	float m_Value;
	// 現在のバーの値最大値
	float m_ValueMax;
	// 赤いバーの値（スタミナ）
	float m_ValueRed;
	float m_WidthOneValue;
	
	// 動かすバー
	CScene2D *m_HpBar;

	// ダメージを受けてからのカウント　赤いバーに使う
	int m_RedResetCount;
	// 赤いバーを変更するかフラグ
	bool m_isRedReset;
};

#endif
//----EOF----