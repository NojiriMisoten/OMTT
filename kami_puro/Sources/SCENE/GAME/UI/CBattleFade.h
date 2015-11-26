//=============================================================================
//
// CBattleFadeクラス [CBattleFade.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CBATTLEADE_H_
#define _CBATTLEADE_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "CBattleFade2D.h"

// いろいろなフェード
enum BattleFade
{
	BATTLE_FADE_LIGHT,
	BATTLE_FADE_MAX
};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBattleFade
{
public:

	CBattleFade(LPDIRECT3DDEVICE9 *pDevice);
	~CBattleFade(void);

	void Update();
	void Uninit();
	void DrawUI();

	// 高さ、Y座標、左端の座標、右端の座標、デバイス
	static CBattleFade *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// タイプを指定してフェードをさせる
	void Start(BattleFade type);

private:
	// 初期化
	void Init(LPDIRECT3DDEVICE9 *pDevice);

	// 画面内に入ってくる更新
	void In();
	// 表示している更新
	void Wait();
	// 画面外に出ていく更新
	void Out();

	// 背景
	CScene2D *m_pLightBack;
	D3DXCOLOR m_ColorBack;

	// 光のフェードに必要な2D
	struct LightData{
		CBattleFade2D *m_pLightLine;
		CBattleFade2D *m_pLightBall;
		CBattleFade2D *m_pLightBallSmall;
		D3DXCOLOR m_ColorLine;
		D3DXCOLOR m_ColorBall;
		D3DXCOLOR m_ColorBallSmall;
		D3DXVECTOR3 m_Pos;

		// ２Dにそれぞれの色をセットする
		void SetColorPolygon()
		{
			m_pLightBall->SetColorPolygon(m_ColorBall);
			m_pLightLine->SetColorPolygon(m_ColorLine);
			m_pLightBallSmall->SetColorPolygon(m_ColorBallSmall);
			m_pLightBall->SetColorPolygon(m_ColorBall);
			m_pLightLine->SetColorPolygon(m_ColorLine);
			m_pLightBallSmall->SetColorPolygon(m_ColorBallSmall);
		}
		// すべてのカラーのアルファを指定した数値にする
		void SetAlpha(float a)
		{
			m_ColorBall.a = a;
			m_ColorLine.a = a;
			m_ColorBallSmall.a = a;
			m_ColorBall.a = a;
			m_ColorLine.a = a;
			m_ColorBallSmall.a = a;
			SetColorPolygon();
		}
		// すべてのカラーのアルファを指定した数値で加算
		void AddAlpha(float a)
		{
			m_ColorBall.a += a;
			m_ColorLine.a += a;
			m_ColorBallSmall.a += a;
			m_ColorBall.a += a;
			m_ColorLine.a += a;
			m_ColorBallSmall.a += a;
			SetColorPolygon();
		}
		// それぞれの座標にセット
		void SetPosPolygon()
		{
			m_Pos.y += 8;
			m_pLightBall->SetPos(m_Pos);
			m_Pos.y -= 8;
			m_pLightBallSmall->SetPos(m_Pos);
		}
	};

	LightData m_LightA;
	LightData m_LightB;

	// 更新を分けるためのフラグ
	bool m_isIn;
	bool m_isOut;
	bool m_isWait;
	// 移動する為の保管するために必要なタイム
	float m_Time;
	// カウント
	int m_Count;
	// InとOutの間の待つ時間　Startで指定する
	int m_WaitInterval;
	// カットインの種類を保存しておく（フェードアウト用）
	BattleFade m_BattleFadeType;

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----