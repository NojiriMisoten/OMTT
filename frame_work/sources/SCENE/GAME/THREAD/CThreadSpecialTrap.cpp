//=============================================================================
//
// CThreadSpecialTrapクラス [CThreadSpecialTrap.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CThreadSpecialTrap.h"
#include "../EFFECT/CEffectManager.h"
#include "../../../MANAGER/CManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// 寿命
static const short THREAD_TRAP_END_TIME = 300;

// 当たり判定の終わる時間
static const short THREAD_TRAP_HIT_END_TIME = 300;
// 当たり判 定幅,高さ
static const float THREAD_TRAP_HIT_WIDTH = 150;
static const float THREAD_TRAP_HIT_HEIGHT = 150;


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CThreadSpecialTrap::CThreadSpecialTrap(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CThreadBase(pDevice, priority, type)
{
	// 変数初期化
	m_ThreadType = THREAD_TYPE_TRAP;

	// この糸の固有ステータス初期化
	m_fWidth = THREAD_TRAP_HIT_WIDTH;
	m_fHeight = THREAD_TRAP_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = THREAD_TRAP_END_TIME;
	m_nHitStartTime = THREAD_TRAP_HIT_START_TIME;
	m_nHitEndTime = THREAD_TRAP_HIT_END_TIME;

	//// 糸を作る
	//m_pBulletAnime = CSceneAnime::Create(
	//	pDevice,
	//	m_vPos, THREAD_TRAP_HIT_WIDTH, THREAD_TRAP_HIT_HEIGHT,
	//	TEXTURE_THREAD, 1, 1, THREAD_TRAP_END_TIME);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CThreadSpecialTrap ::~CThreadSpecialTrap(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CThreadSpecialTrap::Init()
{
	CThreadBase::Init(m_vPos, THREAD_TRAP_HIT_WIDTH, THREAD_TRAP_HIT_HEIGHT, TEXTURE_THREAD);

	//// 糸の位置をセット
	//m_pBulletAnime->SetPos(m_vPos);

	//// 糸の色を変える
	//if (m_nPlayerNum < 0 || m_nPlayerNum > 3){
	//	return E_FAIL;
	//}
	//m_pBulletAnime->SetColorPolygon(PLAYER_COLOR[m_nPlayerNum]);

	return S_OK;  
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CThreadSpecialTrap::Uninit(void)
{
	CThreadBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CThreadSpecialTrap::Update(void)
{
	CThreadBase::Update();

	// カウントで消す
	if (m_nCount == THREAD_TRAP_END_TIME){
		Release();
	}
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CThreadSpecialTrap* CThreadSpecialTrap::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum, D3DXVECTOR3 pos)
{
	// 作成
	CThreadSpecialTrap* p = new CThreadSpecialTrap(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos;

	// 初期化
	p->Init();

	CEffectManager::CreateEffectTrap(pos, nPlayerNum);

	return p;
}

//*****************************************************************************
// 描画
//*****************************************************************************
void CThreadSpecialTrap::Draw(void)
{
	// Baseの表示を切ってm_pBulletAnimeの表示だけにする
//	CThreadBase::Draw();
}

//*****************************************************************************
// プレイヤに当たった時に呼ばれる関数
//*****************************************************************************
void CThreadSpecialTrap::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->AddMp(-4.f);
	pPlayer->AddHp(-1.f);
	// 鈍足
	pPlayer->SetSlowSpeed(true);
}

//----EOF-------