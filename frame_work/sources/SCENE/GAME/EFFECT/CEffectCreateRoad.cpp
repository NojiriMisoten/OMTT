//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectCreateRoad.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int CREATE_ROAD_COUNT_MAX = 1;
static const float CREATE_ROAD_WIDTH = 100;
static const float CREATE_ROAD_HEIGHT = 100;
static const TEXTURE_TYPE CREATE_ROAD_TEXTURE = TEXTURE_TREASURE_KIRAKIRA;
static const int CREATE_ROAD_TEXTURE_X = 8;
static const int CREATE_ROAD_TEXTURE_Y = 1;
static const int CREATE_ROAD_TEXTURE_LOOP = 100;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectCreateRoad::CEffectCreateRoad(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectCreateRoad ::~CEffectCreateRoad(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectCreateRoad::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = CREATE_ROAD_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectCreateRoad::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectCreateRoad::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			CREATE_ROAD_WIDTH, CREATE_ROAD_HEIGHT,
			CREATE_ROAD_TEXTURE, CREATE_ROAD_TEXTURE_X, CREATE_ROAD_TEXTURE_Y,
			CREATE_ROAD_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();

#ifdef _DEBUG
	CDebugProc::Print("道作成エフェクトなう\n");
#endif
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectCreateRoad* CEffectCreateRoad::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectCreateRoad* p = new CEffectCreateRoad(pDevice);

	p->Init(pos);

	return p;
}

//EOF