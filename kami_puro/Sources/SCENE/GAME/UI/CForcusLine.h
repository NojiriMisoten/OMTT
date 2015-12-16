//=============================================================================
//
// CForcusLineクラス [CForcusLine.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _FORCUS_LINE_H_
#define _FORCUS_LINE_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CForcusLine
{
public:
	CForcusLine(LPDIRECT3DDEVICE9 *pDevice);
	~CForcusLine(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 高さ、Y座標、左端の座標、右端の座標、デバイス
	static CForcusLine *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// 集中線を出す
	// 第1引数：表示するフレーム数
	// 第2引数：trueなら派手なテクスチャになる falseなら白黒
	void Start(int interval, bool isFinish);

private:
	// 初期化
	void Init(LPDIRECT3DDEVICE9 *pDevice);

	// 画面内に入ってくる更新
	void In();
	// 表示している更新
	void Wait();
	// 画面外に出ていく更新
	void Out();

	CScene2D *m_pPolygon;
	CScene2D *m_pBack;
	// カットイン
	bool m_isIn;
	bool m_isOut;
	bool m_isWait;
	// 移動するときに必要な値
	float m_Pos;
	// カットインを表示する時間(フレーム)
	int m_IntervalMax;
	int m_IntervalCount;
	// カットアウトするときに色を変える時用
	D3DXCOLOR m_Color;
	D3DXCOLOR m_ColorBack;
	// スクロールのテクスチャ
	UV_INDEX m_BackUV;
	// テクスチャを変更するカウント
	int m_TextureCount;
	
	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----