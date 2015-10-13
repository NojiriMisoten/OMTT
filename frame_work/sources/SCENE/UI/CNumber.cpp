//=============================================================================
//
// CNumberクラス [CNumber.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CNumber.h"
#include "../../CONST/const.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const float NUMBER_WIDTH_TEX = 0.1f;		// １つの数字のテクスチャの横幅
static const int MAX_NUMBER_NUM = 10;			// 数字の数

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CNumber ::CNumber(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CNumber ::~CNumber(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CNumber :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	VERTEX_2D	*pVtx;
	
	// 原点の位置
	m_vPos = pos;

	// 幅と高さの設定
	m_fWidth = width;
	m_fHeight = height;
	
	// ポリゴンのメモリ確保
	(*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D) * 4		// 四角形の頂点数分確保
										, D3DUSAGE_WRITEONLY		// 変えない
										, FVF_VERTEX_2D				// 使用する頂点フォーマット
										, D3DPOOL_MANAGED			// 変えない
										, &m_pD3DVtxBuff			// 格納する場所
										, NULL);					// 変えない
	
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					//ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f		// 中心からの距離なので *0.5
							, m_vPos.y - m_fHeight * 0.5f
							, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f
							, m_vPos.y - m_fHeight * 0.5f
							, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f
							, m_vPos.y + m_fHeight * 0.5f
							, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f
							, m_vPos.y + m_fHeight * 0.5f
							, 0.0f);
	//--------------------------------------------------------------------------------
	//中身は常に1.0f
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//-	-------------------------------------------------------------------------------
	//色と透明度の設定（テクスチャを張るのでポリゴンは白で）
	pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
	//--------------------------------------------------------------------------------
	//使用したいテクスチャの座標(0～1)
	pVtx[0].tex = D3DXVECTOR2(MIN_UV_VALUE, MIN_UV_VALUE);
	pVtx[1].tex = D3DXVECTOR2(NUMBER_WIDTH_TEX, MIN_UV_VALUE);
	pVtx[2].tex = D3DXVECTOR2(MIN_UV_VALUE, MAX_UV_VALUE);
	pVtx[3].tex = D3DXVECTOR2(NUMBER_WIDTH_TEX, MAX_UV_VALUE);
	//----------------------------------------------------------------------------------
	m_pD3DVtxBuff->Unlock();									//ロックしたら必ずアンロック！！
	//********************************************************************************************************************
	m_pD3DTexBuff = CTexture::GetTexture( texType );
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CNumber ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}
	if(m_pD3DTexBuff)
	{
		m_pD3DTexBuff = NULL;
	}
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CNumber ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CNumber ::Draw(void)
{
	// ポリゴンの描画
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_2D));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);										// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);								// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP							// プリミティブの種類
								, 0												// 描画を開始する頂点番号
								, DRAW_SQUARE_PRINITIV_NUM);					// 書きたいポリゴン数(三角形基準・・・四角形作るならポリゴン数２）
}

//*****************************************************************************
// 数字セット関数
//*****************************************************************************
void CNumber :: SetNumber(int number)
{
	float leftX,rightX;
		
	// 数字割出処理
	rightX = (float)((float)number / (float)MAX_NUMBER_NUM) + NUMBER_WIDTH_TEX;
	leftX = (float)((float)number / (float)MAX_NUMBER_NUM);

	VERTEX_2D	*pVtx;

	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);		//ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	
	// 使用したいテクスチャの座標(0～1)
	pVtx[0].tex = D3DXVECTOR2(leftX, MIN_UV_VALUE);
	pVtx[1].tex = D3DXVECTOR2(rightX, MIN_UV_VALUE);
	pVtx[2].tex = D3DXVECTOR2(leftX, MAX_UV_VALUE);
	pVtx[3].tex = D3DXVECTOR2(rightX, MAX_UV_VALUE);
	
	m_pD3DVtxBuff->Unlock();							//ロックしたら必ずアンロック！！
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CNumber* CNumber::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	// 作成
	CNumber* p = new CNumber(pDevice);

	// 初期化
	p->Init(pos, width, height, texType);

	return p;
	
}
//----EOF----