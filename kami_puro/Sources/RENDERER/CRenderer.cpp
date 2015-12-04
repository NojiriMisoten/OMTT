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
#include "../BASE_OBJECT/CScene.h"
#include "../BASE_OBJECT/CScene2D.h"
#include "../SHADER/CShader.h"
#include "../DEBUG_PROC/CDebugProc.h"

#ifdef _DEBUG
#include "../INPUT/CInputKeyboard.h"
#endif

//*****************************************************************************
// マクロ
//*****************************************************************************
static const int		NUM_BACK_BUFFER = 1;									// バックバッファの数
static const D3DXCOLOR	CLEAR_SCREEN_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// 画面クリアカラー
static const int		GAUSS_WEIGHT_NUM = 5;									// ガウスの重みの数
static const int		DEFAULT_RENDER_TEXTURE_SIZE = 512;
//*****************************************************************************
// staticメンバ変数
//*****************************************************************************
LPDIRECT3DDEVICE9				CRenderer ::m_pD3DDevice = NULL;
LPDIRECT3DTEXTURE9				CRenderer::m_pRenderTexture[TYPE_RENDER_MAX] = {NULL};
LPDIRECT3DSURFACE9				CRenderer::m_pRenderSurface[TYPE_RENDER_MAX] = {NULL};
LPDIRECT3DSURFACE9				CRenderer::m_pRenderSurfaceDepth[TYPE_RENDER_MAX] = {NULL};
LPDIRECT3DSURFACE9				CRenderer::m_pDefaultSurface = NULL;
LPDIRECT3DSURFACE9				CRenderer::m_pDefaultSurfaceDepth = NULL;
LPDIRECT3DSURFACE9				CRenderer::m_pKeepSurface = NULL;
LPDIRECT3DSURFACE9				CRenderer::m_pKeepSurfaceDepth = NULL;
LPDIRECT3DTEXTURE9				CRenderer::m_pKeepTexture = NULL;
D3DVIEWPORT9					CRenderer::m_DefaultViewPort;
D3DVIEWPORT9					CRenderer::m_ChangeViewPort[TYPE_RENDER_MAX];
LPDIRECT3DVERTEXDECLARATION9	CRenderer::m_pDecl = NULL;
bool							CRenderer::m_bBeginScene = false;
VF								CRenderer::m_Vertex[4] = 
{
	D3DXVECTOR3(-2.0f, -2.0f, 0.f), D3DXVECTOR3(0, 1, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0.f, 0.f),
	D3DXVECTOR3((float)SCREEN_WIDTH + 0.5f, -2.0f, 0.f), D3DXVECTOR3(0, 1, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1.f, 0.f),
	D3DXVECTOR3(-2.0f, (float)SCREEN_HEIGHT + 0.5f, 0.f), D3DXVECTOR3(0, 1, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0.f, 1.f),
	D3DXVECTOR3((float)SCREEN_WIDTH + 0.5f, (float)SCREEN_HEIGHT + 0.5f, 0.f), D3DXVECTOR3(0, 1, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1.f, 1.f),
};
float							CRenderer::m_fCameraRot = 0.f;
D3DXMATRIX						CRenderer::m_ProjMtx;
D3DXMATRIX						CRenderer::m_ViewMtx;
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CRenderer ::CRenderer(void)
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_bBeginScene = false;
	m_fCameraRot = 0.f;
	D3DXMatrixIdentity(&m_ProjMtx);
	D3DXMatrixIdentity(&m_ViewMtx);
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
	D3DPRESENT_PARAMETERS d3dpp = {};
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
	d3dpp.hDeviceWindow				= hWnd;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.BackBufferCount			= NUM_BACK_BUFFER;			// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= D3DFMT_A8R8G8B8;			// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;				// デプスバッファとして16bitを使う

	// ウィンドウモード
	if(bWindow)
	{
		d3dpp.FullScreen_RefreshRateInHz	= 0;								// リフレッシュレート
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	// フルスクリーンモード
	else
	{
		d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
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

	SetDefaultSamplerState();

	// バックバッファサーフェスの取得
	m_pD3DDevice->GetRenderTarget(0, &m_pDefaultSurface);
	m_pD3DDevice->GetDepthStencilSurface(&m_pDefaultSurfaceDepth);

	// ビューポートの取得
	m_pD3DDevice->GetViewport(&m_DefaultViewPort);

	////==== レンダーターゲット用 ====
	for (int i = 0; i < TYPE_RENDER_MAX; ++i)
	{
		int width = DEFAULT_RENDER_TEXTURE_SIZE;
		int height = DEFAULT_RENDER_TEXTURE_SIZE;
		
		// デバイスの深度バッファの幅と高さを取得
		UINT uiDevZBufWidth;    // デバイスバッファ幅
		UINT uiDevZBufHeight;   // デバイスバッファ高
		IDirect3DSurface9 *pTmpSf;
		m_pD3DDevice->GetDepthStencilSurface(&pTmpSf);
		D3DSURFACE_DESC SufDesc;
		pTmpSf->GetDesc(&SufDesc);
		width = uiDevZBufWidth = SufDesc.Width;
		height = uiDevZBufHeight = SufDesc.Height;
		pTmpSf->Release();
		
		if (i == TYPE_RENDER_HALF_SIDE || i == TYPE_RENDER_HALF_GAUS)
		{
			width = (int)(width * 0.5f);
			height = (int)(height * 0.5f);
		}
		else if (i == TYPE_RENDER_QUARTER_SIDE || i == TYPE_RENDER_QUARTER_GAUS)
		{
			width = (int)(width * 0.25f);
			height = (int)(height * 0.25f);
		}

		if (FAILED(m_pD3DDevice->CreateTexture(width					// 幅
											, height					// 高さ
											, 1						// ミップマップ数
											, D3DUSAGE_RENDERTARGET	// 使用方法
											, D3DFMT_A8R8G8B8		// フォーマット
											, D3DPOOL_DEFAULT		// メモリ管理方法
											, &m_pRenderTexture[i]	// テクスチャインターフェースへのポインタ
											, NULL)))
		{
			MessageBox(NULL, "レンダーターゲットテクスチャの読み込みに失敗しました\n", "エラー", MB_OK | MB_ICONERROR);
			m_pRenderTexture[i] = NULL;
			return false;
		}
		
		// サーフェスのテクスチャ
		m_pRenderTexture[i]->GetSurfaceLevel(0, &m_pRenderSurface[i]);
	
		// デバイスの深度バッファの幅と高さを取得
		m_pD3DDevice->GetDepthStencilSurface(&pTmpSf);
		pTmpSf->GetDesc(&SufDesc);
		width = uiDevZBufWidth = SufDesc.Width;
		height = uiDevZBufHeight = SufDesc.Height;
		pTmpSf->Release();

		// Zバッファ、ステンシルバッファの作成
		if (FAILED(m_pD3DDevice->CreateDepthStencilSurface(width
														, height
														, D3DFMT_D24S8
														, D3DMULTISAMPLE_NONE
														, 0
														, FALSE
														, &m_pRenderSurfaceDepth[i]
														, NULL)))
		{
			MessageBox(NULL, "レンダーターゲットサーフェス失敗しました\n", "エラー", MB_OK | MB_ICONERROR);
			m_pRenderSurfaceDepth[i] = NULL;
			return false;
		}
		m_pD3DDevice->SetRenderTarget(0, m_pRenderSurface[i]);
		m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
		
		// ビューポートの取得
		m_pD3DDevice->GetViewport(&m_ChangeViewPort[i]);
	}
		
	m_pD3DDevice->SetRenderTarget(0, m_pDefaultSurface);
	m_pD3DDevice->SetDepthStencilSurface(m_pDefaultSurfaceDepth);

	// 頂点宣言したものを作る
	m_pD3DDevice->CreateVertexDeclaration(GetVertexElement(), &m_pDecl);

	m_pD3D->Release();
	m_pD3D = NULL;

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

	for (int i = 0; i < TYPE_RENDER_MAX; ++i)
	{
		if (m_pRenderSurface[i])
		{
			m_pRenderSurface[i]->Release();
			m_pRenderSurface[i] = NULL;
		}

		if (m_pRenderSurfaceDepth[i])
		{
			m_pRenderSurfaceDepth[i]->Release();
			m_pRenderSurfaceDepth[i] = NULL;
		}

		if (m_pRenderTexture[i])
		{
			m_pRenderTexture[i]->Release();
			m_pRenderTexture[i] = NULL;
		}
	}

	if (m_pDefaultSurface)
	{
		m_pDefaultSurface->Release();
		m_pDefaultSurface = NULL;
	}

	if (m_pDefaultSurfaceDepth)
	{
		m_pDefaultSurfaceDepth->Release();
		m_pDefaultSurfaceDepth = NULL;
	}

	if (m_pDecl)
	{
		m_pDecl->Release();
		m_pDecl = NULL;
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
	ChangeDefaultRenderTarget();
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), CLEAR_SCREEN_COLOR, 1.0f, 0);
	for (int i = 0; i < TYPE_RENDER_MAX; ++i)
	{
		TYPE_RENDER_TEX render = (TYPE_RENDER_TEX)i;
		ClearRenderTarget(render);
	}

	// 描画の開始
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		m_bBeginScene = true;
	
		// 全て描画
		CScene::DrawAll();

		// トゥーン描画
		//DrawToon();

		// ライトブルーム
		//DrawLightBloom();

		// バックバッファーへの描画
		DrawBackBuffer();

#ifdef _DEBUG
		// FPS描画
		CDebugProc::PrintU("FPS:%d\n", m_nCountFPS);

		// デバッグ情報描画
		CDebugProc::Draw();
#endif
	
		// 描画の終了
		m_pD3DDevice->EndScene();
		m_bBeginScene = false;
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//*****************************************************************************
// レンダーターゲット変更
//*****************************************************************************
void CRenderer::ChangeRenderTarget(TYPE_RENDER_TEX idx)
{
	if (m_bBeginScene)
	{
		m_pD3DDevice->EndScene();
	}

	HRESULT hr;
	hr = m_pD3DDevice->SetRenderTarget(0, m_pRenderSurface[(int)idx]);
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pRenderSurfaceDepth[(int)idx]);
	hr = m_pD3DDevice->SetViewport(&m_ChangeViewPort[idx]);
	
	if (m_bBeginScene)
	{
		m_pD3DDevice->BeginScene();
	}
}

