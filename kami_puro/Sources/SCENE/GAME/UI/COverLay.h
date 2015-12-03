//=============================================================================
//
// COverLayクラス [COverLay.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _COVERLAY_H_
#define _COVERLAY_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"
#include "../../../BASE_OBJECT/CScene2D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class COverLay
{
public:
	// オーバーレイをStartするのに必要なデータ
	struct Data{
		TEXTURE_TYPE texture;
		float fadeInSpeed;
		int interval;
		float fadeOutSpeed;

		Data() {};
		Data(
			TEXTURE_TYPE tex,
			float fadeInSpeed,
			int interval,
			float fadeOutSpeed)
		{
			this->texture = tex;
			this->fadeInSpeed = fadeInSpeed;
			this->interval = interval;
			this->fadeOutSpeed = fadeOutSpeed;
		}
	};

	COverLay(LPDIRECT3DDEVICE9 *pDevice);
	~COverLay(void);

	void Update();
	void Uninit();
	void DrawUI();

	// デバイス
	static COverLay *Create(
		LPDIRECT3DDEVICE9 *pDevice);

	// 第１引数 テクスチャ
	// 第２引数 フェードインの速さ
	// 第３引数 フェードインしてから表示しておく長さ（フレーム）
	// 第４引数 フェードアウトの速さ
	void Start(Data *data);

private:
	// 初期化
	void Init(LPDIRECT3DDEVICE9 *pDevice);

	// 画面内に入ってくる更新
	void In();
	// 表示している更新
	void Wait();
	// 画面外に出ていく更新
	void Out();
	
	// ポリゴン
	CScene2D *m_p2D;
	// カラー
	D3DXCOLOR m_Color;

	// カットイン
	bool m_isIn;
	bool m_isOut;
	bool m_isWait;

	// フェードインの速さ
	float m_FadeInSpeed;
	// フェードアウトの速さ
	float m_FadeOutSpeed;
	// 待ち時間
	int m_IntervalMax;
	// 待ち時間カウント
	int m_IntervalCount;

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----