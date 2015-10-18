//=============================================================================
//
// CStaminaBarクラス [CStaminaBar.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CCROWNDBAR_H_
#define _CCROWNDBAR_H_
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
class CCrowdBar : public CScene2D
{
public:

	CCrowdBar(LPDIRECT3DDEVICE9 *pDevice);
	~CCrowdBar(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 作成
	static CCrowdBar *Create(
		D3DXVECTOR2 &pos,
		float height,
		LPDIRECT3DDEVICE9 *pDevice);

	void Add(float value);

	// バーの頂点を真ん中に集める
	void Reset();

	// バーの頂点を端っこに戻す
	void Replace(float ratio);

private:
	// 初期化
	void Init(D3DXVECTOR2 &pos, float height);

	D3DXVECTOR2 m_Pos;

	// 現在のバーの値
	float m_Value;
	// 現在のバーの値最大値
	float m_ValueMax;
	// バーをアニメーションでベース頂点を変える時用の値
	float mValueBase;
	// バーの値（m_Value）１当たりのピクセル数
	float m_WidthOneValue;

	// 二つのバー
	CScene2D *m_pBarLeft;
	CScene2D *m_pBarRight;
};

#endif
//----EOF----