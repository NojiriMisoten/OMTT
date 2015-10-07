//=============================================================================
//
// CSceneBillBoardクラス [CSceneBillBoard.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneBillBoard.h"
#include "../CAMERA/CameraManager.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneBillBoard::CSceneBillBoard(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) :CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_Scl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_pCameraManager = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneBillBoard ::~CSceneBillBoard(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CSceneBillBoard::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CCameraManager* pCameraManager)
{
	m_pCameraManager = pCameraManager;

	VF *pVtx;

	//原点の位置
	m_Pos = pos;
	
	//対角線の角度
	m_fAngle = D3DX_PI / 6;		// 30度にしたいため / 6

	m_Rot.z = 0;

	// 頂点宣言したものを作る
	(*m_pD3DDevice)->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	// ポリゴンのメモリ確保
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VF) * 4			// 四角形の頂点数なので4
												, D3DUSAGE_WRITEONLY			// 変えない
												, 0					// 使用する頂点フォーマット
												, D3DPOOL_MANAGED				// 変えない
												, &m_pD3DVtxBuff				// 格納する場所
												, NULL)))						// 変えない
	{
		return;
	}

	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	pVtx[0].vtx = D3DXVECTOR3(-width
							, height
							, 0.f);
	pVtx[1].vtx = D3DXVECTOR3(width
							, height
							, 0.f);
	pVtx[2].vtx = D3DXVECTOR3(-width
							, -height
							, 0.f);
	pVtx[3].vtx = D3DXVECTOR3(width
							, -height
							, 0.f);
	//--------------------------------------------------------------------------------
	// 法線
	pVtx[0].nor = DEFAULT_UP_VECTOR;
	pVtx[1].nor = DEFAULT_UP_VECTOR;
	pVtx[2].nor = DEFAULT_UP_VECTOR;
	pVtx[3].nor = DEFAULT_UP_VECTOR;
	//---------------------------------------------------------------------------------
	// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
	pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
	//---------------------------------------------------------------------------------
	// 使用したいテクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(MIN_UV_VALUE, MIN_UV_VALUE);
	pVtx[1].tex = D3DXVECTOR2(MAX_UV_VALUE, MIN_UV_VALUE);
	pVtx[2].tex = D3DXVECTOR2(MIN_UV_VALUE, MAX_UV_VALUE);
	pVtx[3].tex = D3DXVECTOR2(MAX_UV_VALUE, MAX_UV_VALUE);
	//----------------------------------------------------------------------------------
	m_pD3DTexBuff = CTexture::GetTexture( texType );
	m_pD3DVtxBuff->Unlock();								// ロックしたら必ずアンロック！！
	//********************************************************************************************************************

}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneBillBoard ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	m_pD3DTexBuff = NULL;

	SAFE_RELEASE(m_pDecl);

	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneBillBoard ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneBillBoard ::Draw(void)
{
	D3DXMATRIX mtxView;
	
	// カメラの位置取得
	mtxView = m_pCameraManager->GetMtxView();

	D3DXMATRIX mtxScl,mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// 誤差の消去
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 3Dポリゴンの描画
	// ビルボードポリゴンの描画
	(*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);											// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VF));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);								// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP							// プリミティブの種類
							, 0
							, DRAW_SQUARE_PRINITIV_NUM);						// 書きたいポリゴン数(三角形基準・・・四角形作るならポリゴン数２）
}

//*****************************************************************************
// ポリゴンセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneBillBoard :: SetVertexPolygon(void)
{
	VF *pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	pVtx[0].vtx = D3DXVECTOR3(m_Pos.x - m_fLength
							, m_Pos.y + m_fLength
							, m_Pos.z);
	pVtx[1].vtx = D3DXVECTOR3(m_Pos.x + m_fLength
							, m_Pos.y + m_fLength
							, m_Pos.z);
	pVtx[2].vtx = D3DXVECTOR3(m_Pos.x - m_fLength
							, m_Pos.y - m_fLength
							, m_Pos.z);
	pVtx[3].vtx = D3DXVECTOR3(m_Pos.x + m_fLength
							, m_Pos.y - m_fLength
							, m_Pos.z);
	m_pD3DVtxBuff->Unlock();												// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneBillBoard::SetColorPolygon(D3DXCOLOR& color)
{
	VF *pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
	pVtx[0].diffuse = color;
	pVtx[1].diffuse = color;
	pVtx[2].diffuse = color;
	pVtx[3].diffuse = color;
	m_pD3DVtxBuff->Unlock();									// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneBillBoard::SetTexcordPolygon(D3DXVECTOR2& leftTop, D3DXVECTOR2& rightTop, D3DXVECTOR2& leftBottom, D3DXVECTOR2& rightBottom)
{
	VF *pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);			// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
	pVtx[0].tex = leftTop;
	pVtx[1].tex = rightTop;
	pVtx[2].tex = leftBottom;
	pVtx[3].tex = rightBottom;
	m_pD3DVtxBuff->Unlock();								// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneBillBoard* CSceneBillBoard::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CCameraManager* pCameraManager, OBJTYPE objType)
{
	// 作成
	CSceneBillBoard* p = new CSceneBillBoard(pDevice, objType);

	// 初期化
	p->Init(pos, width, height, texType, pCameraManager);

	return p;
}

//*****************************************************************************
// UV設定関数
//*****************************************************************************
void CSceneBillBoard::SetUV(float left, float right, float top, float bottom)
{
	VF *pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	// 使用したいテクスチャの座標(0～1)
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, bottom);
	pVtx[3].tex = D3DXVECTOR2(right, bottom);
	m_pD3DVtxBuff->Unlock();									// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}
//----EOF----