//=============================================================================
//
// CRopeクラス [CRope.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CRope.h"
#include "../../../MANAGER/CManager.h"
#include "../../../RENDERER/CRenderer.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// ゴムの抵抗 小さくすると固いゴムになる
//static const float GOMU_RESIST = 0.85f;
static const float GOMU_RESIST = 0.6f;
// 引っ張ったときその周囲にどのくらい反映するか 2~3ぐらい
static const float AROUND_RESIST = 2.0f;
// 引っ張る力の慣性 これでゴムを引っ張ている力を弱められる
static const float PULL_RESIST = 0.92f;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CRope::CRope(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) :CScene(objType)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_pManager = NULL;
	m_PosMax = 0;
	m_PosMin = 0;
	m_PullPower = 0;
	m_PullInterval = 0;
	m_PullCount = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CRope ::~CRope(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CRope::Init(D3DXVECTOR3& pos, float width, float height, TEXTURE_TYPE texType, CManager* pManager, int wblock, int hblock)
{
	m_pManager = pManager;

	m_PosMin = pos.y;
	m_PosMax = 0;

	VF  *pVtx;
	float DistanceXVertex, DistanceZVertex; // 頂点間の距離
	float lineX = 0;
	float lineZ = 0;
	float tex_X = 0;
	float tex_Y = 0;
	int counter = 0;
	int i;
	float Add_tex_X = 1.f;
	float Add_tex_Y = 1.f;
	m_nNumWidthBlock = wblock;
	m_nNumHeightBlock = hblock;
	m_fWidth = width;
	m_fHeight = height;
	m_fGridWidth = m_fWidth / m_nNumWidthBlock;
	m_fGridHeight = m_fHeight / m_nNumHeightBlock;
	Add_tex_X = 1.f / (float)(m_nNumWidthBlock);
	Add_tex_Y = 1.f / (float)(m_nNumHeightBlock);

	// 総ポリゴン数の計算             ↓四角形は三角形２個なので
	m_nNumPolygon = wblock * hblock * 2 + ((hblock - 1) * 4);	// +~の部分は縮退ポリゴン分

	// 総インデックス数の計算
	m_nNumVertexIndex = 3 + (m_nNumPolygon - 1);	// 最初の１個以降は１頂点プラスでポリゴンが書ける

	// 頂点間の距離算出
	DistanceXVertex = m_fGridWidth;
	DistanceZVertex = m_fGridHeight;

	//原点の位置
	m_Pos = pos;

	// 頂点の数を設定
	m_nNumVtxNum = (wblock + 1) * (hblock + 1);

	// 面法線バッファ作成
	m_vFaceNormalBuf = new D3DXVECTOR3[wblock * hblock * 2];
	for (int i = 0; i < wblock * hblock * 2; i++)
	{
		m_vFaceNormalBuf[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 頂点法線バッファの作成
	m_vVtxNormalBuf = new D3DXVECTOR3[m_nNumVtxNum];
	for (int i = 0; i < m_nNumVtxNum; i++)
	{
		m_vVtxNormalBuf[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// ポリゴンのメモリ確保
	if (FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VF) * m_nNumVtxNum	// VERTEX_2Dの型で確保
		, D3DUSAGE_WRITEONLY				// 変えない
		, 0						// 使用する頂点フォーマット
		, D3DPOOL_MANAGED					// 変えない
		, &m_pD3DVtxBuff					// 格納する場所
		, NULL)))							// 変えない
	{
		return;																// メモリを確保できなかったらメインに返す
	}

	// インデックスバッファの生成
	if (FAILED((*m_pD3DDevice)->CreateIndexBuffer(sizeof(WORD) * m_nNumVertexIndex	// 総インデックス数
		, D3DUSAGE_WRITEONLY
		, D3DFMT_INDEX16
		, D3DPOOL_MANAGED
		, &m_pD3DIndexBuff
		, NULL)))
	{
		return;
	}

	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	// 頂点の設定
	for (int i = 0, count = 0; i < m_nNumVtxNum; i++)
	{
		pVtx[i].vtx = D3DXVECTOR3((-width * 0.5f) + lineX		// 中心からの距離なので半分に
			, pos.y
			, (height * 0.5f) - lineZ);

		pVtx[i].nor = D3DXVECTOR3(0.f, 0.f, 0.f);

		// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
		pVtx[i].diffuse = DEFAULT_POLYGON_COLOR;

		lineX += DistanceXVertex;
		if (i == wblock + (wblock + 1) * count)
		{
			lineX = 0;
			lineZ += DistanceZVertex;
			count++;
		}
		if (i == 0)
		{
			m_vCornerPos[0] = pVtx[i].vtx;
			m_vCornerPos[0].x += m_Pos.x;
			m_vCornerPos[0].y += m_Pos.y;
			m_vCornerPos[0].z += m_Pos.z;
		}

		if (i == wblock)
		{
			m_vCornerPos[1] = pVtx[i].vtx;
			m_vCornerPos[1].x += m_Pos.x;
			m_vCornerPos[1].y += m_Pos.y;
			m_vCornerPos[1].z += m_Pos.z;
		}

		if (i == m_nNumVtxNum - 1)
		{
			m_vCornerPos[2] = pVtx[i].vtx;
			m_vCornerPos[2].x += m_Pos.x;
			m_vCornerPos[2].y += m_Pos.y;
			m_vCornerPos[2].z += m_Pos.z;
		}

		if (i == m_nNumVtxNum - wblock - 1)
		{
			m_vCornerPos[3] = pVtx[i].vtx;
			m_vCornerPos[3].x += m_Pos.x;
			m_vCornerPos[3].y += m_Pos.y;
			m_vCornerPos[3].z += m_Pos.z;
		}
	}

	// 面法線の算出
	int faceNormalCounter = 0, hblockCounter = 0, wblockCounter = 0, vtxIndexOne = 1, vtxIndexSecond = wblock + 1;
	D3DXVECTOR3 norOne, norSecond, vecOneA, vecOneB, vecSecondA, vecSecondB;

	// 縦のループ
	for (hblockCounter = 0; hblockCounter < hblock; hblockCounter++)
	{
		// 横のループ
		for (wblockCounter = 0; wblockCounter < wblock; wblockCounter++, vtxIndexOne++, vtxIndexSecond++)
		{
			// 横に見ていき２個ずつ作る
			// １つ目
			vecOneA = pVtx[vtxIndexSecond - (wblock + 1)].vtx - pVtx[vtxIndexSecond].vtx;
			vecOneB = pVtx[vtxIndexSecond + 1].vtx - pVtx[vtxIndexSecond].vtx;
			D3DXVec3Cross(&norOne, &vecOneA, &vecOneB);
			D3DXVec3Normalize(&norOne, &norOne);
			m_vFaceNormalBuf[faceNormalCounter] = norOne;

			// 頂点の法線の設定
			pVtx[vtxIndexSecond - (wblock + 1)].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexSecond - (wblock + 1)].nor, &pVtx[vtxIndexSecond - (wblock + 1)].nor);

			pVtx[vtxIndexSecond].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexSecond].nor, &pVtx[vtxIndexSecond].nor);

			pVtx[vtxIndexSecond + 1].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexSecond + 1].nor, &pVtx[vtxIndexSecond + 1].nor);

			faceNormalCounter++;

			// ２つ目
			vecSecondA = pVtx[vtxIndexOne + (wblock + 1)].vtx - pVtx[vtxIndexOne].vtx;
			vecSecondB = pVtx[vtxIndexOne - 1].vtx - pVtx[vtxIndexOne].vtx;
			D3DXVec3Cross(&norSecond, &vecSecondA, &vecSecondB);
			D3DXVec3Normalize(&norSecond, &norSecond);
			m_vFaceNormalBuf[faceNormalCounter] = norSecond;

			// 頂点の法線の設定
			pVtx[vtxIndexOne + (wblock + 1)].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexOne + (wblock + 1)].nor, &pVtx[vtxIndexOne + (wblock + 1)].nor);

			pVtx[vtxIndexOne].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexOne].nor, &pVtx[vtxIndexOne].nor);

			pVtx[vtxIndexOne - 1].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexOne - 1].nor, &pVtx[vtxIndexOne - 1].nor);

			faceNormalCounter++;
		}
		vtxIndexOne++;
		vtxIndexSecond++;
	}

	// 使用したいテクスチャの座標
	for (i = 0; i < m_nNumVtxNum; i++)
	{
		if (i != 0)
		{
			if (pVtx[i].vtx.x != pVtx[i - 1].vtx.x)
			{
				tex_X += Add_tex_X;
				// 1を超えないようにする
				if (tex_X > 1.f)
				{
					tex_X = 1.f;
				}
			}

			// １列やったら
			//if (i - 1 == wblock + counter * (wblock + 1))
			if (pVtx[i].vtx.z != pVtx[i - 1].vtx.z)
			{
				counter++;

				tex_Y += Add_tex_Y;
				tex_X = 0.f;
				if (tex_Y > 1.f)
				{
					tex_Y = 1.f;
				}
			}
		}

		// テクスチャ設定
		pVtx[i].tex = D3DXVECTOR2((float)tex_X, (float)tex_Y);
	}

	m_pD3DVtxBuff->Unlock();												// ロックしたら必ずアンロック！！
	//********************************************************************************************************************

	WORD *pIndex;
	// ロック
	m_pD3DIndexBuff->Lock(0, 0, (void**)&pIndex, 0);

	// インデックスの設定																	2つづつ見てるので２つづつ進む
	for (int i = 0, first = wblock + 1, second = 0, counter = 0; i < m_nNumVertexIndex - 1; i += 2, first++, second++)
	{
		// 横に見たときに１づつ増えていくことを利用
		pIndex[i] = first;
		pIndex[i + 1] = second;

		if (counter < hblock - 1)
		{
			// 折り返しの処理
			if (second == wblock + (wblock + 1) * counter)
			{
				i += 2;	// 2つづつ見てるので２つづつ進む
				pIndex[i] = second;
				pIndex[i + 1] = first + 1;

				// 折り返し数カウントアップ
				counter++;
			}
		}
	}

	// アンロック
	m_pD3DIndexBuff->Unlock();

	// 頂点宣言したものを作る
	(*m_pD3DDevice)->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	// リストに追加
	CScene::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// テクスチャ設定
	m_pD3DTexBuff = CTexture::GetTexture(texType);
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CRope::Uninit(void)
{
	if (m_vFaceNormalBuf)
	{
		delete[] m_vFaceNormalBuf;
		m_vFaceNormalBuf = NULL;
	}

	if (m_vVtxNormalBuf)
	{
		delete[] m_vVtxNormalBuf;
		m_vVtxNormalBuf = NULL;
	}

	SAFE_RELEASE(m_pD3DIndexBuff);
	m_pD3DIndexBuff = NULL;
	SAFE_RELEASE(m_pD3DVtxBuff);
	m_pD3DTexBuff = NULL;
	SAFE_RELEASE(m_pDecl);
	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CRope::Update(void)
{
	// 引っ張る力の時間管理
	if (m_PullCount < m_PullInterval)
	{
		m_PullCount++;

		// 引っ張る力
		m_PosMax += m_PullPower;

		// 引っ張る力の慣性
		m_PullPower *= PULL_RESIST;
	}
	else
	{
		// ゴムの慣性
		m_PosMax *= GOMU_RESIST;
	}

	VF *pVtx;
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 分割数の半分
	int numBlockHalf = static_cast<int>(m_nNumWidthBlock * 0.5f);
	for (int i = 0; i < m_nNumVtxNum; i++)
	{
		// 最初の折り返すときと折り返したはじめは０
		int num = i % (m_nNumWidthBlock+1);
		// 端が0で中央の頂点に行くほど1になる計算
		float max_ratio = 1.0f -
			static_cast<float>(abs(num - numBlockHalf)) /
			static_cast<float>(numBlockHalf);
		// 二次関数みたいに緩やかにする
		max_ratio = max_ratio * (AROUND_RESIST - max_ratio) / AROUND_RESIST;

		// 中央の頂点ほどMaxの座標に近くなるようセット
		pVtx[i].vtx.y = m_PosMin + m_PosMax * max_ratio;
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CRope::DrawNormalRender(void)
{
	D3DXMATRIX			mtxScl, mtxRot, mtxTranslate, mtxWVP;

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

	// シェーダーの適用
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_TEX);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_TEX);

	PS_TYPE type = PS_TYPE_TEX;

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(type);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(type);

	(*m_pD3DDevice)->SetVertexShader(*_vs);
	(*m_pD3DDevice)->SetPixelShader(*_ps);

	UINT texSampler = (*_psc)->GetSamplerIndex("texSampler");
	HRESULT hr = 0;
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = (*m_pD3DDevice)->SetSamplerState(texSampler, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	D3DXMATRIX view, proj;
	CCameraManager* pCameraManager = m_pManager->GetCameraManager();
	view = pCameraManager->GetMtxView();
	proj = pCameraManager->GetMtxProj();

	mtxWVP = m_mtxWorld * view * proj;
	hr = (*_vsc)->SetMatrix((*m_pD3DDevice), "gWVP", &mtxWVP);

	// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);

	// ポリゴンの描画	
	hr = (*m_pD3DDevice)->SetVertexDeclaration(m_pDecl);											// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VF));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetIndices(m_pD3DIndexBuff);								// インデックスの設定
	(*m_pD3DDevice)->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP					// プリミティブの種類
		, 0
		, 0
		, m_nNumVtxNum							// 総頂点数
		, 0
		, m_nNumPolygon);
	
	(*m_pD3DDevice)->SetTexture(texSampler, NULL);
	(*m_pD3DDevice)->SetVertexShader(NULL);
	(*m_pD3DDevice)->SetPixelShader(NULL);
}

//*****************************************************************************
// ゴムアニメ―ションスタート
// 引っ張る力、その手を離すまでのフレームカウント
//*****************************************************************************
void CRope::Pull(float pullPower, int pullInterval)
{
	m_PullCount = 0;
	m_PullInterval = pullInterval;
	m_PullPower = pullPower;
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CRope* CRope::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3 pos, float width, float height,
	int wBlock, int hBlock,
	TEXTURE_TYPE texType,
	CManager* pManager)
{
	// 作成
	CRope* p = new CRope(pDevice);

	// 初期化
	p->Init(pos, width, height, texType, pManager, wBlock, hBlock);

	p->AddLinkList(CRenderer::TYPE_RENDER_NORMAL);
	return p;

}
//----EOF----