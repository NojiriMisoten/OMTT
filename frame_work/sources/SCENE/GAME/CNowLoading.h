//=============================================================================
//
// CNowLoadingクラス [CNowLoading.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _C_NOWLOADING_H_
#define _C_NOWLOADING_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CNowLoading
{
public:
	// コンストラクタ
	CNowLoading(void);

	// デストラクタ
	~CNowLoading(void);

	//======================================================
	// 初期化
	// 引数: デバイス
	//======================================================
	void Init(LPDIRECT3DDEVICE9* pDevice);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

private:
	static const int		MAX_CHAR_NUM = 10;						// 最大文字数

	LPDIRECT3DDEVICE9		*m_pD3DDevice;							// デバイスオブジェクト(描画に必要)
	D3DXVECTOR3				m_vCharPos[MAX_CHAR_NUM];				// 文字の座標
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff[MAX_CHAR_NUM];			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;							// テクスチャ情報を格納する場所のアドレスを確保する場所
	float					m_fMoveCounter[MAX_CHAR_NUM];			// 移動アニメーション管理
};

#endif
//----EOF----