//*****************************************************************************
// 元のレンダーターゲットに変更
//*****************************************************************************
void CRenderer::ChangeDefaultRenderTarget(void)
{
	if (m_bBeginScene)
	{
		m_pD3DDevice->EndScene();
	}

	HRESULT hr;
	hr = m_pD3DDevice->SetRenderTarget(0, m_pDefaultSurface);
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pDefaultSurfaceDepth);
	hr = m_pD3DDevice->SetViewport(&m_DefaultViewPort);

	if (m_bBeginScene)
	{
		m_pD3DDevice->BeginScene();
	}
}

//*****************************************************************************
// レンダーテクスチャーゲット
//*****************************************************************************
LPDIRECT3DTEXTURE9* CRenderer::GetRenderTexture(TYPE_RENDER_TEX idx)
{
	return &m_pRenderTexture[(int)idx];
}

//*****************************************************************************
// レンダーターゲットのクリア
//*****************************************************************************
void CRenderer::ClearRenderTarget(TYPE_RENDER_TEX idx)
{
	m_pD3DDevice->SetRenderTarget(0, m_pRenderSurface[(int)idx]);
	m_pD3DDevice->SetDepthStencilSurface(m_pRenderSurfaceDepth[(int)idx]);
	m_pD3DDevice->SetViewport(&m_ChangeViewPort[idx]);

	// テクスチャサーフェイスのクリア
	if (idx == TYPE_RENDER_NORMAL_VEC)
	{
		m_pD3DDevice->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			1.0f,
			0);
	}
	else if (idx == TYPE_RENDER_TOON_OBJECT_DEPTH)
	{
		m_pD3DDevice->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(255, 255, 255, 255),
			1.0f,
			0);
	}
	else
	{
		m_pD3DDevice->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(255, 0, 0, 0),
			1.0f,
			0);
	}
	ChangeDefaultRenderTarget();
}

