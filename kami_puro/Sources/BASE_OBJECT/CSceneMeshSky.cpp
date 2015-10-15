//=============================================================================
//
// CSceneMeshSkyクラス [CSceneMeshSky.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneMeshSky.h"
#include "../SHADER/CShader.h"
#include "../CAMERA/CameraManager.h"
#include "../RENDERER/CRenderer.h"
#include "../MANAGER/CManager.h"
//*****************************************************************************
// マクロ
//*****************************************************************************
static const float DEFAULT_ROT_SPD = 0.0001f;		// 空の動く速さ
static const float CULC_TEX_Y_COEFFICIENT = 1.3f;	// テクスチャのY座標計算の係数
static const float CULC_POS_Y_COEFFICIENT = 8.f;	// Y座標計算の係数
static const float MIN_TEX_POS = 0.01f;				// テクスチャの最小座標

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneMeshSky::CSceneMeshSky(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) :CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fAngle = 0.0f;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneMeshSky ::~CSceneMeshSky(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CSceneMeshSky :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VF *pVtx;
	
	float DistanceXVertex,DistanceZVertex;	// 頂点間の距離
	float lineX=0;
	float lineZ=0;
	float nNumDig = 0;						// 角度用
	float PlusDig = 0;
	float tex_X = MAX_UV_VALUE;
	float tex_Y = MAX_UV_VALUE;
	float Add_tex_X = MAX_UV_VALUE;
	float Add_tex_Y = MAX_UV_VALUE;
	float r = width;
	int counter =0;
	m_nNumWidthBlock = wblock;
	m_nNumHeightBlock = hblock;
	m_fRadius = width;
	m_fHeight = height;
	// 角度の計算
	PlusDig = D3DX_PI * 2.f / (float)m_nNumWidthBlock;	// 360度にしたいから２倍している
	nNumDig = 0;

	// テクスチャの増加量計算
	Add_tex_X = MAX_UV_VALUE / (float)(m_nNumWidthBlock);
	Add_tex_Y = MAX_UV_VALUE / (float)(m_nNumHeightBlock / CULC_TEX_Y_COEFFICIENT);

	// 総ポリゴン数の計算             ↓四角形は三角形２個なので
	m_nNumPolygon = wblock * hblock * 2 + ((hblock - 1) * 4);	// +~の部分は縮退ポリゴン分

	// 総インデックス数の計算  ３は最初の三角形の頂点数
	m_nNumVertexIndex = 3 + (m_nNumPolygon - 1);	// 最初の１個以降は１頂点プラスでポリゴンが書ける

	// 頂点間の距離算出
	DistanceXVertex = width / wblock;
	DistanceZVertex = height / hblock;

	// 原点の位置
	m_vPos = pos;
	
	// 頂点の数を設定
	m_nNumVtxNum = (wblock + 1) * (hblock + 1);

	// 頂点宣言したものを作る
	(*m_pD3DDevice)->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	// ポリゴンのメモリ確保
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VF) * m_nNumVtxNum	// VERTEX_2Dの型で４頂点*表示桁数分確保
												, D3DUSAGE_WRITEONLY				// 変えない
												, 0						// 使用する頂点フォーマット
												, D3DPOOL_MANAGED					// 変えない
												, &m_pD3DVtxBuff					// 格納する場所
												, NULL)))							// 変えない
	{
		return E_FAIL;																// メモリを確保できなかったらメインに返す
	}

	// インデックスバッファの生成
	if(FAILED((*m_pD3DDevice)->CreateIndexBuffer(sizeof(WORD) * m_nNumVertexIndex	// 総インデックス数
												, D3DUSAGE_WRITEONLY
												, D3DFMT_INDEX16
												, D3DPOOL_MANAGED
												, &m_pD3DIndexBuff
												, NULL)))
	{
		return E_FAIL;
	}

	float posY;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);		// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	// 頂点の設定(内側に向かって描画したいので普段と逆)
	for(int i = 0, count = 0;i < m_nNumVtxNum; i++)
	{
		posY = (float)(DistanceZVertex * count) * cosf(count * D3DX_PI / CULC_POS_Y_COEFFICIENT / (float)m_nNumHeightBlock);
		if(i == m_nNumVtxNum - 1 - m_nNumWidthBlock)
		{
			count = m_nNumHeightBlock - 1;
			posY = (float)(DistanceZVertex * (count)) * cosf((count) * D3DX_PI / CULC_POS_Y_COEFFICIENT / (float)m_nNumHeightBlock);
		}

		pVtx[i].vtx = D3DXVECTOR3(r * cosf(nNumDig)			// (半径 * 減衰率) * cos
								, posY
								, (r * sinf(nNumDig)));
		// 角度増加
		nNumDig += PlusDig;

		// 横一列やったら
		if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
		{
			// 行番号増加
			count++;

			// 角度を戻す
			nNumDig = 0;
										// 90度がほしいので半分に
			r = m_fRadius * cosf(count * D3DX_PI * 0.5f / (float)m_nNumHeightBlock);
		}
	}

	for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
	{
		// テクスチャの設定(内側に向かって描画したいので普段と逆)
		// マイナス1にならないようにする
		if(tex_X < MIN_UV_VALUE)
		{
			tex_X = MAX_UV_VALUE;
		}

		// １列やったら
		if(i - 1 == m_nNumWidthBlock + count * (m_nNumWidthBlock + 1))
		{
			// 行番号増加
			count++;

			// テクスチャＸをもとにもどす
			tex_X = MAX_UV_VALUE;
		
			// テクスチャＹを変更
			tex_Y -= Add_tex_Y;

			// マイナス1にならないようにする
			if(tex_Y < MIN_UV_VALUE)
			{
				tex_Y = MIN_UV_VALUE;
			}
		}

		if(tex_Y == MIN_UV_VALUE)
		{
			tex_Y = MIN_TEX_POS;
			tex_X = MIN_TEX_POS;
		}

		// テクスチャ設定
		pVtx[i].tex = D3DXVECTOR2(tex_X, tex_Y);

		// テクスチャＸ座標を増やす
		tex_X += Add_tex_X;

		// 法線の設定
		pVtx[i].nor = DEFAULT_UP_VECTOR;

		// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
		pVtx[i].diffuse = DEFAULT_POLYGON_COLOR;
	}
	
	m_pD3DVtxBuff->Unlock();																	//ロックしたら必ずアンロック！！
	m_pD3DTexBuff = CTexture::GetTexture( texType );
	//********************************************************************************************************************
	WORD *pIndex;
	// ロック
	m_pD3DIndexBuff->Lock(0, 0, (void**)&pIndex, 0);
	
	// インデックスの設定																	// 2個づつみているので+2
	for(int i = 0, first = wblock + 1, second = 0, counter = 0 ; i < m_nNumVertexIndex - 1 ; i += 2, first++, second++)
	{
		// 横に見たときに１づつ増えていくことを利用
		pIndex[i] = first;
		pIndex[i+1] = second;

		if(counter < hblock - 1)
		{
			// 折り返しの処理
			if(second == wblock + (wblock + 1) * counter)
			{
				i += 2;	// 2個づつみているので+2
				pIndex[i] = second;
				pIndex[i+1] = first + 1;

				// 折り返し数カウントアップ
				counter++;
			}
		}
	}

	// アンロック
	m_pD3DIndexBuff->Unlock();


	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	CScene::AddLinkList(CRenderer::TYPE_RENDER_BASE_HIGH_LUMINANCE);
	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneMeshSky ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	m_pD3DTexBuff = NULL;

	if(m_pD3DIndexBuff)
	{
		m_pD3DIndexBuff->Release();
		m_pD3DIndexBuff = NULL;
	}

	SAFE_RELEASE(m_pDecl);

	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneMeshSky ::Update(void)
{
	m_vRot.y += DEFAULT_ROT_SPD;
	NormalizeRotation(&m_vRot.y);
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneMeshSky::DrawNormalRender(void)
{
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映		*****順番注意*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.y, m_vRot.x, m_vRot.z);	// Y軸を基点に回転しているのでY,X,Zの順に入れる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// シェーダーの適用
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_TEX);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_TEX);

	PS_TYPE type = PS_TYPE_TEX;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

	HRESULT hr = D3D_OK;
	UINT texSamplerIndex = (*_psc)->GetSamplerIndex("texSampler");

	hr = (*m_pD3DDevice)->SetSamplerState(texSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	D3DXMATRIX view, proj, wvp;
	view = m_pManager->GetCameraManager()->GetMtxView();
	proj = m_pManager->GetCameraManager()->GetMtxProj();
	wvp = m_mtxWorld * view * proj;

	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gWVP", &wvp);	// こいつらをあらかじめ掛け合わせてシェーダーに送るとコスト削減可

	// 頂点宣言したやつをセット(SetFVFの代わり)
	(*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);

	// ポリゴンの描画
	hr = (*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);											// 頂点フォーマットの設定
	hr = (*m_pD3DDevice)->SetTexture(texSamplerIndex, m_pD3DTexBuff);
	hr = (*m_pD3DDevice)->SetVertexShader(*_vs);
	hr = (*m_pD3DDevice)->SetPixelShader(*_ps);
	
	// ポリゴンの描画
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VF));		// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetIndices(m_pD3DIndexBuff);							// インデックスセット
	(*m_pD3DDevice)->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP				// プリミティブの種類
											,0
											,0
											,m_nNumVtxNum					// 総頂点数
											,0
											,m_nNumPolygon);

	hr = (*m_pD3DDevice)->SetTexture(texSamplerIndex, NULL);
	hr = (*m_pD3DDevice)->SetVertexShader(NULL);
	hr = (*m_pD3DDevice)->SetPixelShader(NULL);
}

