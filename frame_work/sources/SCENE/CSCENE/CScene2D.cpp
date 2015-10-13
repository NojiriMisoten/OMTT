//=============================================================================
//
// CScene2Dクラス [CScene2D.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CScene2D.h"
#include "../../MATH/matrix.h"
#include "../../MATH/vector.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CScene2D::CScene2D(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene(nPriority, objType)
{
	m_pD3DDevice = pDevice;									// デバイスオブジェクト(描画に必要)
	m_pD3DVtxBuff = NULL;									// 頂点座標情報を格納する場所のアドレスを確保する場所
	m_pD3DTexBuff = NULL;									// テクスチャ情報を格納する場所のアドレスを確保する場所
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ポリゴンの中央の位置
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ポリゴンの回転
	m_AutoDraw = true;
	for (int idx = 0; idx < VERTEX_NUM; ++idx)
	{
		m_vtxPos[idx] = D3DXVECTOR2(0.0f, 0.0f);
	}
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CScene2D ::~CScene2D(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CScene2D::Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VERTEX_2D *pVtx;

	// 原点の位置
	m_vPos = pos;

	// 角度初期化
	m_vRot.z = 0.f;

	// 幅高さ設定
	m_fWidth = width;
	m_fHeight = height;

	// ポリゴンのメモリ確保
	if (FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D)* 4			// 四角形の頂点数だから * 4
		, D3DUSAGE_WRITEONLY			// 変えない
		, FVF_VERTEX_2D					// 使用する頂点フォーマット
		, D3DPOOL_MANAGED				// 変えない
		, &m_pD3DVtxBuff				// 格納する場所
		, NULL)))						// 変えない
	{
		return E_FAIL;															// メモリを確保できなかったらメインに返す
	}

	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	pVtx[0].vtx = D3DXVECTOR3(- m_fWidth * 0.5f		// 中心からの距離なので半分に
							, - m_fHeight * 0.5f
							, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(+ m_fWidth * 0.5f
							, - m_fHeight * 0.5f
							, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(- m_fWidth * 0.5f
							, + m_fHeight * 0.5f
							, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(+ m_fWidth * 0.5f
							, + m_fHeight * 0.5f
							, 0.0f);
	//--------------------------------------------------------------------------------
	// 中身は常に1.0f
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//---------------------------------------------------------------------------------
	// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
	pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
	//--------------------------------------------------------------------------------
	// 使用したいテクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(MIN_UV_VALUE, MIN_UV_VALUE);
	pVtx[1].tex = D3DXVECTOR2(MAX_UV_VALUE, MIN_UV_VALUE);
	pVtx[2].tex = D3DXVECTOR2(MIN_UV_VALUE, MAX_UV_VALUE);
	pVtx[3].tex = D3DXVECTOR2(MAX_UV_VALUE, MAX_UV_VALUE);
	//----------------------------------------------------------------------------------
	m_pD3DTexBuff = CTexture::GetTexture(texType);
	m_pD3DVtxBuff->Unlock();																	// ロックしたら必ずアンロック！！
	//********************************************************************************************************************

	// 座標変換
	SetVertexPolygon();

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CScene2D::Uninit(void)
{
	if (m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	m_pD3DTexBuff = NULL;

	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CScene2D::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CScene2D::Draw(void)
{
	// 自動描画ONなら
	if (m_AutoDraw)
	{
		// 座標変換
		SetVertexPolygon();

		// ポリゴンの描画
		(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,渡すものが入ってるやつ,0,データの型指定)
		(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									// 頂点フォーマットの設定
		(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);							// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
		(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						// プリミティブの種類
			, 0											// 描画を開始する頂点番号
			, DRAW_SQUARE_PRINITIV_NUM);				// 書きたいポリゴン数
	}
}

//*****************************************************************************
// ポリゴンセット関数  更新で呼んだりする
//*****************************************************************************
void CScene2D::SetVertexPolygon(void)
{
	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v, vertexPos[4];

	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	// 座標変換
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);

	// ポリゴンの設定
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
								, v.y
								, 0.0f);

		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ロックしたら必ずアンロック！！


	pos = VECTOR2(m_vJudgePos.x, m_vJudgePos.y);
	vertexPos[0] = VECTOR2(-m_fJudgeWidth * 0.5f, -m_fJudgeHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fJudgeWidth * 0.5f, -m_fJudgeHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fJudgeWidth * 0.5f, m_fJudgeHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fJudgeWidth * 0.5f, m_fJudgeHeight * 0.5f);

	// 座標変換
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		m_vtxJudgePos[i].x = v.x;
		m_vtxJudgePos[i].y = v.y;
	}
}

//*****************************************************************************
// 頂点変更
//*****************************************************************************
void CScene2D::SetVertexPolygon(D3DXVECTOR3 pos, float width, float height)
{
	m_vPos = pos;
	m_fWidth = width;
	m_fHeight = height;
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void	CScene2D::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_2D	*pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
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
CScene2D* CScene2D::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int nPriority)
{
	// 作成
	CScene2D* p = new CScene2D(pDevice, nPriority);

	// 初期化
	p->Init(pos, width, height, texType);

	return p;
}

//*****************************************************************************
// UVセット関数
//*****************************************************************************
void	CScene2D::SetUV(UV_INDEX *pUVIndex)
{
	VERTEX_2D *pVtx;
	//********************************************************************************************************************
	//ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)

	pVtx[0].tex = D3DXVECTOR2(pUVIndex->left, pUVIndex->top);
	pVtx[1].tex = D3DXVECTOR2(pUVIndex->right, pUVIndex->top);
	pVtx[2].tex = D3DXVECTOR2(pUVIndex->left, pUVIndex->bottom);
	pVtx[3].tex = D3DXVECTOR2(pUVIndex->right, pUVIndex->bottom);

	m_pD3DVtxBuff->Unlock();									// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}

//*****************************************************************************
// UV値90度回転関数
//*****************************************************************************
void	CScene2D::Rot90_UV(void)
{
	VERTEX_2D *pVtx;
	D3DXVECTOR2 uv[4];
	//********************************************************************************************************************
	//ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)

	for (int i = 0; i < 4; ++i)
	{
		uv[i] = pVtx[i].tex;
	}

	pVtx[0].tex = uv[1];
	pVtx[1].tex = uv[3];
	pVtx[2].tex = uv[0];
	pVtx[3].tex = uv[2];

	m_pD3DVtxBuff->Unlock();									// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}

//*****************************************************************************
// セット関数
//*****************************************************************************
void	CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_vPos = pos;
}

//*****************************************************************************
// セット関数
//*****************************************************************************
void	CScene2D::SetRot(D3DXVECTOR3 rot)
{
	m_vRot = rot;
}

//*****************************************************************************
// サイズ変更関数_左側基点
//*****************************************************************************
void CScene2D::AddWidth_BaseLeft(float addWidth)
{
	// 変更量計算
	m_fWidth += addWidth;

	VECTOR2 vertexPos[4];
	
	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	m_vPos.x += addWidth * 0.5f;

	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v;

	// 座標変換
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);
	
	// ポリゴンの設定
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
			, v.y
			, 0.0f);
	
		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ロックしたら必ずアンロック！！
}

//*****************************************************************************
// サイズ変更関数_右側基点
//*****************************************************************************
void CScene2D::AddWidth_BaseRight(float addWidth)
{
	// 変更量計算
	m_fWidth += addWidth;

	VECTOR2 vertexPos[4];

	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	m_vPos.x -= addWidth * 0.5f;

	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v;

	// 座標変換
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);

	// ポリゴンの設定
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
			, v.y
			, 0.0f);

		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ロックしたら必ずアンロック！！
}

