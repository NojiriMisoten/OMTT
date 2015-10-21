//=============================================================================
//
//	CCommandChartUIクラスの処理(CCommandChartUI.cpp)
//
//	Author : 佐藤　諒一
//
//=============================================================================
// インクルード
#include "CCommandChartUI.h"
#include "../../../RENDERER/CRenderer.h"

//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
static const float COMMAND_POLYGON_WIDTH = 150.0f;	// コマンドのポリゴンの横幅
static const float COMMAND_POLYGON_HEIGHT = 50.0f;	// コマンドのポリゴンの高さ

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CCommandChartUI::CCommandChartUI(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) : CScene2D(pDevice, objType)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CCommandChartUI::~CCommandChartUI()
{
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void CCommandChartUI::Init(BUTTON_TYPE ButtonType, D3DXVECTOR3 pos)
{
	// ポリゴンの作成
	CScene2D::Init(pos, COMMAND_POLYGON_WIDTH, COMMAND_POLYGON_HEIGHT, TEXTURE_NULL);

	// リストに追加
	CScene2D::AddLinkList(CRenderer::TYPE_RENDER_UI);

	// 暫定処理
	// ボタンの種類によって色を変えている
	switch (ButtonType)
	{
	case BUTTON_TYPE_1:
		SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		break;
	case BUTTON_TYPE_2:
		SetColorPolygon(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		break;
	case BUTTON_TYPE_3:
		SetColorPolygon(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		break;
	case BUTTON_TYPE_4:
		SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CCommandChartUI::Uninit(void)
{
	// 終了
	CScene2D::Uninit();
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CCommandChartUI::Update(void)
{
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void CCommandChartUI::Draw(void)
{
	CScene2D::Draw();
}

//-----------------------------------------------------------------------------
// 生成処理
//-----------------------------------------------------------------------------
CCommandChartUI* CCommandChartUI::Create(LPDIRECT3DDEVICE9 *pDevice,  BUTTON_TYPE ButtonType, D3DXVECTOR3 pos)
{
	// new
	CCommandChartUI* temp = new CCommandChartUI(pDevice);

	// 初期化
	temp->Init(ButtonType, pos);

	// 返す
	return temp;
}

// EOF