//*****************************************************************************
// 高輝度抽出したい奴描画
//*****************************************************************************
void CSceneMeshSky::DrawBaseHighLuminanceRender(void)
{
	DrawNormalRender();
}

//*****************************************************************************
// ポリゴンセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneMeshSky :: SetVertexPolygon(void)
{
	VF	*pVtx;
	int lineX = 0, lineZ = 0;
	float DistanceXVertex,DistanceZVertex; // 頂点間の距離
	DistanceXVertex = m_fRadius / m_nNumWidthBlock;
	DistanceZVertex = m_fHeight / m_nNumHeightBlock;

	// 頂点情報の設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum; i++)
	{
		// 頂点の設定
		for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
		{
			pVtx[i].vtx = D3DXVECTOR3((m_vPos.x - m_fRadius * 0.5f) + lineX	// 中心からの距離なので半分に
									, 0.0f
									, (m_vPos.z + m_fHeight * 0.5f) - lineZ);

			lineX += (int)DistanceXVertex;
			if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
			{
				lineX = 0;
				lineZ += (int)DistanceZVertex;
				count++;
			}
		}
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneMeshSky::SetColorPolygon(D3DXCOLOR color)
{
	VF *pVtx;
	int lineX = 0, lineZ = 0;

	// 頂点情報の設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum ; i++)
	{
		pVtx[i].diffuse = color;
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneMeshSky* CSceneMeshSky::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float r, float height, int wblock, int hblock, TEXTURE_TYPE texType, CManager* pManager, D3DXVECTOR3 rot)
{
	// 作成
	CSceneMeshSky* p = new CSceneMeshSky(pDevice);

	// 初期化
	p->Init(pos, r, height, texType, wblock, hblock);
	p->m_pManager = pManager;
	p->SetRot(rot);

	return p;
}
//----EOF----