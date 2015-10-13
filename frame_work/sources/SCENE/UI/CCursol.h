//=============================================================================
//
// CCursolクラス [CCursol.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CCURSOL_H_
#define _CCURSOL_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../TEXTURE/CTexture.h"
#include "../CSCENE/CScene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCursol : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス
	//======================================================
	CCursol(LPDIRECT3DDEVICE9 *pDevice);

	// デストラクタ
	~CCursol();

	// 終了関数
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 描画フラグセッター
	void SetDrawFlag(bool flag){ m_bDraw = flag; };

	//==================================================================
	// 大きさと場所変更関数
	// 引数: 座標、幅、高さ
	//==================================================================
	void SetVertexPolygon(D3DXVECTOR3 pos, float width, float height);

	//==================================================================
	// クリエイト関数関数
	// 引数: デバイス、座標、幅、高さ、テクスチャー、左右の距離
	// 戻り値: ポインタ
	//==================================================================
	CCursol* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, float lengthOfLeftAndRight);

	// ポジションアクセサ
	D3DXVECTOR3 GetPos(void){ return m_vPos;};
	void		SetPos(D3DXVECTOR3 pos);
	void		SetPos(float x,float y, float z);

	// ロットアクセサ
	D3DXVECTOR3 GetRot(void){return m_vRot;};
	void		SetRot(D3DXVECTOR3 rot){m_vRot = rot;};
	void		SetRot(float x,float y,float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z;};

private:
	static const int MENU_CURSOL_OBJ_NUM = 2; // 左右両方に１つづつ出すので２

	// 使っていない仮想関数
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1){return E_FAIL;};
	
	//==================================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャの種類、左右の距離、(横分割数、縦分割数)
	// 戻り値: 成否
	//==================================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, float lengthOfLeftAndRight, int wblock = 1, int hblock = 1);

	//=============================================
	// 色セット関数
	// 引数: 色
	//=============================================
	void SetColorPolygon(D3DXCOLOR color);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;								// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff[MENU_CURSOL_OBJ_NUM];			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff[MENU_CURSOL_OBJ_NUM];			// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_vPos;										// ポリゴンの中央の位置
	D3DXVECTOR3				m_vRot;										// ポリゴンの回転
	float					m_fWidth;									// 幅
	float					m_fHeight;									// 高さ
	float					m_fAlpha;									// アルファ値
	float					m_fLengthOfLeftToRight;						// 左右の離す長さ
	float					m_fMoveAlphaSign;							// アルファ変更の符号
	bool					m_bDraw;									// 描画フラグ
};
#endif
//----EOF----