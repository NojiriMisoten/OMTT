//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 野尻　尚希
//
//=============================================================================
#include "main.h"
#include "../MANAGER/CManager.h"
#include "../RENDERER/CRenderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const char*	CLASS_NAME	= "AppClass";		// ウインドウのクラス名
static const char*	WINDOW_NAME	= "ドミノエフェクト";			// ウインドウのキャプション名
static const int	TIME_INTERVAL = 500;			// 実行間隔
static const int	FPS_COEFFICIENT = 1000;			// FPSの係数
static const int	BASE_FPS = 1000 / 60;			// FPSの基準速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init(void);
void Update(void);
void Draw(void);
void Uninit(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
static CManager					*g_pManager;

// 頂点宣言
static D3DVERTEXELEMENT9 g_vElement[] =
{
	// 頂点ストリーム(パイプライン)番号, オフセット(頂点の型の先頭からのバイト数), データ型, DEFAULTでＯＫ, 使用用途, 使用用途が同じものを複数使うときに仕分ける番号
	{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }		// 頂点 (D3DDECLUSAGE_POSITIONTは座標変換済み頂点を表す)
	, { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 }	// 法線
	, { 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 }	// 色
	, { 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }	// UV
	, D3DDECL_END()																	// 宣言の終了
};
//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	// メモリリークチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// マネージャー作成
	g_pManager = new CManager;

	// FPS用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの使える機能の設定
	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_MINIMIZEBOX & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	// 矩形作成
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.bottom = SCREEN_HEIGHT;
	rc.right = SCREEN_WIDTH;

	long WidthWindow = rc.right - rc.left;
	long HeightWindow = rc.bottom - rc.top;

	// アジャストウィンドウ
	AdjustWindowRect(&rc, style, FALSE);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						style,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						WidthWindow,
						HeightWindow,
						NULL,
						NULL,
						hInstance,
						NULL);


	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	bool modeWindow = true;
	//if(MessageBox(NULL,"フルスクリーンで起動しますか？","ウィンドウモード",MB_YESNO)==IDYES)
	//{
	//	//フルスクリーンで初期化処理(ウィンドウを作成してから行う)
	//	modeWindow = false;
	//}
	//else
	//{
	//	//通常の 初期化処理(ウィンドウを作成してから行う)
	//	modeWindow = true;
	//}

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(g_pManager->Init(hInstance, hWnd, modeWindow)))
	{
		return -1;
	}

	UpdateWindow(hWnd);

	// メッセージループ
	while(1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// 現在時刻取得
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= TIME_INTERVAL)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				// FPS表示
				g_pManager->GetRenderer()->SetFPS(dwFrameCount * FPS_COEFFICIENT / (dwCurrentTime - dwFPSLastTime));
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= BASE_FPS)
			{
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	g_pManager->Uninit();
	delete g_pManager;
}

//=============================================================================
// 更新処理
//=============================================================================
void Update()
{
	g_pManager->Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw()
{
	g_pManager->Draw();
}

D3DVERTEXELEMENT9* GetVertexElement(void)
{
	return &g_vElement[0];
}
//----EOF----