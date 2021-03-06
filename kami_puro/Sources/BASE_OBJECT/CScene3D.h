//=============================================================================
//
// CScene3Dクラス [CScene3D.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENE3D_H_
#define _CSCENE3D_H_

//=============================================================================
// インクルード
//=============================================================================
#include "CScene.h"

class CManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene3D : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ、オブジェタイプ
	//======================================================
	CScene3D(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_3D);

	// デストラクタ
	virtual ~CScene3D();

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void DrawNormalRender(void);
	
	// ポジションアクセサ
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	void SetPos(D3DXVECTOR3& pos){ m_Pos = pos;};
	void SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};
	
	// ロットアクセサ
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void SetRot(D3DXVECTOR3& rot){m_Rot = rot;};
	void SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};
	
	//=================================================
	// 高さ取得
	// 引数: 知りたい場所、(out)法線
	// 戻り値: 高さ
	//=================================================
	float GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	//=================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、テクスチャタイプ
	// 戻り値: 作ったやつのアドレス
	//=================================================
	static CScene3D* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager);


	// 幅のセッター
	void SetWidth(float width);

	// 座標のADD
	void AddPos(D3DXVECTOR3 &pos);

	// テクスチャのセッター UVのV
	// 第１引数：左頂点のテクスチャ座標V
	// 第２引数：右頂点のテクスチャ座標V
	void SetTextureV(float v0, float v1);

private:
	// 頂点反映関数
	void SetVertexPolygon(void);

	//=================================================
	// 色変更関数
	// 引数: 色
	//=================================================
	void SetColorPolygon(D3DXCOLOR color);


protected:
	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ、(横分割数、縦分割数)
	// 戻り値: 成否
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager, int wblock = 1, int hblock = 1);

	D3DXVECTOR3				m_vScl;					// 地面の大きさ（スケール）
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// テクスチャ情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	static D3DXMATRIX		m_mtxWorld;				// ワールドマトリックス
	float					m_fWidth;				// ポリゴンの対角線の長さ
	float					m_fHeight;				// ポリゴンの対角線の角度
	CManager				*m_pManager;
};
#endif
//----EOF----