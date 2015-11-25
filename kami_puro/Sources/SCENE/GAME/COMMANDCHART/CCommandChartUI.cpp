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
static const float COMMAND_ERROR_RANGE = 0.1f;			// コマンドUIが目的の座標周辺で動きを止める際の誤差
static const float COMMAND_MOVEMENT_COEFFICIENT = 0.7f;	// 目的の座標に行くときの移動量の係数
static const float COMMAND_MAX_ALPHA = 0.9f;			// アルファ値の最大値
static const UV_INDEX DEFAULT_BUTTON_RIGHT_UP = UV_INDEX(0.5f, 0.75f, 0.5f, 1.0f);	// 右側の上方向のボタンを押した時のテクスチャのUV
static const UV_INDEX DEFAULT_BUTTON_RIGHT_DOWN = UV_INDEX(0.75f, 1.0f, 0.5f, 1.0f);	// 右側の下方向のボタンを押した時のテクスチャのUV
static const UV_INDEX DEFAULT_BUTTON_LEFT_UP = UV_INDEX(0.5f, 0.75f, 0.0f, 0.5f);		// 左側の上方向のボタンを押した時のテクスチャのUV
static const UV_INDEX DEFAULT_BUTTON_LEFT_DOWN = UV_INDEX(0.75f, 1.0f, 0.0f, 0.5f);	// 左側の下方向のボタンを押した時のテクスチャのUV
static const UV_INDEX AFTER_BUTTON_RIGHT_UP = UV_INDEX(0.0f, 0.25f, 0.5f, 1.0f);	// 右側の上方向のボタンを押した時のテクスチャのUV
static const UV_INDEX AFTER_BUTTON_RIGHT_DOWN = UV_INDEX(0.25f, 0.5f, 0.5f, 1.0f);	// 右側の下方向のボタンを押した時のテクスチャのUV
static const UV_INDEX AFTER_BUTTON_LEFT_UP = UV_INDEX(0.0f, 0.25f, 0.0f, 0.5f);		// 左側の上方向のボタンを押した時のテクスチャのUV
static const UV_INDEX AFTER_BUTTON_LEFT_DOWN = UV_INDEX(0.25f, 0.5f, 0.0f, 0.5f);	// 左側の下方向のボタンを押した時のテクスチャのUV


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CCommandChartUI::CCommandChartUI(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) : CScene2D(pDevice, objType)
{
	m_isMoveX = true;
	m_isMoveY = true;
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
void CCommandChartUI::Init(BUTTON_TYPE ButtonType, D3DXVECTOR3 pos, TEXTURE_TYPE Texture)
{
	// ポリゴンの作成
	CScene2D::Init(pos, COMMAND_POLYGON_WIDTH, COMMAND_POLYGON_HEIGHT, Texture);

	// リストに追加
	CScene2D::AddLinkList(CRenderer::TYPE_RENDER_UI);

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//*************************************************************************
	//	テクスチャを貼り付けたら全てのポリゴンの色を白にするよ
	//*************************************************************************
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// ボタンの種類によって色を変えている
	switch (ButtonType)
	{
	case BUTTON_TYPE_1:
		SetUV((UV_INDEX*)&DEFAULT_BUTTON_RIGHT_UP);
		break;
	case BUTTON_TYPE_2:
		SetUV((UV_INDEX*)&DEFAULT_BUTTON_RIGHT_DOWN);
		break;
	case BUTTON_TYPE_3:
		SetUV((UV_INDEX*)&DEFAULT_BUTTON_LEFT_UP);
		break;
	case BUTTON_TYPE_4:
		SetUV((UV_INDEX*)&DEFAULT_BUTTON_LEFT_DOWN);
		break;
	default:
		break;
	}
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	SetColorPolygon(m_Color);

	// 自分のボタン情報を保存
	m_ButtonType = ButtonType;

	// 入力判断フラグをfalseにする
	m_isInputCommand = false;
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
	// 移動処理
	Move();
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
CCommandChartUI* CCommandChartUI::Create(LPDIRECT3DDEVICE9 *pDevice, BUTTON_TYPE ButtonType, D3DXVECTOR3 pos, TEXTURE_TYPE Texture)
{
	// new
	CCommandChartUI* temp = new CCommandChartUI(pDevice);

	// 初期化
	temp->Init(ButtonType, pos, Texture);

	// 返す
	return temp;
}

//-----------------------------------------------------------------------------
// 移動用関数
//-----------------------------------------------------------------------------
void CCommandChartUI::Move(void)
{
	// x座標の移動を行うか
	if (m_isMoveX)
	{
		// x座標の移動が終わるまでｘ座標の移動を行う
		if (m_Pos.x < m_DestPos.x - COMMAND_ERROR_RANGE || m_DestPos.x + COMMAND_ERROR_RANGE < m_Pos.x)
		{
			// 移動量
			float fMovement = (m_DestPos.x - m_Pos.x)*COMMAND_MOVEMENT_COEFFICIENT;
			m_Pos.x += fMovement;

			float fAlpha = 0;

			fAlpha = abs(COMMAND_MAX_ALPHA / fMovement);

			m_Color.a += fAlpha;

			if (m_Color.a > COMMAND_MAX_ALPHA)
			{
				m_Color.a = COMMAND_MAX_ALPHA;
			}

			// 移動させる
			SetPos(m_Pos);

			// 色を変更
			SetColorPolygon(m_Color);
		}
		else
		{
			// 現在の座標を目的の座標に合わせる
			m_Pos.x = m_DestPos.x;
			SetPos(m_Pos);
			m_isMoveX = false;
		}
	}
	// x座標の移動が終わっていたらy座標の移動を始める
	else if (m_isMoveY)
	{
		// x座標の移動が終わるまでｘ座標の移動を行う
		if (m_Pos.y < m_DestPos.y - COMMAND_ERROR_RANGE || m_DestPos.y + COMMAND_ERROR_RANGE < m_Pos.y)
		{
			// 移動量
			float fMovement = (m_DestPos.y - m_Pos.y)*COMMAND_MOVEMENT_COEFFICIENT;
			m_Pos.y += fMovement;

			float fAlpha = 0;

			fAlpha = abs(COMMAND_MAX_ALPHA / fMovement);

			m_Color.a += fAlpha;

			if (m_Color.a > COMMAND_MAX_ALPHA)
			{
				m_Color.a = COMMAND_MAX_ALPHA;
			}

			// 移動させる
			SetPos(m_Pos);

			// 色を変更
			SetColorPolygon(m_Color);
		}
		else
		{
			// 現在の座標を目的の座標に合わせる
			m_Pos.y = m_DestPos.y;
			SetPos(m_Pos);
			m_isMoveY = false;
		}
	}
	// 両方の移動が終わっている時完全に見えていなければならない
	else if (m_Color.a != COMMAND_MAX_ALPHA)
	{
		m_Color.a = COMMAND_MAX_ALPHA;
		SetColorPolygon(m_Color);
	}
}

//-----------------------------------------------------------------------------
// 入力された時のコマンドのテクスチャのUV値変更
//-----------------------------------------------------------------------------
void CCommandChartUI::InputUIUVChange(BUTTON_TYPE ButtonType, bool isPush)
{
	// 自分のボタン情報を保存
	m_ButtonType = ButtonType;

	// ボタンの種類によって色を変えている
	switch (ButtonType)
	{
	case BUTTON_TYPE_1:
		if (!isPush)
		{
			SetUV((UV_INDEX*)&DEFAULT_BUTTON_RIGHT_UP);
			break;
		}
		SetUV((UV_INDEX*)&AFTER_BUTTON_RIGHT_UP);
		break;

	case BUTTON_TYPE_2:
		if (!isPush)
		{
			SetUV((UV_INDEX*)&DEFAULT_BUTTON_RIGHT_DOWN);
			break;
		}
		SetUV((UV_INDEX*)&AFTER_BUTTON_RIGHT_DOWN);
		break;

	case BUTTON_TYPE_3:
		if (!isPush)
		{
			SetUV((UV_INDEX*)&DEFAULT_BUTTON_LEFT_UP);
			break;
		}
		SetUV((UV_INDEX*)&AFTER_BUTTON_LEFT_UP);
		break;

	case BUTTON_TYPE_4:
		if (!isPush)
		{
			SetUV((UV_INDEX*)&DEFAULT_BUTTON_LEFT_DOWN);
			break;
		}
		SetUV((UV_INDEX*)&AFTER_BUTTON_LEFT_DOWN);
		break;

	default:
		break;
	}
}

//-----------------------------------------------------------------------------
//	コマンド入力終了時の処理
//-----------------------------------------------------------------------------
void CCommandChartUI::EndOfInputCommand(void)
{
	// コマンド入力終了時にリストから除外
	CScene2D::UnLinkList(CRenderer::TYPE_RENDER_NORMAL);
}

//-----------------------------------------------------------------------------
//	コマンド入力開始時の処理
//	引数：座標
//-----------------------------------------------------------------------------
void CCommandChartUI::RestartOfInputCommand(D3DXVECTOR3 pos)
{
	// コマンド入力開始時にリストに追加
	CScene2D::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	// 座標を設定
	m_Pos = pos;
	SetPos(m_Pos);
}

// EOF