//=============================================================================
//
// CFieldクラス [CField.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "CField.h"

//=======================================================================
// マクロ
//=======================================================================
static const float DOWN_POS_POW = 5.0f;
static const float MOVE_POW = DOWN_POS_POW / 10.f;

//=======================================================================
// コンストラクタ
//=======================================================================
CField::CField(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) :CScene2D(pDevice, nPriority, m_objType)
{

}

//=======================================================================
// デストラクタ
//=======================================================================
CField::~CField()
{
}

//=======================================================================
// 終了
//=======================================================================
void CField::Uninit(void)
{
	CScene2D::Uninit();
}

//=======================================================================
// 更新
//=======================================================================
void CField::Update(void)
{
	if (m_vPos.y < m_vDestPos.y)
	{
		m_vPos.y += MOVE_POW;
	}
	else if (m_vPos.y > m_vDestPos.y)
	{
		m_vPos.y -= MOVE_POW * 0.5f;
	}
}

//=======================================================================
// 描画
//=======================================================================
void CField::Draw(void)
{
	CScene2D::Draw();
}

//=======================================================================
// クリエイト関数
//=======================================================================
CField* CField::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	CField* p = new CField(pDevice);

	p->Init(pos, width, height, texType);

	return p;
}

//=======================================================================
// 初期化関数
//=======================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	CScene2D::Init(pos, width, height, texType);

	// 当たり判定ステータスセット
	SetJudgeParam(pos, width, height, texType);
	
	m_vDestPos = m_vDefaultPos = pos;

	return TRUE;
}

//=======================================================================
// 乗ってる
//=======================================================================
void CField::Ride(bool ride)
{
	if (ride)
	{
		m_vDestPos.y = m_vDefaultPos.y + DOWN_POS_POW;
	}
	else
	{
		m_vDestPos.y = m_vDefaultPos.y;
	}
}

//=======================================================================
// 当たり判定ステータスセット関数
//=======================================================================
void CField::SetJudgeParam(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	switch (texType)
	{
		// 浮島
	case TEXTURE_FIELD_01:
		m_fJudgeWidth = width * 0.8f;
		m_fJudgeHeight = height * 0.6f;
		m_vJudgePos = pos;
		m_vJudgePos.y -= m_fJudgeHeight * 0.4f;
		break;

		// 宝置くやつ
	case TEXTURE_FIELD_02:
		m_fJudgeWidth = width * 0.8f;
		m_fJudgeHeight = height * 0.6f;
		m_vJudgePos = pos;
		m_vJudgePos.y -= m_fJudgeHeight * 0.4f;
		break;

		// スタート兼ゴール
	case TEXTURE_FIELD_03:
		m_fJudgeWidth = width;
		m_fJudgeHeight = height * 0.8f;
		m_vJudgePos = pos;
		m_vJudgePos.y -= m_fJudgeHeight * 0.2f;
		break;

	default:
		m_fJudgeWidth = width;
		m_fJudgeHeight = height;
		m_vJudgePos = pos;
		break;
	}
}
//----EOF----