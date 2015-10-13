//=============================================================================
//
// CSceneMeshFieldクラス [CSceneMeshField.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENEMESHFIELD_H_
#define _CSCENEMESHFIELD_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../MAIN/main.h"
#include "CScene.h"

class CManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneMeshField : CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: オブジェタイプ
	//======================================================
	CSceneMeshField(LPDIRECT3DDEVICE9 *pDevice);

	// デストラクタ
	~CSceneMeshField();
	
	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void DrawNormalRender(void);
	void DrawToonObjectDepthRender(void);
	void DrawNormalVecRender(void);
	
	// ポジションアクセサ
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos;};
	void		SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};
	
	// ロットアクセサ
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void		SetRot(D3DXVECTOR3& rot){m_Rot = rot;};
	void		SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};
	
	//=================================================
	// 高さ取得
	// 引数: 知りたい場所、(out)法線
	// 戻り値: 高さ
	//=================================================
	float GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	//=================================================
	// サイズゲット
	// 引数: (out)Ｘサイズ、(out)Ｚサイズ
	//=================================================
	void GetSize(float* pOutXSize, float* pOutZSize){*pOutXSize = m_fWidth; *pOutZSize = m_fHeight;};

	//=================================================
	// 角の座標ゲット
	// 引数: 角の番号
	// 戻り値: 角の座標
	//=================================================
	D3DXVECTOR3& GetCornerPos(int idx){ return m_vCornerPos[idx];};

	//=================================================
	// クリエイト関数
	// 引数: デバイス、フィールドファイルパス
	// 戻り値: 作ったやつのポインタ
	//=================================================
	static CSceneMeshField* Create(LPDIRECT3DDEVICE9 *pDevice, char* fieldFileName, CManager* pManager);

private:
	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャーファイルネーム、(横分割数、縦分割数)
	// 戻り値: 成否
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, char* texFileName, CManager* pManager, int wblock = 1, int hblock = 1, bool useMap = true);

	// 頂点セット
	void SetVertexPolygon(void);

	//=================================================
	// 色変更関数
	// 引数: 色
	//=================================================
	void SetColorPolygon(D3DXCOLOR& color);

	//=================================================
	// ポリゴンの高さ取得
	// 引数: 頂点座標0、頂点座標1、頂点座標2、調べたい座標、（out）法線ポインタ
	// 戻り値: 高さ
	//=================================================
	float GetHeightPolygon(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	LPDIRECT3DDEVICE9		*m_pD3DDevice;						// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;						// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;					// インデックス情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;						// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_vCornerPos[4];					// 角の座標
	D3DXVECTOR3*			m_vFaceNormalBuf;					// 面法線バッファ
	D3DXVECTOR3*			m_vVtxNormalBuf;					// 頂点法線バッファ
	float					m_fAngle;							// ポリゴンの対角線の角度
	float					m_fWidth;							// 幅
	float					m_fHeight;							// 高さ
	float					m_fGridWidth;						// グリッドの幅
	float					m_fGridHeight;						// グリッドの高さ
	int						m_nNumPolygon;						// ポリゴン数
	int						m_nNumVertexIndex;					// インデックス数
	int						m_nNumVtxNum;						// 頂点数
	int						m_nNumWidthBlock;					// 横の分割数
	int						m_nNumHeightBlock;					// 縦の分割数
	int						m_nID;								// ID
	CManager				*m_pManager;
	static D3DXMATRIX		m_mtxWorld;							// ワールドマトリックス
};
#endif
//----EOF----