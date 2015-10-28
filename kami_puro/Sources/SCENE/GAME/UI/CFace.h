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

	// 表情
	enum Expression{
		// 良い表情
		EXPRESSION_GOOD,
		// ふつうな表情
		EXPRESSION_NORAML,
		// ダメな表情
		EXPRESSION_BAD,
		EXPRESSION_MAX
	};

	CFace(LPDIRECT3DDEVICE9 *pDevice);
	~CFace(void);

	void Update();
	void Uninit();
	void Draw();

	// 作成
	static CFace *Create(
		D3DXVECTOR2 &posLeft,
		D3DXVECTOR2 &posRight,
		D3DXVECTOR2 &size,
		LPDIRECT3DDEVICE9 *pDevice);

	// 開始アニメーションをする　引数↓
	// 終了するまでのカウント(何フレームアニメーションするか)
	void StartAnimation(int endCount);

	void SetExpressionLeft();

private:

	struct FaceBace{
		// 座標
		D3DXVECTOR2 m_Pos;
		// 顔の2D
		CScene2D *m_pFace2D;
		// 顔の背景の2D
		CScene2D *m_pBack2D;
		// 表情
		Expression m_Expression;
	};

	// 初期化
	void Init(
		D3DXVECTOR2 &posLeft,
		D3DXVECTOR2 &posRight,
		D3DXVECTOR2 &size);
	// 開始アニメーションをする更新
	void UpdateAnime();

	FaceBace m_FaceLeft;
	FaceBace m_FaceRight;

	// 表情
	Expression m_ExpressionLeft;
	Expression m_ExpressionRight;

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