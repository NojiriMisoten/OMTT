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
#include "../../MAIN/main.h"
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
	CScene2D(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_0, OBJTYPE m_objType = OBJTYPE_2D);

	// デストラクタ
	~CScene2D();

	//=======================================================================
	// 初期化関数
	// 引数:座標、幅、高さ、テクスチャの種類、（以下必要なし）（横分割数、縦分割数）
	// 戻り値:作成したアドレス
	//=======================================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// ポジションアクセサ
	D3DXVECTOR3 GetPos(void){ return m_vPos; };
	void		SetPos(D3DXVECTOR3 pos);
	void		SetPos(float x, float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z; };
	void		AddPos(D3DXVECTOR3 pos){ m_vPos += pos; };

	// ロットアクセサ
	D3DXVECTOR3 GetRot(void){ return m_vRot; };
	void		SetRot(D3DXVECTOR3 rot);
	void		SetRot(float x, float y, float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z; };

	// サイズアクセサ
	float GetWidth(void){ return m_fWidth; };
	void SetWidth(float width){ m_fWidth = width; };
	float GetHeight(void){ return m_fHeight; };
	void SetHeight(float height){ m_fHeight = height; };

	//=======================================================================
	// サイズと場所変更関数
	// 引数:座標、幅、高さ
	//=======================================================================
	void SetVertexPolygon(D3DXVECTOR3 pos, float width, float height);

	//=======================================================================
	// 幅変更関数_左側基点（右側伸びる）
	// 引数:加えたい幅
	//=======================================================================
	void AddWidth_BaseLeft(float addWidth);

	//=======================================================================
	// 幅変更関数_右側基点（左側伸びる）
	// 引数:加えたい幅
	//=======================================================================
	void AddWidth_BaseRight(float addWidth);

	//=======================================================================
	// 幅変更関数_下側基点（上側伸びる）
	// 引数:加えたい高さ
	//=======================================================================
	void AddHeight_BaseBottom(float addHeight);

	//=======================================================================
	// 幅変更関数_上側基点（下側伸びる）
	// 引数:加えたい高さ
	//=======================================================================
	void AddHeight_BaseTop(float addHeight);

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
	// UV値90度回転関数
	//=======================================================================
	void Rot90_UV(void);

	//=======================================================================
	// 自動描画フラグセット関数
	// 引数:真偽値
	//=======================================================================
	void SetDrawFlag(bool flag){ m_AutoDraw = flag; };

	//=======================================================================
	// テクスチャー変更関数
	// 引数:テクスチャーの種類
	//=======================================================================
	void ChangeTexture(TEXTURE_TYPE texType){ m_pD3DTexBuff = CTexture::GetTexture(texType); };

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、座標、幅、高さ、テクスチャの種類
	// 戻り値:作成したアドレス
	//=======================================================================
	static CScene2D* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int nPriority = TYPE_PRIORITY_0);

	//=======================================================================
	// 頂点座標取得関数
	// 引数:頂点番号
	// 戻り値:座標
	//=======================================================================
	D3DXVECTOR2* GetVertexPos(int idx){ return &m_vtxPos[idx]; };

	//=======================================================================
	// 当たり判定用頂点座標取得関数
	// 引数:頂点番号
	// 戻り値:座標
	//=======================================================================
	D3DXVECTOR2* GetVertexJudgePos(int idx){ return &m_vtxJudgePos[idx]; };

	D3DXVECTOR3 GetJudgePos(void){ return m_vJudgePos; };
	float GetJudgeWidth(void){ return m_fJudgeWidth; };
	float GetJudgeHeight(void){ return m_fJudgeHeight; };

private:
	static const int VERTEX_NUM = 4;	// 頂点の数
	bool		m_AutoDraw;				// 自動で描画するか
	D3DXVECTOR2	m_vtxPos[VERTEX_NUM];	// 頂点の座標
	D3DXVECTOR2	m_vtxJudgePos[VERTEX_NUM];	// 頂点の座標

protected:
	// 頂点情報セットワールド変換関数
	void SetVertexPolygon(void);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3	m_vPos;								// ポリゴンの中央の位置
	D3DXVECTOR3	m_vRot;								// ポリゴンの回転
	float		m_fWidth;							// 幅
	float		m_fHeight;							// 高さ
	// 当たり判定用のパラメーター
	D3DXVECTOR3 m_vJudgePos;
	float m_fJudgeWidth;
	float m_fJudgeHeight;
};
#endif
//----EOF----