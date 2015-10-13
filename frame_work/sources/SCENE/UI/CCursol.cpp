//=============================================================================
//
// CCursolクラス [CCursol.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCursol.h"
#include "../../INPUT/CInputKeyboard.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const float DEFFAULT_MOV_ALPFA_SPD = 1.0f / 60.f;				// 通常の透明度の変化スピード

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCursol ::CCursol(LPDIRECT3DDEVICE9 *pDevice)
{
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		m_pD3DVtxBuff[idx] = NULL;
		m_pD3DTexBuff[idx] = NULL;
	}
	m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_pD3DDevice = pDevice;
	m_fAlpha = 0.f;
	m_bDraw = false;
	m_fLengthOfLeftToRight = 0.f;
	m_fMoveAlphaSign = 1.0f;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCursol ::~CCursol(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CCursol :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, float lengthOfLeftAndRight, int wblock, int hblock )
{
	VERTEX_2D	*pVtx;
	
	// 原点の位置
	m_vPos = pos;

	// 幅と高さの設定
	m_fWidth = width;
	m_fHeight = height;

	// 左から右への長さ
	m_fLengthOfLeftToRight = lengthOfLeftAndRight;

	// 左か
	bool createLeft = true;

	// 符号
	float sign = 1.f;

	// UV値の移動量
	float movTexX = MAX_UV_VALUE / MENU_CURSOL_OBJ_NUM;

	// UV値
	float leftTexX = MIN_UV_VALUE;
	float topTexY = MIN_UV_VALUE;
	float rightTexX = movTexX;
	float bottomTexY = MAX_UV_VALUE;
	
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// 左右の判断
		if(createLeft)
		{
			sign = -1.f;
		}
		else
		{
			sign = 1.f;
		}

		// ポリゴン生成の位置のオフセット
		float createPosOffset = sign * lengthOfLeftAndRight * 0.5f; // 中心からの距離なので半分に

		// ポリゴンのメモリ確保
		(*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// VERTEX_2Dの型で４頂点*表示桁数分確保
												D3DUSAGE_WRITEONLY,			// 変えない
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// 変えない
												&m_pD3DVtxBuff[idx],		// 格納する場所
												NULL);						// 変えない
		
		//********************************************************************************************************************
		// ポリゴンの設定
		m_pD3DVtxBuff[idx]->Lock(0,0,(void**)&pVtx,0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
		//--------------------------------------------------------------------------------
		// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
		pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// 中心からの距離なので *0.5
								, m_vPos.y - m_fHeight * 0.5f						// Y座標 
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y - m_fHeight * 0.5f						// Y座標 中心からの距離なので *0.5
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y + m_fHeight * 0.5f						// Y座標 中心からの距離なので *0.5
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y + m_fHeight * 0.5f						// Y座標 中心からの距離なので *0.5
								, 0.0f);
		//--------------------------------------------------------------------------------
		//中身は常に1.0f
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//--------------------------------------------------------------------------------
		//色と透明度の設定
		pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
		pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
		pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
		pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
		//--------------------------------------------------------------------------------
		//使用したいテクスチャの座標(0～1)
		pVtx[0].tex = D3DXVECTOR2(leftTexX, topTexY);
		pVtx[1].tex = D3DXVECTOR2(rightTexX, topTexY);
		pVtx[2].tex = D3DXVECTOR2(leftTexX, bottomTexY);
		pVtx[3].tex = D3DXVECTOR2(rightTexX, bottomTexY);
		//----------------------------------------------------------------------------------
		m_pD3DVtxBuff[idx]->Unlock();																	//ロックしたら必ずアンロック！！
		//********************************************************************************************************************
		m_pD3DTexBuff[idx] = CTexture::GetTexture( texType );

		// UV値移動
		leftTexX += movTexX;
		rightTexX += movTexX;

		// フラグ反転
		createLeft = !createLeft;
	}

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CCursol ::Uninit(void)
{
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		if(m_pD3DVtxBuff[idx])
		{
			m_pD3DVtxBuff[idx]->Release();
			m_pD3DVtxBuff[idx] = NULL;		// 頂点座標情報を格納する場所のアドレスを確保する場所
		}
		if(m_pD3DTexBuff[idx])
		{
			m_pD3DTexBuff[idx] = NULL;		// テクスチャ情報を格納する場所のアドレスを確保する場所
		}
	}
	m_bDraw = false;
	Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CCursol ::Update(void)
{
	if(!m_bDraw)
	{
		return;
	}
	// 透明度変更
	m_fAlpha += DEFFAULT_MOV_ALPFA_SPD * m_fMoveAlphaSign;
	
	// 限界値判定
	if(m_fAlpha > MAX_ALPHA)
	{
		m_fAlpha = MAX_ALPHA;

		// 符号反転
		m_fMoveAlphaSign *= -1.f;
	}
	else if(m_fAlpha < MIN_ALPHA)
	{
		m_fAlpha = MIN_ALPHA;

		// 符号反転
		m_fMoveAlphaSign *= -1.f;
	}

	// 色変更
	SetColorPolygon(D3DXCOLOR(DEFAULT_POLYGON_COLOR.r
					, DEFAULT_POLYGON_COLOR.g
					, DEFAULT_POLYGON_COLOR.b
					, m_fAlpha));
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CCursol ::Draw(void)
{
	// 描画しないなら何もしない
	if(!m_bDraw)
	{
		return;
	}

	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULLチェック
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}

		// ポリゴンの描画
		(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff[idx] ,0, sizeof(VERTEX_2D));	// (0,渡すものが入ってるやつ,0,データの型指定)
		(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);											// 頂点フォーマットの設定
		(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff[idx]);								// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
		(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP								// プリミティブの種類
									, 0													// 描画を開始する頂点番号
									, DRAW_SQUARE_PRINITIV_NUM);						// 書きたいポリゴン数(三角形基準・・・四角形作るならポリゴン数２）
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
CCursol* CCursol::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, float lengthOfLeftAndRight)
{
	CCursol* p = new CCursol(pDevice);

	// 初期化
	p->Init(pos, width, height, texType, lengthOfLeftAndRight);

	return p;
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void CCursol::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_2D	*pVtx;
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULLチェック
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}
		
		//ポリゴンの設定
		m_pD3DVtxBuff[idx]->Lock(0,0,(void**)&pVtx,0);							//ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
		
		//色と透明度の設定（テクスチャを張るのでポリゴンは白で）
		pVtx[0].diffuse = color;
		pVtx[1].diffuse = color;
		pVtx[2].diffuse = color;
		pVtx[3].diffuse = color;
		m_pD3DVtxBuff[idx]->Unlock();											//ロックしたら必ずアンロック！！
	}
}

