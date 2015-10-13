//=============================================================================
//
// CRendererクラス [CRenderer.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CRenderer.h"
#include "../SCENE/CSCENE/CScene2D.h"
#include "../DEBUG_PROC/CDebugProc.h"
#ifdef _DEBUG
#include "../INPUT/CInputKeyboard.h"
#endif

//*****************************************************************************
// マクロ
//*****************************************************************************
static const float		POLYGON_WIDTH = 200.0f;
static const float		POLYGON_HEIGHT = 200.0f;
static const int		NUM_BACK_BUFFER = 1;									// バックバッファの数
static const D3DXCOLOR	CLEAR_SCREEN_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 画面クリアカラー

//*****************************************************************************
// staticメンバ変数
//*****************************************************************************
LPDIRECT3DDEVICE9 CRenderer ::m_pD3DDevice = NULL;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CRenderer ::CRenderer(void)
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CRenderer ::~CRenderer(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CRenderer :: Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount			= NUM_BACK_BUFFER;			// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;		// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;				// デプスバッファとして16bitを使う
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.hDeviceWindow				= hWnd;
	
	// ウィンドウモード
	if(bWindow)
	{
		d3dpp.FullScreen_RefreshRateInHz	= 0;								// リフレッシュレート
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	// フルスクリーンモード
	else
	{
		d3dpp.FullScreen_RefreshRateInHz	= 60;			// リフレッシュレート
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
								, D3DDEVTYPE_HAL
								, hWnd
								, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED
								, &d3dpp
								, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
									, D3DDEVTYPE_HAL
									, hWnd
									, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED
									, &d3dpp
									, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
										, D3DDEVTYPE_REF
										, hWnd
										, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED
										, &d3dpp
										, &m_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートパラメータの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);

	// サンプラーステートパラメータの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// アルファテストを行う
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CRenderer ::Uninit(void)
{
	// デバイスオブジェクトの開放
	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	
	// Direct3Dオブジェクトの開放
	if(m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CRenderer ::Update()
{
	// 全て更新
	CScene::UpdateAll();
}

void CRenderer ::Update(int priority)
{
	// 指定したもののみ更新
	CScene::UpdateChoice(priority);
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CRenderer ::Draw()
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), CLEAR_SCREEN_COLOR, 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// 全て描画
		CScene::DrawAll();
#ifdef _DEBUG
		// FPS描画
		CDebugProc::Print("FPS:%d\n", m_nCountFPS);

		// デバッグ情報描画
		CDebugProc::Draw();
#endif
		// 描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

// 指定したもののみ描画
void CRenderer ::Draw(int priority, int priority2, int priority3, int priority4, int priority5, int priority6, int priority7, int priority8)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), CLEAR_SCREEN_COLOR, 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
#ifdef _DEBUG
		// FPS描画
		CDebugProc::Print("FPS:%d\n",m_nCountFPS);

		// デバッグ情報描画
		CDebugProc::Draw();

		// ワイヤーフレーム描画
		if(CInputKeyboard::GetKeyboardPress(DIK_1))
		{
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}

		// ソリッド描画
		if(CInputKeyboard::GetKeyboardPress(DIK_0))
		{
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
#endif
		// 指定描画
		CScene::DrawChoice(priority);
		if(priority2 > -1)
		{
			CScene::DrawChoice(priority2);
		}
		if(priority3 > -1)
		{
			CScene::DrawChoice(priority3);
		}
		if(priority4 > -1)
		{
			CScene::DrawChoice(priority4);
		}
		if(priority5 > -1)
		{
			CScene::DrawChoice(priority5);
		}
		if(priority6 > -1)
		{
			CScene::DrawChoice(priority6);
		}
		if(priority7 > -1)
		{
			CScene::DrawChoice(priority7);
		}
		if(priority8 > -1)
		{
			CScene::DrawChoice(priority8);
		}

		// 描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//----EOF----