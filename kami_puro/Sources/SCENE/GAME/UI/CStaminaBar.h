//=============================================================================
//
// CStaminaBarクラス [CStaminaBar.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CSTAMINABAR_H_
#define _CSTAMINABAR_H_
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
class CStaminaBar : public CScene2D
{
public:
	// バーの座標を固定して動かすか
	enum POSITIONBASE{
		// 右側を固定
		POSITIONBASE_RIGHT,
		// 左側を固定
		POSITIONBASE_LEFT,
	};

	CStaminaBar(LPDIRECT3DDEVICE9 *pDevice);
	~CStaminaBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 作成
	// 左端座標,右端座標,バーの変動時にどちら側を固定するか
	static CStaminaBar *Create(
		D3DXVECTOR2 &leftPos,
		D3DXVECTOR2 &rightPos,
		POSITIONBASE positionBase,
		LPDIRECT3DDEVICE9 *pDevice);

	// スタミナを使う
	// 使う量をセットする スタミナが足りなかった場合false
	bool UseStamina(float value);

private:
	// 初期化
	void Init(
		D3DXVECTOR2 &leftPos,
		D3DXVECTOR2 &rightPos,
		POSITIONBASE positionBase);

	// スタミナの自動回復
	void AutoHeal(float value);

	// バーの端の座標
	D3DXVECTOR2 m_PosLeft;
	D3DXVECTOR2 m_PosRight;
	// バーのどちらを固定して動かすか
	POSITIONBASE m_PositionBase;
	// 現在のバーの値（スタミナ）
	float m_Value;
	// 現在のバーの値（スタミナ）最大値
	float m_ValueMax;
	// スタミナの値（m_Value）１当たりのピクセル数
	float m_WidthOneValue;
};

#endif
//----EOF----