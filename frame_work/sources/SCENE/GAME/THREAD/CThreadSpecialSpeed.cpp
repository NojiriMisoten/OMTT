//=============================================================================
//
// CThreadSpecialSpeedクラス [CThreadSpecialSpeed.cpp]
// Author : 佐藤　諒一
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CThreadSpecialSpeed.h"
#include "../../../SCENE/CSCENE/CSceneAnime.h"
#include "../FIELD/CFieldManager.h"
#include "../FIELD/CNaviTile.h"
//*****************************************************************************
// マクロ
//*****************************************************************************
// 幅,高さ
static const float	MIN_WIDTH = 20.f;
static const float	MIN_HEIGHT = 20.f;
static const float	MAX_WIDTH = 40.f;
static const float	MAX_HEIGHT = 400.f;

// 糸が床になるまでのアニメーション時間
static const int	THREAD_FIELD_ANIM_TIME = 30;

// 長さ変更量
static const float	ADD_HEIGHT = MAX_HEIGHT / (float)GROW_THREAD_TIME;

// 幅変更量
static const float	ADD_WIDTH = MAX_WIDTH / (float)THREAD_FIELD_ANIM_TIME;

static const float DOWN_POS_POW = 5.0f;
static const float MOVE_POW = DOWN_POS_POW / 10.f;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CThreadSpecialSpeed::CThreadSpecialSpeed(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CThreadBase(pDevice, priority, type)
{
	// 変数初期化
	m_ThreadType = THREAD_TYPE_SPEED;

	// この糸の固有ステータス初期化
	m_fWidth = MIN_WIDTH;
	m_fHeight = MIN_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_bSetNavi = false;
	m_nAnimTimer = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CThreadSpecialSpeed ::~CThreadSpecialSpeed(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CThreadSpecialSpeed::Init(short nPlayerNum, D3DXVECTOR3 pos, DIRECTION_PLAYER_FACING playerDirection)
{
	m_nPlayerNum = nPlayerNum;
	m_vPos = pos - PLAYER_DIRECTION_VECTOR[playerDirection] * 5.f;
	m_Direction = playerDirection;
	CThreadBase::Init(m_vPos, m_fWidth, m_fHeight, TEXTURE_THREAD);

	if (m_Direction == PLAYER_DIRECTION_RIGHT || m_Direction == PLAYER_DIRECTION_LEFT)
	{
		Rot90_UV();
	}
	m_fJudgeWidth = m_fWidth * 0.9f;
	m_fJudgeHeight = m_fHeight;
	m_vJudgePos = m_vPos;

	CManager::PlaySoundA(SOUND_LABEL_SE_CREATE_ROAD);
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CThreadSpecialSpeed::Uninit(void)
{
	CThreadBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CThreadSpecialSpeed::Update(void)
{
	if (GetObjType() != CScene::OBJTYPE_FIELD)
	{
		GrowThread();
	}
	else
	{
		if (!m_bSetNavi)
		{
			CNaviTile* navi = CFieldManager::GetNaviTileAdr();
			if (navi)
			{
				D3DXVECTOR2 pos(m_vJudgePos.x, m_vJudgePos.y);
				navi->SetCanMove(pos, m_fJudgeWidth, m_fJudgeHeight, true);
			}
			m_bSetNavi = true;
		}

		FieldAnim();
	}
	m_fJudgeWidth = m_fWidth * 0.8f;
	m_fJudgeHeight = m_fHeight;
	m_vJudgePos = m_vPos;
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CThreadSpecialSpeed* CThreadSpecialSpeed::Create(LPDIRECT3DDEVICE9 *pDevice, short nPlayerNum, D3DXVECTOR3 pos, DIRECTION_PLAYER_FACING playerDirection)
{
	// 作成
	CThreadSpecialSpeed* p = new CThreadSpecialSpeed(pDevice);

	// 初期化
	p->Init(nPlayerNum, pos, playerDirection);


	return p;
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CThreadSpecialSpeed::Draw(void)
{
	CThreadBase::Draw();
}

//*****************************************************************************
// 糸伸びる処理
//*****************************************************************************
void CThreadSpecialSpeed::GrowThread(void)
{
	m_nCount++;
	if (m_nCount < GROW_THREAD_TIME * 0.5)
	{
		m_fAddPower = ADD_HEIGHT;
	}
	else if (m_nCount < GROW_THREAD_TIME)
	{
		m_fAddPower = -ADD_HEIGHT;
	}
	else
	{
		Death();
	}


	if (m_Direction == PLAYER_DIRECTION_UP)
	{
		CScene2D::AddHeight_BaseBottom(m_fAddPower);
	}
	else if (m_Direction == PLAYER_DIRECTION_DOWN)
	{
		CScene2D::AddHeight_BaseTop(m_fAddPower);
	}
	else if (m_Direction == PLAYER_DIRECTION_LEFT || m_Direction == PLAYER_DIRECTION_DOWNER_LEFT || m_Direction == PLAYER_DIRECTION_UPPER_LEFT)
	{
		CScene2D::AddWidth_BaseRight(m_fAddPower);
	}
	else if (m_Direction == PLAYER_DIRECTION_RIGHT || m_Direction == PLAYER_DIRECTION_DOWNER_RIGHT || m_Direction == PLAYER_DIRECTION_UPPER_RIGHT)
	{
		CScene2D::AddWidth_BaseLeft(m_fAddPower);
	}
}

//*****************************************************************************
// デス関数
//*****************************************************************************
void CThreadSpecialSpeed::Death(void)
{
	Release();
}

//*****************************************************************************
// フィールドアニメーション
//*****************************************************************************
void CThreadSpecialSpeed::FieldAnim(void)
{
	m_nAnimTimer++;
	if (m_nAnimTimer > THREAD_FIELD_ANIM_TIME)
	{
		UpDown();
		return;
	}

	if (m_Direction == PLAYER_DIRECTION_UP)
	{
		m_fWidth += ADD_WIDTH;
	}
	else if (m_Direction == PLAYER_DIRECTION_DOWN)
	{
		m_fWidth += ADD_WIDTH;
	}
	else if (m_Direction == PLAYER_DIRECTION_LEFT || m_Direction == PLAYER_DIRECTION_DOWNER_LEFT || m_Direction == PLAYER_DIRECTION_UPPER_LEFT)
	{
		m_fHeight += ADD_WIDTH;
	}
	else if (m_Direction == PLAYER_DIRECTION_RIGHT || m_Direction == PLAYER_DIRECTION_DOWNER_RIGHT || m_Direction == PLAYER_DIRECTION_UPPER_RIGHT)
	{
		m_fHeight += ADD_WIDTH;
	}
	m_fDestWidth = m_fDefaultWidth = m_fWidth;
	m_fDefaultHeight = m_fDestHeight = m_fHeight;
}

//*****************************************************************************
// ライド関数
//*****************************************************************************
void CThreadSpecialSpeed::Ride(bool ride)
{

	if (ride)
	{
		if (m_Direction == PLAYER_DIRECTION_UP)
		{
			m_fDestHeight = m_fDefaultHeight + DOWN_POS_POW;
		}
		else if (m_Direction == PLAYER_DIRECTION_DOWN)
		{
			m_fDestHeight = m_fDefaultHeight + DOWN_POS_POW;
		}
		else if (m_Direction == PLAYER_DIRECTION_LEFT || m_Direction == PLAYER_DIRECTION_DOWNER_LEFT || m_Direction == PLAYER_DIRECTION_UPPER_LEFT)
		{
			m_fDestWidth = m_fDefaultWidth + DOWN_POS_POW;
		}
		else if (m_Direction == PLAYER_DIRECTION_RIGHT || m_Direction == PLAYER_DIRECTION_DOWNER_RIGHT || m_Direction == PLAYER_DIRECTION_UPPER_RIGHT)
		{
			m_fDestWidth = m_fDefaultWidth + DOWN_POS_POW;
		}
	}
	else
	{
		m_fDestHeight = m_fDefaultHeight;
		m_fDestWidth = m_fDefaultWidth;
	}
}

//*****************************************************************************
// 上下
//*****************************************************************************
void CThreadSpecialSpeed::UpDown(void)
{
	if (m_Direction == PLAYER_DIRECTION_UP || m_Direction == PLAYER_DIRECTION_DOWN)
	{
		if (m_fHeight < m_fDestHeight)
		{
			m_fHeight += MOVE_POW;
			m_fWidth -= MOVE_POW;
		}
		else if (m_fHeight >= m_fDestHeight)
		{
			m_fHeight -= MOVE_POW * 0.5f;
			m_fWidth += MOVE_POW * 0.5f;
		}
	}
	else
	{
		if (m_fWidth < m_fDestWidth)
		{
			m_fHeight -= MOVE_POW;
			m_fWidth += MOVE_POW;
		}
		else if (m_fWidth >= m_fDestWidth)
		{
			m_fHeight += MOVE_POW * 0.5f;
			m_fWidth -= MOVE_POW * 0.5f;
		}
	}
}

//*****************************************************************************
// プレイヤに当たった時に呼ばれる関数
//*****************************************************************************
void CThreadSpecialSpeed::HitPlayer(CPlayer* pPlayer)
{
	// プレイヤにはしない
}
//----EOF-------