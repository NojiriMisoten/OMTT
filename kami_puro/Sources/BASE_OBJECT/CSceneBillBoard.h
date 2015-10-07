//=============================================================================
//
// CSceneBillBoardクラス [CSceneBillBoard.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENE_BILLBOARD_H_
#define _CSCENE_BILLBOARD_H_

//=============================================================================
// インクルード
//=============================================================================
#include "CScene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CCameraManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneBillBoard : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ
	//======================================================
	CSceneBillBoard(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType);

	// デストラクタ
	~CSceneBillBoard();

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// ポジションアクセサ
	D3DXVECTOR3& GetPos(void){ return m_Pos;};
	void		SetPos(D3DXVECTOR3& pos){ m_Pos = pos;};
	void		SetPos(float x,float y, float z){ m_Pos.x = x; m_Pos.y = y; m_Pos.z = z;};

	// ロットアクセサ
	D3DXVECTOR3& GetRot(void){return m_Rot;};
	void		SetRot(D3DXVECTOR3& rot){m_Rot = rot;};
	void		SetRot(float x,float y,float z){ m_Rot.x = x; m_Rot.y = y; m_Rot.z = z;};

	// UV値変更
	void SetTexcordPolygon(D3DXVECTOR2& leftTop, D3DXVECTOR2& rightTop, D3DXVECTOR2& leftBottom, D3DXVECTOR2& rightBottom);
	void SetUV(float left, float right, float top, float bottom);

	//==================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、テクスチャータイプ、カメラマネージャーポインター
	// 戻り値: 作ったやつのポインタ
	//==================================================
	static CSceneBillBoard* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CCameraManager* pCameraManager, OBJTYPE objType = OBJTYPE_3D);

	// 色設定関数
	void SetColorPolygon(D3DXCOLOR& color);

protected:
	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャの種類、カメラマネージャーポインター
	// 戻り値: 成否
	//======================================================
	void Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CCameraManager* pCameraManager);

	// 頂点反映関数
	void SetVertexPolygon(void);

	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	D3DXMATRIX		m_mtxWorld;				// ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_Scl;					// 地面の大きさ（スケール）
	float					m_fLength;				// ポリゴンの対角線の長さ
	float					m_fAngle;				// ポリゴンの対角線の角度
	CCameraManager			*m_pCameraManager;		// カメラマネージャー

};
#endif
//----EOF----