//*****************************************************************************
// バックバッファーへの描画
//*****************************************************************************
void CRenderer::DrawBackBuffer(void)
{
	ChangeDefaultRenderTarget();

	D3DXMATRIX world;	// ワールドマトリクス
	// ワールドマトリクス作成
	D3DXMatrixIdentity(&world);

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_BACK_BUFFER);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_BACK_BUFFER);		// 定数レジスタのテーブル
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// 定数レジスタのテーブル
	HRESULT hr = 0;

	// シェーダーの適用
	hr = m_pD3DDevice->SetVertexShader(*_vs);
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	LPDIRECT3DTEXTURE9* tex;
	UINT toonSamplerIndex = (*_psc)->GetSamplerIndex("toonRenderSampler");
	UINT lightBloomSamplerIndex = (*_psc)->GetSamplerIndex("lightBloomRenderSampler");
	UINT fadeSamplerIndex = (*_psc)->GetSamplerIndex("fadeRenderSampler");

	// 普通の
	tex = GetRenderTexture(TYPE_RENDER_TOON);
	hr = m_pD3DDevice->SetTexture(toonSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(toonSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(toonSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(toonSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(toonSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	// ライトブルームの
	tex = GetRenderTexture(TYPE_RENDER_LIGHT_BLOOM);
	hr = m_pD3DDevice->SetTexture(lightBloomSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(lightBloomSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(lightBloomSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(lightBloomSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(lightBloomSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// フェードの
	tex = GetRenderTexture(TYPE_RENDER_FADE);
	hr = m_pD3DDevice->SetTexture(fadeSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(fadeSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(fadeSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(fadeSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(fadeSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// 頂点シェーダーの定数レジスタに値をセット
	D3DXMATRIX view, proj, screenProj;

	screenProj = SCREEN_PROJ;
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &screenProj);

	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &screenProj);

	// 頂点宣言したやつをセット(SetFVFの代わり)
	m_pD3DDevice->SetVertexDeclaration(m_pDecl);

	// 描画
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, DRAW_SQUARE_PRINITIV_NUM, m_Vertex, sizeof(VF));

	hr = m_pD3DDevice->SetTexture(toonSamplerIndex, NULL);
	hr = m_pD3DDevice->SetTexture(fadeSamplerIndex, NULL);
	hr = m_pD3DDevice->SetTexture(lightBloomSamplerIndex, NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
	hr = m_pD3DDevice->SetPixelShader(NULL);
}

//*****************************************************************************
// トゥーンにする
//*****************************************************************************
void CRenderer::DrawToon(void)
{
	// レンダーターゲット変更
	ChangeRenderTarget(TYPE_RENDER_TOON);

	D3DXMATRIX world;	// ワールドマトリクス
	// ワールドマトリクス作成
	D3DXMatrixIdentity(&world);

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_TOON);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_TOON);		// 定数レジスタのテーブル
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// 定数レジスタのテーブル
	HRESULT hr = 0;

	// シェーダーの適用
	hr = m_pD3DDevice->SetVertexShader(*_vs);
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	LPDIRECT3DTEXTURE9* tex;
	UINT normalTexSamplerIndex = (*_psc)->GetSamplerIndex("normalRendereTexSampler");
	UINT normalVecTexSamplerIndex = (*_psc)->GetSamplerIndex("normalVecTexSampler");

	// 普通の
	tex = GetRenderTexture(TYPE_RENDER_NORMAL);
	hr = m_pD3DDevice->SetTexture(normalTexSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(normalTexSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(normalTexSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(normalTexSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(normalTexSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// エッジの
	tex = GetRenderTexture(TYPE_RENDER_NORMAL_VEC);
	hr = m_pD3DDevice->SetTexture(normalVecTexSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(normalVecTexSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(normalVecTexSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(normalVecTexSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(normalVecTexSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// 頂点シェーダーの定数レジスタに値をセット
	D3DXMATRIX view, proj, screenProj;

	// 座標変換用パラメータ
	screenProj = SCREEN_PROJ;
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &screenProj);
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &screenProj);

	// 解像度
	float resolution[2] = { DEFAULT_RENDER_TEXTURE_SIZE, DEFAULT_RENDER_TEXTURE_SIZE };
	(*_psc)->SetFloatArray(m_pD3DDevice, "g_ScreenResolution", &resolution[0], 2);

	// 頂点宣言したやつをセット(SetFVFの代わり)
	m_pD3DDevice->SetVertexDeclaration(m_pDecl);

	// 描画
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, DRAW_SQUARE_PRINITIV_NUM, m_Vertex, sizeof(VF));

	hr = m_pD3DDevice->SetTexture(normalTexSamplerIndex, NULL);
	hr = m_pD3DDevice->SetTexture(normalVecTexSamplerIndex, NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
	hr = m_pD3DDevice->SetPixelShader(NULL);
}

//================================================
// ガウス
//================================================
void CRenderer::DrawGauss(TYPE_RENDER_TEX orijinTex, TYPE_RENDER_TEX sideRender, TYPE_RENDER_TEX gausRender)
{
	D3DXMATRIX world;	// ワールドマトリクス
	// ワールドマトリクス作成
	D3DXMatrixIdentity(&world);

	float total = 0.f;
	float weightTbl[GAUSS_WEIGHT_NUM] = { 0.f };
	float dispersion = 100.f;		// 適当な数値 0禁止

	// ガウスの重みの計算
	for (int i = 0; i < GAUSS_WEIGHT_NUM; ++i)
	{
		weightTbl[i] = expf(-0.5f * (i * i) / dispersion);
		if (i == 0)
		{
			total += weightTbl[i];
		}
		else
		{
			total += 2.0f * weightTbl[i];
		}
	}
	for (int i = 0; i < GAUSS_WEIGHT_NUM; ++i)
	{
		weightTbl[i] /= total;
	}

	// レンダーターゲット変更
	ChangeRenderTarget(sideRender);

	HRESULT hr;
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// 定数レジスタのテーブル
	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_GAUSS_SIDE);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_GAUSS_SIDE);		// 定数レジスタのテーブル
	// シェーダーの適用
	UINT texSamplerIndex = (*_psc)->GetSamplerIndex("texSampler");
	hr = m_pD3DDevice->SetPixelShader(*_ps);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	// 元の
	LPDIRECT3DTEXTURE9* tex = GetRenderTexture(orijinTex);
	hr = m_pD3DDevice->SetTexture(texSamplerIndex, *tex);

	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &SCREEN_PROJ);
	float resolution = 1.0f / SCREEN_WIDTH;
	hr = (*_psc)->SetFloat(m_pD3DDevice, "g_Resolution", resolution);
	hr = (*_psc)->SetFloat(m_pD3DDevice, "g_WeihtTabl_Zero", weightTbl[0]);
	hr = (*_psc)->SetFloatArray(m_pD3DDevice, "g_WeightTabl_One_Four", &weightTbl[1], 4);

	// シェーダーの適用
	hr = m_pD3DDevice->SetVertexShader(*_vs);

	// 描画
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(VF));

	// 元に戻す
	m_pD3DDevice->SetTexture(texSamplerIndex, NULL);
	m_pD3DDevice->SetPixelShader(NULL);

	// レンダーターゲット変更
	ChangeRenderTarget(gausRender);

	_ps = CShader::GetPS(PS_TYPE_GAUSS_VERTICAL);
	_psc = CShader::GetPSC(PS_TYPE_GAUSS_VERTICAL);		// 定数レジスタのテーブル
	texSamplerIndex = (*_psc)->GetSamplerIndex("texSampler");
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ガウス掛けたやつ
	tex = GetRenderTexture(sideRender);
	hr = m_pD3DDevice->SetTexture(texSamplerIndex, *tex);

	// コンスタントテーブルに値送る
	resolution = 1.0f / SCREEN_HEIGHT;
	hr = (*_psc)->SetFloat(m_pD3DDevice, "g_Resolution", resolution);
	hr = (*_psc)->SetFloat(m_pD3DDevice, "g_WeihtTabl_Zero", weightTbl[0]);
	hr = (*_psc)->SetFloatArray(m_pD3DDevice, "g_WeightTabl_One_Four", &weightTbl[1], 4);

	// シェーダーの適用
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	// 描画
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(VF));

	// 元に戻す
	m_pD3DDevice->SetTexture(texSamplerIndex, NULL);
	m_pD3DDevice->SetPixelShader(NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
}

//================================================
// 高輝度抽出
//================================================
void CRenderer::DrawBrightness(void)
{
	ChangeRenderTarget(TYPE_RENDER_HIGH_LUMINANCE);

	D3DXMATRIX world;	// ワールドマトリクス
	// ワールドマトリクス作成
	D3DXMatrixIdentity(&world);

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_HIGH_LUMINANCE);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_HIGH_LUMINANCE);		// 定数レジスタのテーブル
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// 定数レジスタのテーブル
	HRESULT hr = 0;

	// シェーダーの適用
	hr = m_pD3DDevice->SetVertexShader(*_vs);
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	LPDIRECT3DTEXTURE9* tex;
	UINT texSamplerIndex = (*_psc)->GetSamplerIndex("texSampler");
	// 普通の
	tex = GetRenderTexture(TYPE_RENDER_BASE_HIGH_LUMINANCE);
	hr = m_pD3DDevice->SetTexture(texSamplerIndex, *tex);

	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(texSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// 頂点シェーダーの定数レジスタに値をセット
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);	// こいつらをあらかじめ掛け合わせてシェーダーに送るとコスト削減可
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &SCREEN_PROJ);

	float resolution[2] = { SCREEN_WIDTH, SCREEN_HEIGHT };
	hr = (*_psc)->SetFloatArray(m_pD3DDevice, "g_ScreenResolution", &resolution[0], 2);

	// 頂点宣言したやつをセット(SetFVFの代わり)
	m_pD3DDevice->SetVertexDeclaration(m_pDecl);

	// 描画
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(VF));

	hr = m_pD3DDevice->SetTexture(texSamplerIndex, NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
	hr = m_pD3DDevice->SetPixelShader(NULL);
}

//================================================
// ライトブルーム
//================================================
void CRenderer::DrawLightBloom(void)
{
	// 高輝度抽出
	DrawBrightness();

	// 1/2サイズガウス
	DrawGauss(TYPE_RENDER_HIGH_LUMINANCE, TYPE_RENDER_HALF_SIDE, TYPE_RENDER_HALF_GAUS);

	// 1/4サイズガウス
	DrawGauss(TYPE_RENDER_HALF_GAUS, TYPE_RENDER_QUARTER_SIDE, TYPE_RENDER_QUARTER_GAUS);

	// レンダーターゲット変更
	ChangeRenderTarget(CRenderer::TYPE_RENDER_LIGHT_BLOOM);

	D3DXMATRIX world;	// ワールドマトリクス
	// ワールドマトリクス作成
	D3DXMatrixIdentity(&world);

	LPDIRECT3DPIXELSHADER9* _ps = CShader::GetPS(PS_TYPE_LIGHT_BLOOM);
	LPD3DXCONSTANTTABLE* _psc = CShader::GetPSC(PS_TYPE_LIGHT_BLOOM);		// 定数レジスタのテーブル
	LPDIRECT3DVERTEXSHADER9* _vs = CShader::GetVS(VS_TYPE_DRAW_2D);
	LPD3DXCONSTANTTABLE* _vsc = CShader::GetVSC(VS_TYPE_DRAW_2D);		// 定数レジスタのテーブル
	HRESULT hr = 0;

	// シェーダーの適用
	hr = m_pD3DDevice->SetVertexShader(*_vs);
	hr = m_pD3DDevice->SetPixelShader(*_ps);

	LPDIRECT3DTEXTURE9* tex;
	UINT gausSamplerIndex = (*_psc)->GetSamplerIndex("gausSampler");
	
	// ガウスのやつ
	tex = GetRenderTexture(TYPE_RENDER_QUARTER_GAUS);
	hr = m_pD3DDevice->SetTexture(gausSamplerIndex, *tex);
	hr = m_pD3DDevice->SetSamplerState(gausSamplerIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(gausSamplerIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = m_pD3DDevice->SetSamplerState(gausSamplerIndex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	hr = m_pD3DDevice->SetSamplerState(gausSamplerIndex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// 頂点シェーダーの定数レジスタに値をセット
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gWorld", &world);	// こいつらをあらかじめ掛け合わせてシェーダーに送るとコスト削減可
	hr = (*_vsc)->SetMatrix(m_pD3DDevice, "gScreenProj", &SCREEN_PROJ);

	float resolution[2] = { SCREEN_WIDTH, SCREEN_HEIGHT };
	hr = (*_psc)->SetFloatArray(m_pD3DDevice, "g_ScreenResolution", &resolution[0], 2);

	// 頂点宣言したやつをセット(SetFVFの代わり)
	m_pD3DDevice->SetVertexDeclaration(m_pDecl);

	// 描画
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_Vertex, sizeof(VF));

	hr = m_pD3DDevice->SetTexture(gausSamplerIndex, NULL);
	hr = m_pD3DDevice->SetVertexShader(NULL);
	hr = m_pD3DDevice->SetPixelShader(NULL);
}

//================================================
// レンダーターステートのデフォルト設定
//================================================
void CRenderer::SetDefaultSamplerState(void)
{
	// レンダーステートパラメータの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// アルファテストを行う
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}
//----EOF----