//*****************************************************************************
// 幅変更関数_下側基点（上側伸びる）
//*****************************************************************************
void CScene2D::AddHeight_BaseBottom(float addHeight)
{
	// 変更量計算
	m_fHeight += addHeight;

	VECTOR2 vertexPos[4];

	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	m_vPos.y -= addHeight * 0.5f;

	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v;

	// 座標変換
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);

	// ポリゴンの設定
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
			, v.y
			, 0.0f);

		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ロックしたら必ずアンロック！！
}

//*****************************************************************************
// 幅変更関数_上側基点（下側伸びる）
//*****************************************************************************
void CScene2D::AddHeight_BaseTop(float addHeight)
{
	// 変更量計算
	m_fHeight += addHeight;

	VECTOR2 vertexPos[4];

	vertexPos[0] = VECTOR2(-m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[1] = VECTOR2(m_fWidth * 0.5f, -m_fHeight * 0.5f);
	vertexPos[2] = VECTOR2(-m_fWidth * 0.5f, m_fHeight * 0.5f);
	vertexPos[3] = VECTOR2(m_fWidth * 0.5f, m_fHeight * 0.5f);

	m_vPos.y += addHeight * 0.5f;

	MATRIX3 transrationMatrix, rotationMatrix, worldMatrix;
	VECTOR2 pos(m_vPos.x, m_vPos.y), v;

	// 座標変換
	Matrix3Identity(&worldMatrix);
	Matrix3Rotation(&rotationMatrix, m_vRot.z);
	Matrix3Translation(&transrationMatrix, &pos);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &rotationMatrix);
	Matrix3Multiply(&worldMatrix, &worldMatrix, &transrationMatrix);

	// ポリゴンの設定
	VERTEX_2D *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	for (int i = 0; i < 4; i++)
	{
		Vector2Transform(&v, &vertexPos[i], &worldMatrix);
		pVtx[i].vtx = D3DXVECTOR3(v.x
			, v.y
			, 0.0f);

		m_vtxPos[i].x = pVtx[i].vtx.x;
		m_vtxPos[i].y = pVtx[i].vtx.y;
	}
	m_pD3DVtxBuff->Unlock();		// ロックしたら必ずアンロック！！
}

//----EOF----