//=============================================================================
//
// CRopeクラス [CRope.h]
// Author : 塚本俊彦
//
//=============================================================================
#ifndef _CROPE_H_
#define _CROPE_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../../BASE_OBJECT/CScene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRope : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ、オブジェタイプ
	//======================================================
	CRope(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_3D);

	// デストラクタ
	~CRope();

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void DrawNormalRender(void);

	// ポジションアクセサ
	D3DXVECTOR3& GetPos(void){ return m_Pos; };
	void SetPos(D3DXVECTOR3& pos){ m_Pos = pos; };
	void SetPos(float x, float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z; };

	// ロットアクセサ
	D3DXVECTOR3& GetRot(void){ return m_Rot; };
	void SetRot(D3DXVECTOR3& rot){ m_Rot = rot; };
	void SetRot(float x, float y, float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z; };

	//=================================================
	// 高さ取得
	// 引数: 知りたい場所、(out)法線
	// 戻り値: 高さ
	//=================================================
	float GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	//=================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、ブロック数、テクスチャタイプ
	// 戻り値: 作ったやつのアドレス
	//=================================================
	static CRope* Create(
		LPDIRECT3DDEVICE9 *pDevice,
		D3DXVECTOR3 pos, float width, float height,
		int wBlock, int hBlock,
		TEXTURE_TYPE texType,
		CManager* pManager);

	// ゴムアニメ―ションスタート
	// 引っ張る力、その手を離すまでのフレームカウント
	void Pull(float pullPower, int pullInterval);

private:
	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ、(横分割数、縦分割数)
	// 戻り値: 成否
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager, int wblock = 1, int hblock = 1);

	// 保存しときたいポインタ
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	CManager				*m_pManager;

	// メッシュを出すための変数
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_vScl;					// 地面の大きさ（スケール）
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
	int						m_nNumVtxNum;						// 頂点数
	int						m_nNumPolygon;						// ポリゴン数
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;		// インデックス情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_vCornerPos[4];		// 角の座標
	D3DXVECTOR3*			m_vFaceNormalBuf;		// 面法線バッファ
	D3DXVECTOR3*			m_vVtxNormalBuf;		// 頂点法線バッファ
	float					m_fWidth;				// ポリゴンの対角線の長さ
	float					m_fHeight;				// ポリゴンの対角線の角度
	float					m_fAngle;				// ポリゴンの対角線の角度
	float					m_fGridWidth;			// グリッドの幅
	float					m_fGridHeight;			// グリッドの高さ
	int						m_nNumVertexIndex;		// インデックス数
	int						m_nNumWidthBlock;		// 横の分割数
	int						m_nNumHeightBlock;		// 縦の分割数
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;

	// ゴムの動きをするための変数
	float m_PosMax;		// 可動範囲の座標最大
	float m_PosMin;		// 可動範囲の座標最小
	float m_PullPower;	// 座標を動かす引く力
	int m_PullInterval;	// 引くフレームカウント
	int m_PullCount;	// それをカウントする
};

#endif
//----EOF----