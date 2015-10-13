//=============================================================================
//
// CThreadNormalクラス [CThreadNormal.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CThreadNormal.h"
#include "../EFFECT/CEffectManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// 当たり判定の始まる時間
static const short THREAD_NORMAL_HIT_START_TIME = 1;
// 当たり判定の終わる時間
static const short THREAD_NORMAL_HIT_END_TIME = 30;
// 当たり判 定幅,高さ
static const float THREAD_NORMAL_HIT_WIDTH = 30;
static const float THREAD_NORMAL_HIT_HEIGHT = 30;

// 弾の移動量
static const float THREAD_NORMAL_VEL = 10.f;


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CThreadNormal::CThreadNormal(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CThreadBase(pDevice, priority, type)
{
	// 変数初期化
	m_ThreadType = THREAD_TYPE_NORMAL;

	// この糸の固有ステータス初期化
	m_fWidth = THREAD_NORMAL_HIT_WIDTH;
	m_fHeight = THREAD_NORMAL_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = THREAD_NORMAL_END_TIME;
	m_nHitStartTime = THREAD_NORMAL_HIT_START_TIME;
	m_nHitEndTime = THREAD_NORMAL_HIT_END_TIME;

	// 弾を作る
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CThreadNormal ::~CThreadNormal(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CThreadNormal::Init()
{
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CThreadNormal::Uninit(void)
{
	CThreadBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CThreadNormal::Update(void)
{

	// 弾の移動
	m_vPos += m_vPlayerDirection * THREAD_NORMAL_VEL;

	// カウントで消す
	if (m_nCount == THREAD_NORMAL_END_TIME){
		Release();
	}
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CThreadNormal* CThreadNormal::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum, D3DXVECTOR3 pos, D3DXVECTOR3 direction)
{
	// 作成
	CThreadNormal* p = new CThreadNormal(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos;
	p->m_vPlayerDirection = direction;

	// 初期化
	p->Init();

	// エフェクト生成
	CEffectManager::CreateEffect(pos, EFFECT_NORMAL_ATTACK_THREAD, direction);

	return p;
}

//*****************************************************************************
// 描画
//*****************************************************************************
void CThreadNormal::Draw(void)
{
}

//*****************************************************************************
// プレイヤに当たった時に呼ばれる関数
//*****************************************************************************
void CThreadNormal::HitPlayer(CPlayer* pPlayer)
{
	// 鈍足にしてダメージは無し
	pPlayer->SetSlowSpeed(true);

	CManager::PlaySoundA(SOUND_LABEL_SE_SPEED_DOWN);
}

//----EOF-------