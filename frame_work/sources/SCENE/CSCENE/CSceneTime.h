//=============================================================================
//
// CSceneTimeクラス [CSceneTime.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENETIME_H_
#define _CSCENETIME_H_

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
static const int MAX_TIME_NUM = 2;		// 桁数
static const int MAX_TIME = 99;			// 最大値

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneTime : public CScene2D
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス
	//======================================================
	CSceneTime(LPDIRECT3DDEVICE9 *pDevice);

	// デストラクタ
	~CSceneTime();

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ、桁数
	//======================================================
	void Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum);
	
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
	void Add(int addTime);

	//======================================================
	// 時間ゲット関数
	// 戻り値: 時間
	//======================================================
	int GetTime(void){return m_nTime;};

	//======================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、テクスチャタイプ、桁数
	// 戻り値: 作ったやつのアドレス
	//======================================================
	static CSceneTime* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int numberNum);

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
	float				m_fWidth;					// 幅
	float				m_fHeight;					// 高さ
	int					m_nNumberNum;				// 桁数
	int					m_nTime;					// タイム
	int					m_nCounter;					// カウンター
	CNumber				*m_Number[MAX_TIME_NUM];	// 数字(桁数分)
};
#endif
//----EOF----