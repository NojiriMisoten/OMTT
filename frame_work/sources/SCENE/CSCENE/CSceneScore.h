//=============================================================================
//
// CSceneScoreクラス [CSceneScore.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENESCORE_H_
#define _CSCENESCORE_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene2D.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CNumber;

//*****************************************************************************
// マクロ
//*****************************************************************************
static const int MAX_SCORE_NUM = 3;		// 桁数
static const int MAX_SCORE = 999;	// 最大値

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneScore : public CScene2D
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス
	//======================================================
	CSceneScore(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_UI);

	// デストラクタ
	~CSceneScore();

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ
	//======================================================
	void Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);
	
	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);
	
	//======================================================
	// 加算関数
	// 引数: 足す数
	//======================================================
	static void Add(int addNum);

	//======================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、テクスチャタイプ、桁数
	// 戻り値: 作ったやつのアドレス
	//======================================================
	static CSceneScore* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum);

private:
	// 使ってない初期化関数
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock){return E_FAIL;};
	
	// 頂点反映関数
	static void SetVertexPolygon(void);

	//=================================================
	// 色変更関数
	// 引数: 色
	//=================================================
	static void SetColorPolygon(D3DXCOLOR color);
	
	LPDIRECT3DDEVICE9	*m_pD3DDevice;				// デバイスオブジェクト(描画に必要)
	static float		m_fWidth;					// 幅
	static float		m_fHeight;					// 高さ
	static int			m_nScore;					// スコア
	static CNumber		*m_Number[MAX_SCORE_NUM];	// 数字(桁数分)
};
#endif
//----EOF-----