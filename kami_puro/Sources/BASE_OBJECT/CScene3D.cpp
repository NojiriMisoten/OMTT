//=============================================================================
//
// CScene3Dクラス [CScene3D.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CScene3D.h"

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
D3DXMATRIX CScene3D ::m_mtxWorld;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CScene3D ::CScene3D(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType):CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CScene3D ::~CScene3D(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CScene3D :: Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VF *pVtx;
	
	//原点の位置
	m_Pos = pos;

	// 幅と高さ
	m_fWidth = width;
	m_fHeight = height;
	
	// 頂点宣言したものを作る
	(*m_pD3DDevice)->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	// ポリゴンのメモリ確保
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VF) * 4			// 四角形の頂点数なので * 4
												, D3DUSAGE_WRITEONLY			// 変えない
												, 0					// 使用する頂点フォーマット
												, D3DPOOL_MANAGED				// 変えない
												, &m_pD3DVtxBuff				// 格納する場所
												, NULL)))						// 変えない
	{
		return;															// メモリを確保できなかったらメインに返す
	}

	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	pVtx[0].vtx = D3DXVECTOR3(-m_fWidth * 0.5f								// 中心からの距離
							, 0.f
							, m_fHeight * 0.5f);
	pVtx[1].vtx = D3DXVECTOR3(m_fWidth * 0.5f
							, 0.f	
							, m_fHeight * 0.5f);
	pVtx[2].vtx = D3DXVECTOR3(-m_fWidth * 0.5f
							, 0.f
							, -m_fHeight * 0.5f);
	pVtx[3].vtx = D3DXVECTOR3(m_fWidth * 0.5f
							, 0.f
							, -m_fHeight * 0.5f);
	//--------------------------------------------------------------------------------
	// 法線
	pVtx[0].nor = DEFAULT_UP_VECTOR;
	pVtx[1].nor = DEFAULT_UP_VECTOR;
	pVtx[2].nor = DEFAULT_UP_VECTOR;
	pVtx[3].nor = DEFAULT_UP_VECTOR;
	//-	-------------------------------------------------------------------------------
	//色と透明度の設定（テクスチャを張るのでポリゴンは白で）
	pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
	//--------------------------------------------------------------------------------
	//使用したいテクスチャの座標(0～1)
	pVtx[0].tex = D3DXVECTOR2(MIN_ALPHA, MIN_ALPHA);
	pVtx[1].tex = D3DXVECTOR2(MAX_ALPHA, MIN_ALPHA);
	pVtx[2].tex = D3DXVECTOR2(MIN_ALPHA, MAX_ALPHA);
	pVtx[3].tex = D3DXVECTOR2(MAX_ALPHA, MAX_ALPHA);
	//----------------------------------------------------------------------------------
	m_pD3DVtxBuff->Unlock();											//ロックしたら必ずアンロック！！
	//********************************************************************************************************************
	
	// テクスチャ設定
	m_pD3DTexBuff = CTexture::GetTexture(texType);


	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CScene3D ::Uninit(void)
{
	SAFE_RELEASE(m_pD3DVtxBuff);
	m_pD3DTexBuff = NULL;
	SAFE_RELEASE(m_pDecl);
	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CScene3D ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CScene3D ::Draw(void)
{
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映		*****順番注意*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);	// Y軸を基点に回転しているのでY,X,Zの順に入れる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
	// ポリゴンの描画
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VF));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);											// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);								// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP							// プリミティブの種類
								, 0												// 描画を開始する頂点番号
								, DRAW_SQUARE_PRINITIV_NUM);					// 書きたいポリゴン数
}

//*****************************************************************************
// ポリゴンセット関数  更新で呼んだりする
//*****************************************************************************
void CScene3D :: SetVertexPolygon(void)
{
	VF *pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	pVtx[0].vtx = D3DXVECTOR3(-m_fWidth * 0.5f					// 中心からの距離なので半分に
							, 0.f
							, m_fHeight * 0.5f);
	pVtx[1].vtx = D3DXVECTOR3(m_fWidth * 0.5f
							, 0.f
							, m_fHeight * 0.5f);
	pVtx[2].vtx = D3DXVECTOR3(-m_fWidth * 0.5f
							, 0.f
							, -m_fHeight * 0.5f);
	pVtx[3].vtx = D3DXVECTOR3(m_fWidth * 0.5f
							, 0.f
							, -m_fHeight * 0.5f);
	m_pD3DVtxBuff->Unlock();													//ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void	CScene3D::SetColorPolygon(D3DXCOLOR color)
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
// クリエイト関数
//*****************************************************************************
CScene3D* CScene3D::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	// 作成
	CScene3D* p = new CScene3D(pDevice);

	// 初期化
	p->Init(pos, width, height, texType);

	return p;
	
}
//----EOF----