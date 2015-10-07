//=============================================================================
//
// CFadeクラス [CFade.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../FADE/CFade.h"
#include "../TEXTURE/CTexture.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const D3DXVECTOR3	DEFFAULT_FADE_POS(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);	// デフォルトのフェードの初期座標

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
FADE_MODE	CFade::m_mode = MODE_FADE_NULL;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CFade ::CFade(LPDIRECT3DDEVICE9 *pDevice):CScene2D(pDevice)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CFade ::~CFade(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CFade :: Init(TEXTURE_TYPE texType)
{
	D3DXVECTOR3 pos(DEFFAULT_FADE_POS);
	CScene2D::Init(pos, (float)DEFFAULT_FADE_WIDTH, (float)DEFFAULT_FADE_HEIGHT, texType);
	
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 色
	m_nEndTime = 0;										// 終了時間
	m_bFade = false;									// フェードフラグ
	m_mode = MODE_FADE_NULL;							// 状態
	m_fMovAlpha = 0.0f;									// 透明度(速度)

	// フェード用のリストに追加
	CScene::AddLinkList(CRenderer::TYPE_RENDER_FADE);
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CFade ::Uninit(void)
{
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 色
	m_nEndTime = 0;										// 終了時間
	m_bFade = false;									// フェードフラグ
	m_mode = MODE_FADE_NULL;							// 状態
	m_fMovAlpha = 0.0f;									// 透明度(速度)

	CScene2D::Uninit();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CFade ::Update(void)
{
	if(m_bFade)
	{
		switch (m_mode)
		{
			// フェードインの処理
			case MODE_FADE_IN:
				
				// 徐々に濃く
				m_color.a -= m_fMovAlpha;

				// 透明度が0未満になったら
				if(m_color.a < 0.0f)
				{
					m_color.a = 0.0f;

					// フェードイン終了
					m_mode = MODE_FADE_IN_END;
				}

				// 色セット
				SetColorPolygon(m_color);
			break;

			// フェードインの終了処理
			case MODE_FADE_IN_END:
				m_bFade = false;
			break;

			// フェードアウトの処理
			case MODE_FADE_OUT:
				// 徐々に薄く
				m_color.a += m_fMovAlpha;

				// 透明度がマックスになったら
				if(m_color.a > MAX_ALPHA)
				{
					m_color.a = MAX_ALPHA;

					// フェードアウト終了
					m_mode = MODE_FADE_OUT_END;
				}
				// 色セット
				SetColorPolygon(m_color);
			break;

			// フェードアウトの終了処理
			case MODE_FADE_OUT_END:
				m_bFade = false;
			break;

			// フェードインフィニティの処理
			case MODE_FADE_INFINITY:
				// 色セット
				SetColorPolygon(m_color);
			break;
		}
	}
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CFade::DrawFadeRender(void)
{
	if(m_bFade)
	{
		// ポリゴンの描画
		(*m_pD3DDevice)->SetStreamSource(0,m_pD3DVtxBuff,0,sizeof(VERTEX_2D));	//(0,渡すものが入ってるやつ,0,データの型指定)
		(*m_pD3DDevice)->SetFVF(FVF_VERTEX_2D);									//頂点フォーマットの設定
		(*m_pD3DDevice)->SetTexture(0, NULL);									//テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
		(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP						//プリミティブの種類
									, 0											//描画を開始する頂点番号
									, DRAW_SQUARE_PRINITIV_NUM);				//書きたいポリゴン数
	}
}

//*****************************************************************************
// フェードスタート関数
//*****************************************************************************
void CFade ::Start(FADE_MODE mode, D3DXCOLOR color, int endToTime)
{
	// 現在のモードと違ってたら
	if(m_mode != mode)
	{
		// 状態変更
		m_mode = mode;

		// フェードする色
		m_color = color;

		// フェードインならアルファ値MAX
		if(mode == MODE_FADE_IN)
		{
			m_color.a = MAX_ALPHA;
		}

		// フェードアウトならアルファ値MIN
		if(mode == MODE_FADE_OUT)
		{
			m_color.a = MIN_ALPHA;
		}

		// 終了時間設定
		m_nEndTime = endToTime;

		// フェードフラグオン
		m_bFade = true;

		// 速度決定
		m_fMovAlpha = MAX_ALPHA / (float)m_nEndTime;
	}
}
//----EOF----