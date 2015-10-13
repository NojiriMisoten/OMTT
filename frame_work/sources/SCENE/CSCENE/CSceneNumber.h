//=============================================================================
//
// CSceneNumberクラス [CSceneNumber.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENENUMBER_H_
#define _CSCENENUMBER_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../MAIN/main.h"
#include "../../TEXTURE/CTexture.h"
#include "CScene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneNumber : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス
	//======================================================
	CSceneNumber(LPDIRECT3DDEVICE9 *pDevice, int nPriority = OBJTYPE_SCORE);

	// デストラクタ
	~CSceneNumber();

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、数字、テクスチャータイプ
	//======================================================
	void Init(D3DXVECTOR3 pos, float width, float height, int number, TEXTURE_TYPE texType);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	//======================================================
	// 数字変更関数
	// 引数: 数字
	//======================================================
	void SetNumber(int number);

	//======================================================
	// 描画するか
	// 引数: するかどうか
	//======================================================
	void SetDrawFlag(bool flag){m_bDraw = flag;};

	// ポジションアクセサ
	D3DXVECTOR3 GetPos(void){ return m_vPos;};
	void		SetPos(D3DXVECTOR3 pos);
	void		SetPos(float x,float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z;};

	// ロットアクセサ
	D3DXVECTOR3 GetRot(void){return m_vRot;};
	void		SetRot(D3DXVECTOR3 rot){m_vRot = rot;};
	void		SetRot(float x,float y,float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z;};

	//=================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、数字、テクスチャタイプ
	// 戻り値: 作ったやつのアドレス
	//=================================================
	static CSceneNumber* CSceneNumber::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, int number, TEXTURE_TYPE texType);
private:
	// 使ってない初期化
	HRESULT		Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1){return E_FAIL;};
	
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_vPos;					// ポリゴンの中央の位置
	D3DXVECTOR3				m_vRot;					// ポリゴンの回転
	float					m_fWidth;				// 幅
	float					m_fHeight;				// 高さ
	bool					m_bDraw;				// 描画するか
};

#endif
//----EOF----