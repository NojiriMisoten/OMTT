//=============================================================================
//
// CSceneXクラス [CSceneX.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENEX_H_
#define _CSCENEX_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../MAIN/main.h"
#include "CScene.h"
#include "../MODEL/CModel.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneX : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、オブジェタイプ
	//======================================================
	CSceneX(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE m_objType = OBJTYPE_X);

	// デストラクタ
	~CSceneX();

	//======================================================
	// 初期化
	// 引数: 座標、モデルのファイルパス
	// 戻り値: 成否
	//======================================================
	void Init(D3DXVECTOR3& pos, char* modelFileName);

	//======================================================
	// 初期化
	// 引数: 座標、モデルタイプ
	// 戻り値: 成否
	//======================================================
	void Init(D3DXVECTOR3& pos, MODEL_TYPE type);
	
	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// ポジションアクセサ
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	D3DXVECTOR3* GetPosAdr(void){ return &m_Pos;};
	void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos;};
	void		SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};

	// ロットアクセサ
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void		SetRot(D3DXVECTOR3& rot){m_Rot = rot;};
	void		SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};
	
	// スケールアクセサ
	D3DXVECTOR3& GetScl(void){return m_vScl;};
	void		SetScl(D3DXVECTOR3& scl){m_vScl = scl;};
	void		SetScl(float x,float y,float z){ m_vScl.x = x; m_vScl.y = y; m_vScl.z = z;};
	
	//=================================================
	// 半径取得
	// 戻り値: 半径
	//=================================================
	float GetRadius(void){return m_fRadius;};

	//=================================================
	// 高さ取得
	// 戻り値: 高さ
	//=================================================
	float GetHeight(void){return m_fHeight;};

	//=================================================
	// アルファ値セット
	// 引数: アルファ値
	//=================================================
	void SetAlpha(float alpha);

	//=================================================
	// スペキュラーセット
	// 引数: スペキュラーパワー
	//=================================================
	void SetSpecularPow(float pow);

	//=================================================
	// メッシュ情報取得
	// 戻り値: メッシュポインタ
	//=================================================
	LPD3DXMESH* GetMesh(void){return &m_ModelInfo->pD3DXMeshModel;};

	//=================================================
	// インデックスバッファー取得
	// 戻り値: インデックスバッファー
	//=================================================
	WORD* GetIndexBuffer(void){return m_ModelInfo->pIndex;};
	
	//=================================================
	// バーテックスバッファー取得
	// 戻り値: バーテックスバッファー
	//=================================================
	VERTEX* GetVertexBuffer(void){return m_ModelInfo->pVertex;};

	// エッジテクスチャ作成
	void CreateEdgeTexture(void);

	// シャドウテクスチャ作成
	void CreateShadowTexture(void);

	//=================================================
	// クリエイト関数
	// 引数: デバイス、座標、モデルファイルパス
	// 戻り値: 作ったやつのアドレス
	//=================================================
	static CSceneX* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, char* modelFileName);

	//=================================================
	// クリエイト関数
	// 引数: デバイス、座標、モデルタイプ
	// 戻り値: 作ったやつのアドレス
	//=================================================
	static CSceneX* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, MODEL_TYPE type);

protected:
	float				m_fRadius;								// 半径
	float				m_fWidth;								// 幅（あたり判定用）
	float				m_fDepth;								// 奥行（あたり判定用）
	float				m_fHeight;								// 高さ（あたり判定用）
	LPDIRECT3DDEVICE9	*m_pD3DDevice;							// デバイスオブジェクト(描画に必要)
	X_MODEL				*m_ModelInfo;							// モデルの情報
	D3DXVECTOR3			m_vScl;									// 大きさ（スケール）
	char				m_cModelFileName[MAX_LENGTH_FILE_PATH];	// モデルのファイルネーム
	static D3DXMATRIX	m_mtxWorld;								// ワールドマトリックス
};
#endif
//----EOF----