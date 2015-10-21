//=============================================================================
//
// CFaceクラス [CFace.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CFACE_H_
#define _CFACE_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../RENDERER/CRenderer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFace
{
public:

	CFace(LPDIRECT3DDEVICE9 *pDevice);
	~CFace(void);

	void Update();
	void Uninit();
	void Draw();

	// 作成
	static CFace *Create(
		D3DXVECTOR2 &posLeft,
		D3DXVECTOR2 &sizeLeft,
		D3DXVECTOR2 &posRight,
		D3DXVECTOR2 &sizeRight,
		LPDIRECT3DDEVICE9 *pDevice);

	// 開始アニメーションをする　引数↓
	// 終了するまでのカウント(何フレームアニメーションするか)
	void StartAnimation(int endCount);

private:
	// 初期化
	void Init(
		D3DXVECTOR2 &posLeft,
		D3DXVECTOR2 &sizeLeft,
		D3DXVECTOR2 &posRight,
		D3DXVECTOR2 &sizeRight);
	// 開始アニメーションをする更新
	void UpdateAnime();

	// 座標
	D3DXVECTOR2 m_PosLeft;
	D3DXVECTOR2 m_PosRight;
	// 左側の顔
	CScene2D *m_pFaceLeft;
	// 左側の顔の枠
	CScene2D *m_pBackLeft;
	// 右側の顔
	CScene2D *m_pFaceRight;
	// 右側の顔の枠
	CScene2D *m_pBackRight;

	// 開始アニメをするためのカウント
	int m_AnimeCount;
	// 開始アニメの終わり　フレーム数
	int m_AnimeCountMax;
	// 開始アニメしているフラグ
	bool m_isAnime;
	// 開始アニメ1フレームで変更するアルファ値
	float m_AnimeOneFrameAlpha;
	D3DXCOLOR m_Anime2DColor;

	// デバイス
	LPDIRECT3DDEVICE9 *m_pD3DDevice;
};

#endif
//----EOF----