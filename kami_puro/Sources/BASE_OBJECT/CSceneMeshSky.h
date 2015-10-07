//=============================================================================
//
// CSceneMeshSkyクラス [CSceneMeshSky.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENEMESHSKY_H_
#define _CSCENEMESHSKY_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../MAIN/main.h"
#include "CScene.h"

//=============================================================================
// マクロ
//=============================================================================
static const D3DXVECTOR3	DEFAULT_SKY_POS(0.f, -50.f, 0.f);	// デフォルトの座標
static const float			DEFAULT_SKY_RADIUS = 400.f;	// デフォルトの半径
static const float			DEFAULT_SKY_HEIGHT = 400.f;	// デフォルトの高さ
static const int			DEFAULT_SKY_SEPLATE_NUM = 20;	// デフォルトの分割数

class CManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneMeshSky : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ
	//======================================================
	CSceneMeshSky(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objtype = OBJTYPE_3D);

	// デストラクタ
	~CSceneMeshSky();

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ、横分割数、縦分割数
	// 戻り値: 成否
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void DrawNormalRender(void);
	void DrawBaseHighLuminanceRender(void);

	// ポジションアクセサ
	D3DXVECTOR3& GetPos(void){ return m_Pos; };
	void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos; };
	void		SetPos(float x, float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z; };

	// ロットアクセサ
	D3DXVECTOR3& GetRot(void){ return m_Rot; };
	void		SetRot(D3DXVECTOR3& rot){ m_Rot = rot; };
	void		SetRot(float x, float y, float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z; };

	//=================================================
	// クリエイト関数
	// 引数: デバイス、座標、半径、幅、高さ、横分割数、縦分割数、テクスチャタイプ、角度
	// 戻り値: 作ったやつのアドレス
	//=================================================
	static CSceneMeshSky* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float r, float height, int wblock, int hblock, TEXTURE_TYPE texType, CManager* pManager, D3DXVECTOR3 rot = D3DXVECTOR3(0, 0, 0));

private:
	// 頂点反映関数
	void SetVertexPolygon(void);

	//=================================================
	// 色変更関数
	// 引数: 色
	//=================================================
	void SetColorPolygon(D3DXCOLOR color);

	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;		// インデックス情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_vPos;					// ポリゴンの中央の位置
	D3DXVECTOR3				m_vRot;					// ポリゴンの回転
	D3DXVECTOR3				m_vScl;					// 大きさ（スケール）
	float					m_fAngle;				// ポリゴンの対角線の角度
	float					m_fRadius;				// 半径
	float					m_fHeight;				// 高さ
	int						m_nNumPolygon;			// ポリゴン数
	int						m_nNumVertexIndex;		// インデックス数
	int						m_nNumVtxNum;			// 頂点数
	int						m_nNumWidthBlock;		// 横の分割数
	int						m_nNumHeightBlock;		// 縦の分割数
	CManager				*m_pManager;
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
};
#endif
//----EOF----