//*****************************************************************************
// ポリゴンセット関数  更新で呼んだりする
//*****************************************************************************
void CCursol :: SetVertexPolygon(D3DXVECTOR3 pos, float width, float height)
{
	// 座標更新
	m_vPos = pos;

	// 大きさ更新
	m_fWidth = width;
	m_fHeight = height;

	// 左か
	bool createLeft = true;

	// 符号
	float sign = 1.f;

	VERTEX_2D	*pVtx;
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULLチェック
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}

		// 左右の判断
		if(createLeft)
		{
			sign = -1.f;
		}
		else
		{
			sign = 1.f;
		}

		// ポリゴン生成の位置のオフセット
		float createPosOffset = sign * m_fLengthOfLeftToRight * 0.5f; // 中心からの距離なので半分に

		// ポリゴンの設定
		m_pD3DVtxBuff[idx]->Lock(0,0,(void**)&pVtx,0);								// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)

		// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
		pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y - m_fHeight * 0.5f						// Y座標
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y - m_fHeight * 0.5f						// Y座標
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y + m_fHeight * 0.5f						// Y座標
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y + m_fHeight * 0.5f						// Y座標
								, 0.0f);
		m_pD3DVtxBuff[idx]->Unlock();												// ロックしたら必ずアンロック！！
		
		// フラグ反転
		createLeft = !createLeft;
	}
}

//*****************************************************************************
// 座標セット関数
//*****************************************************************************
void CCursol :: SetPos(D3DXVECTOR3 pos)
{
	// 座標更新
	m_vPos = pos;

	// 左か
	bool createLeft = true;

	// 符号
	float sign = 1.f;

	VERTEX_2D	*pVtx;
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULLチェック
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}

		// 左右の判断
		if(createLeft)
		{
			sign = -1.f;
		}
		else
		{
			sign = 1.f;
		}

		// ポリゴン生成の位置のオフセット
		float createPosOffset = sign * m_fLengthOfLeftToRight * 0.5f; // 中心からの距離なので半分に

		// ポリゴンの設定
		m_pD3DVtxBuff[idx]->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)

		// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
		pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y - m_fHeight * 0.5f						// Y座標
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y - m_fHeight * 0.5f						// Y座標
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y + m_fHeight * 0.5f						// Y座標
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y + m_fHeight * 0.5f						// Y座標
								, 0.0f);
		m_pD3DVtxBuff[idx]->Unlock();												// ロックしたら必ずアンロック！！
		
		// フラグ反転
		createLeft = !createLeft;
	}
}

//*****************************************************************************
// 座標セット関数
//*****************************************************************************
void CCursol :: SetPos(float x, float y, float z)
{
	// 座標更新
	m_vPos.x = x;
	m_vPos.y = y;
	m_vPos.z = z;

	// 左か
	bool createLeft = true;

	// 符号
	float sign = 1.f;

	VERTEX_2D	*pVtx;
	for(int idx = 0; idx < MENU_CURSOL_OBJ_NUM; ++idx)
	{
		// NULLチェック
		if(!m_pD3DVtxBuff[idx])
		{
			continue;
		}

		// 左右の判断
		if(createLeft)
		{
			sign = -1.f;
		}
		else
		{
			sign = 1.f;
		}

		// ポリゴン生成の位置のオフセット
		float createPosOffset = sign * m_fLengthOfLeftToRight * 0.5f;	// 中心からの距離なので半分に

		// ポリゴンの設定
		m_pD3DVtxBuff[idx]->Lock(0, 0, (void**)&pVtx, 0);							// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
		
		// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
		pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y - m_fHeight * 0.5f						// Y座標
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y - m_fHeight * 0.5f						// Y座標
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y + m_fHeight * 0.5f						// Y座標
								, 0.0f);											// ２Ｄのときはz座標は必ず0.0
		pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fWidth * 0.5f + createPosOffset		// X座標
								, m_vPos.y + m_fHeight * 0.5f						// Y座標
								, 0.0f);
		m_pD3DVtxBuff[idx]->Unlock();												// ロックしたら必ずアンロック！！
		
		// フラグ反転
		createLeft = !createLeft;
	}
}
//----EOF----