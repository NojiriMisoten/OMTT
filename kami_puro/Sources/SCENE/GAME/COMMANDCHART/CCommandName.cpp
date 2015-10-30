//=============================================================================
//
//	CCommandNameの処理[CComandName.cpp]
//
//	Author : 佐藤　諒一
//
//=============================================================================
//-----------------------------------------------------------------------------
//	インクルード
//-----------------------------------------------------------------------------
#include "CCommandName.h"

//-----------------------------------------------------------------------------
//	マクロ定義
//-----------------------------------------------------------------------------
static const float COMMAND_NAME_POLYGON_WIDTH = 100.0f;			// コマンドのポリゴンの横幅
static const float COMMAND_NAME_POLYGON_HEIGHT = 20.0f;			// コマンドのポリゴンの高さ
static const float COMMAND_NAME_DEFAULT_POSITION_ADD_X = COMMAND_NAME_POLYGON_WIDTH * 4.0f / 5.0f;
static const float COMMAND_NAME_ERROR_RANGE = 0.1f;				// コマンドUIが目的の座標周辺で動きを止める際の誤差
static const float COMMAND_NAME_MOVEMENT_COEFFICIENT = 0.7f;	// 目的の座標に行くときの移動量の係数
static const float COMMAND_NAME_MAX_ALPHA = 0.9f;				// アルファ値の最大値

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CCommandName::CCommandName(LPDIRECT3DDEVICE9 *pDevice, OBJTYPE objType) : CScene2D(pDevice, objType)
{
	m_isMoveX = true;
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CCommandName::~CCommandName()
{
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void CCommandName::Init(D3DXVECTOR3 pos, TEXTURE_TYPE Texture)
{
	// ポリゴンの大きさの3/5ずらす
	//pos.x += COMMAND_NAME_DEFAULT_POSITION_ADD_X;
	// ポリゴンの作成
	CScene2D::Init(pos, COMMAND_NAME_POLYGON_WIDTH, COMMAND_NAME_POLYGON_HEIGHT, Texture);

	// リストに追加
	CScene2D::AddLinkList(CRenderer::TYPE_RENDER_NORMAL);

	m_Color = D3DXCOLOR(0.3f, 0.4f, 0.5f, 1.0f);
	SetColorPolygon(m_Color);

	m_DestPos = pos;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CCommandName::Uninit(void)
{
	// 終了
	CScene2D::Uninit();
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CCommandName::Update(void)
{
	// 移動処理
	Move();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void CCommandName::Draw(void)
{
	CScene2D::Draw();
}

//-----------------------------------------------------------------------------
// 生成処理
//-----------------------------------------------------------------------------
CCommandName* CCommandName::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, TEXTURE_TYPE Texture)
{
	// new
	CCommandName* temp = new CCommandName(pDevice);

	// 初期化
	temp->Init(pos, Texture);

	// 返す
	return temp;
}

//-----------------------------------------------------------------------------
// 移動用関数
//-----------------------------------------------------------------------------
void CCommandName::Move(void)
{
	// x座標の移動を行うか
	if (m_isMoveX)
	{
		// x座標の移動が終わるまでｘ座標の移動を行う
		if (m_Pos.x < m_DestPos.x - COMMAND_NAME_ERROR_RANGE || m_DestPos.x + COMMAND_NAME_ERROR_RANGE < m_Pos.x)
		{
			// 移動量
			float fMovement = (m_DestPos.x - m_Pos.x)*COMMAND_NAME_MOVEMENT_COEFFICIENT;
			m_Pos.x += fMovement;

			float fAlpha = 0;

			fAlpha = abs(COMMAND_NAME_MAX_ALPHA / fMovement);

			m_Color.a += fAlpha;

			if (m_Color.a > COMMAND_NAME_MAX_ALPHA)
			{
				m_Color.a = COMMAND_NAME_MAX_ALPHA;
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
}

// EOF

// EOF