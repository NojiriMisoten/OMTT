//=============================================================================
//
// CNowLoadingクラス [CNowLoading.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CNowLoading.h"
#include "../../MATH/math.h"
#include "../../CONST/const.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const float			DEFAULT_CHAR_SIZE = 75.f;											//文字サイズ
static const D3DXVECTOR3	INIT_CHAR_POS = D3DXVECTOR3(50.f, (float)SCREEN_WIDTH * 0.5f, 0.f);	// 文字初期座標
static const float			MOV_POW = D3DX_PI / 120.f;											// 移動量
static const float			MOV_WIDTH = 0.4f;													// 移動幅
static const float			CULC_INIT_MOVE_COUNTER_COEFFICIENT = 15.f;							// moveCounterの初期値計算用係数

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CNowLoading::CNowLoading(void)
{
	m_pD3DDevice = NULL;
	m_pD3DTexBuff = NULL;
	for(int index = 0; index < MAX_CHAR_NUM; ++index)
	{
		m_vCharPos[index] = D3DXVECTOR3(0,0,0);
		m_pD3DVtxBuff[index] = NULL;
		m_fMoveCounter[index] = (float)index * MOV_POW * CULC_INIT_MOVE_COUNTER_COEFFICIENT;
	}
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CNowLoading::~CNowLoading(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CNowLoading::Init(LPDIRECT3DDEVICE9* pDevice)
{
	VERTEX_2D	*pVtx;
	m_pD3DDevice = pDevice;
	float width = DEFAULT_CHAR_SIZE;
	float height = DEFAULT_CHAR_SIZE;
	float texX = 0.f;
	float movTexX = MAX_UV_VALUE / (float)MAX_CHAR_NUM;

	// テクスチャ読み込み
	if( FAILED( D3DXCreateTextureFromFile((*pDevice), "data/TEXTURE/NowLoading.png", &m_pD3DTexBuff)))
	{
		#ifdef _DEBUG
		MessageBox( NULL, "テクスチャの読み込みに失敗しました\n" , "エラー", MB_OK | MB_ICONERROR );
		#endif
		m_pD3DTexBuff = NULL;
	}

	for(int index = 0; index < MAX_CHAR_NUM; ++index)
	{
		// ポリゴンのメモリ確保
		if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// 四角形の頂点数分確保
												D3DUSAGE_WRITEONLY,				// 変えない
												FVF_VERTEX_2D,					// 使用する頂点フォーマット
												D3DPOOL_MANAGED,				// 変えない
												&m_pD3DVtxBuff[index],			// 格納する場所
												NULL)))							// 変えない
		{
			return;																// メモリを確保できなかったらメインに返す
		}

		// 初期座標の設定
		m_vCharPos[index] = INIT_CHAR_POS;
		m_vCharPos[index].x += index * DEFAULT_CHAR_SIZE;
		m_vCharPos[index].y += MOV_WIDTH * sinf(m_fMoveCounter[index]);

		//********************************************************************************************************************
		// ポリゴンの設定
		m_pD3DVtxBuff[index]->Lock(0, 0, (void**)&pVtx, 0);				//ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
		//--------------------------------------------------------------------------------
		// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
		pVtx[0].vtx = D3DXVECTOR3(m_vCharPos[index].x - width * 0.5f		// 中心からの距離なので0.5倍している
								, m_vCharPos[index].y - height * 0.5f
								, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(m_vCharPos[index].x + width * 0.5f
								, m_vCharPos[index].y - height * 0.5f
								, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(m_vCharPos[index].x - width * 0.5f
								, m_vCharPos[index].y + height * 0.5f
								, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(m_vCharPos[index].x + width* 0.5f
								, m_vCharPos[index].y + height* 0.5f
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
		// 使用したいテクスチャの座標(0～1)
		pVtx[0].tex = D3DXVECTOR2(texX, MIN_ALPHA);
		pVtx[1].tex = D3DXVECTOR2(texX + movTexX, MIN_ALPHA);
		pVtx[2].tex = D3DXVECTOR2(texX, MAX_ALPHA);
		pVtx[3].tex = D3DXVECTOR2(texX + movTexX, MAX_ALPHA);
		//----------------------------------------------------------------------------------
		m_pD3DVtxBuff[index]->Unlock();							// ロックしたら必ずアンロック！！
		//********************************************************************************************************************

		// UV値変更
		texX += movTexX;
	}
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CNowLoading::Uninit(void)
{
	if(m_pD3DTexBuff)
	{
		m_pD3DTexBuff->Release();
		m_pD3DTexBuff = NULL;
	}
	for(int index = 0; index < MAX_CHAR_NUM; ++index)
	{
		if(m_pD3DVtxBuff[index])
		{
			m_pD3DVtxBuff[index]->Release();
			m_pD3DVtxBuff[index] = NULL;
		}
	}
}


//*****************************************************************************
// 更新
//*****************************************************************************
void CNowLoading::Update(void)
{
	VERTEX_2D	*pVtx;
	float width = DEFAULT_CHAR_SIZE;
	float height = DEFAULT_CHAR_SIZE;

	// 文字の移動
	for(int index = 0; index < MAX_CHAR_NUM; ++index)
	{
		m_fMoveCounter[index] += MOV_POW;
		NormalizeRotation(&m_fMoveCounter[index]);
		m_vCharPos[index].y += MOV_WIDTH * sinf(m_fMoveCounter[index]);
		
		m_pD3DVtxBuff[index]->Lock(0, 0, (void**)&pVtx, 0);				//ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
		// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
		pVtx[0].vtx = D3DXVECTOR3(m_vCharPos[index].x - width * 0.5f	// 中心からの距離なので0.5倍している
								, m_vCharPos[index].y - height * 0.5f
								, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(m_vCharPos[index].x + width * 0.5f
								, m_vCharPos[index].y - height * 0.5f
								, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(m_vCharPos[index].x - width * 0.5f
								, m_vCharPos[index].y + height * 0.5f
								, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(m_vCharPos[index].x + width* 0.5f
								, m_vCharPos[index].y + height* 0.5f
								, 0.0f);
		m_pD3DVtxBuff[index]->Unlock();									//ロックしたら必ずアンロック！！
	}
}

//*****************************************************************************
// 描画
//*****************************************************************************
void CNowLoading::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	(*m_pD3DDevice)->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), DEFAULT_POLYGON_COLOR, 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED((*m_pD3DDevice)->BeginScene()))
	{
		// ポリゴンの描画
		for(int index = 0; index < MAX_CHAR_NUM; ++index)
		{
			(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff[index], 0, sizeof(VERTEX_2D));	// (0,渡すものが入ってるやつ,0,データの型指定)
			(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);												// 頂点フォーマットの設定
			(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);										// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
			(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP									// プリミティブの種類
											, 0													// 描画を開始する頂点番号
											, DRAW_SQUARE_PRINITIV_NUM);						// 書きたいポリゴン数(三角形基準・・・四角形作るならポリゴン数２）
		}
		// 描画の終了
		(*m_pD3DDevice)->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	(*m_pD3DDevice)->Present(NULL, NULL, NULL, NULL);
}
//----EOF----