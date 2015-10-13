//=============================================================================
//
// CSceneAnimeクラス [CSceneAnime.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENEANIME_H_
#define _CSCENEANIME_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../MAIN/main.h"
#include "../CSCENE/CScene.h"
#include "../CSCENE/CScene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const short	INFINIT_LOOP = -1;					// 無限ループを示す


//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneAnime : public CScene2D
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ
	//======================================================
	CSceneAnime(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_EFFECT, OBJTYPE objType = OBJTYPE_NONE);

	// デストラクタ
	~CSceneAnime();

	//======================================================
	// 初期化　ループなし
	// 引数: 座標、幅、高さ、テクスチャータイプ、テクスチャ横分割数、テクスチャ縦分割数、終了時間
	// 戻り値: 成否
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int endTime);

	//======================================================
	// 初期化　ループなし
	// 引数: 座標、幅、高さ、テクスチャータイプ、テクスチャ横分割数、テクスチャ縦分割数、ループ回数（-1で無限）
	// 戻り値: 成否
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int animSpd, int loopNum);
	
	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// アニメスピードのセッター
	void SetAnimeSpeed(int value){
		m_nChangeTime = value;
	}

	// テクスチャのセッタ
	void SetTexture(TEXTURE_TYPE tex, int x, int y){
		m_pD3DTexBuff = CTexture::GetTexture(tex);
		m_nSepalateNumX = x;
		m_nSepalateNumY = y;
	}

	//======================================================
	// アニメーション番号設定
	// 引数: アニメーション番号
	//======================================================
	void SetIndex(int nIdx);

	//======================================================
	// アニメーション番号設定
	// 引数: アニメーション番号,反転するか
	//======================================================
	void SetIndex(int nIdx, bool reverse);

	//======================================================
	// 自動更新するか
	// 引数: フラグ
	//======================================================
	void SetAutoUpdate(bool flag){m_bAutoUpdate = flag;};

	//======================================================
	// 描画するか
	// 引数: フラグ
	//======================================================
	void SetDrawFlag(bool flag){m_bDraw = flag;};

	//======================================================
	// クリエイト関数　ループなし
	// 引数: デバイス、座標、幅、高さ、テクスチャータイプ、テクスチャ横分割数、テクスチャ縦分割数、終了時間
	// 戻り値: 作ったやつのアドレス
	//======================================================
	static CSceneAnime* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY, int endTime);
	
	//======================================================
	// クリエイト関数　ループあり
	// 引数: デバイス、座標、幅、高さ、テクスチャータイプ、テクスチャ横分割数、テクスチャ縦分割数、ループ回数（-1で無限）
	// 戻り値: 作ったやつのアドレス
	//======================================================
	static CSceneAnime* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX, int sepalateY, int animSpd, int loopNum, int priority = TYPE_PRIORITY_EFFECT);

protected:
	// ループなし更新
	void Update_Normal(void);

	// ループあり更新
	void Update_Loop(void);
	
	LPDIRECT3DDEVICE9	*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	float				m_fLeft;				// UV値(左)
	float				m_fRight;				// UV値(右)
	float				m_fTop;					// UV値(上)
	float				m_fBottom;				// UV値(下)
	int					m_nWidth;				// 横幅
	int					m_nHeight;				// 縦幅
	int					m_nSepalateNumX;		// 画像分割数(横)・・・ex:横に５個並んでるなら５
	int					m_nSepalateNumY;		// 画像分割数(縦)
	int					m_nIdx;					// 分割番号
	int					m_nDoAnimCounter;		// アニメーション時間管理用
	int					m_nEndTime;				// 終了時間
	int					m_nMaxLoopNum;			// 最大ループ回数
	int					m_nCurLoopNum;			// ループ回数
	int					m_nChangeTime;			// 切り替え時間
	int					m_nMaxIdx;				// idxの最大値
	bool				m_bAutoUpdate;			// 自動更新するか
	bool				m_bDraw;				// 描画するか
};
#endif
//----EOF----