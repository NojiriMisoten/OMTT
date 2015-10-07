//=============================================================================
//
// CScene2Dクラス [CScene2D.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENE2D_H_
#define _CSCENE2D_H_

//=============================================================================
// インクルード
//=============================================================================
#include "CScene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene2D : public CScene
{
public:
	//=======================================================================
	// コンストラクタ
	// 引数:デバイス、プライオリティ、オブジェタイプ
	//=======================================================================
	CScene2D(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType = OBJTYPE_2D);

	// デストラクタ
	~CScene2D();

	//=======================================================================
	// 初期化関数
	// 引数:座標、幅、高さ、テクスチャの種類、（以下必要なし）（横分割数、縦分割数）
	// 戻り値:作成したアドレス
	//=======================================================================
	HRESULT Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void DrawUI(void);

	// ポジションアクセサ
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	void		SetPos(D3DXVECTOR3& pos);
	void		SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};

	// ロットアクセサ
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void		SetRot(D3DXVECTOR3& rot);
	void		SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};

	// サイズアクセサ
	float GetWidth(void){return m_fWidth;};
	void SetWidth(float width){m_fWidth = width;};
	float GetHeight(void){return m_fHeight;};
	void SetHeight(float height){m_fHeight = height;};
	
	//=======================================================================
	// サイズと場所変更関数
	// 引数:座標、幅、高さ
	//=======================================================================
	void SetVertexPolygon(D3DXVECTOR3 pos, float width, float height);

	//=======================================================================
	// 色変更関数
	// 引数:色
	//=======================================================================
	void SetColorPolygon(D3DXCOLOR color);

	//=======================================================================
	// UV値変更関数
	// 引数:UV値構造体ポインタ
	//=======================================================================
	void SetUV(UV_INDEX *pUVIndex);

	//=======================================================================
	// 自動描画フラグセット関数
	// 引数:真偽値
	//=======================================================================
	void SetDrawFlag(bool flag){m_AutoDraw = flag;};

	//=======================================================================
	// テクスチャー変更関数
	// 引数:テクスチャーの種類
	//=======================================================================
	void ChangeTexture(TEXTURE_TYPE texType){m_pD3DTexBuff = CTexture::GetTexture(texType);};

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、座標、幅、高さ、テクスチャの種類
	// 戻り値:作成したアドレス
	//=======================================================================
	static CScene2D* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType);

private:
	float		m_fLength;				// ポリゴンの対角線の長さ
	float		m_fAngle;				// ポリゴンの対角線の角度
	float		m_fWidth;				// 幅
	float		m_fHeight;				// 高さ
	bool		m_AutoDraw;				// 自動で描画するか

protected:
	// 頂点情報セット関数
	void SetVertexPolygon(void);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// テクスチャ情報を格納する場所のアドレスを確保する場所
};
#endif
//----EOF----