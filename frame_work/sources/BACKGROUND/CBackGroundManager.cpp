//=============================================================================
//
// CBackGroundManagerクラス [CBackGroundManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "CBackGroundManager.h"
#include "../SCENE/CSCENE/CScene2D.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const D3DXVECTOR3 BG_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);
static const D3DXVECTOR3 BG_POS_TWICE_L = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT * 0.5f, 0);
static const D3DXVECTOR3 BG_POS_TWICE_R = D3DXVECTOR3(0.0f, (float)SCREEN_HEIGHT * 0.5f, 0);
static const UV_INDEX BG_UV_TWICE = { 0.0f, 2.0f, 0.0f, 1.0f };
static const float BG_WIDTH = SCREEN_WIDTH;
static const float BG_HEIGHT = SCREEN_HEIGHT;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CBackGroundManager::CBackGroundManager(LPDIRECT3DDEVICE9 *pDevice)
{
	// 初期化
	m_pDevice = pDevice;
	m_nBackGroundNum = 0;

	for (int i = 0; i < BACKGROUND_MAX; i++){
		m_vPosition[i] = D3DXVECTOR3(0, 0, 0);
		m_bAnimation[i] = false;
		m_pSprite[i] = NULL;
		m_bDirection[i] = BG_DIRECTION_NONE;
	}
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CBackGroundManager ::~CBackGroundManager(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CBackGroundManager::Init()
{
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CBackGroundManager::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CBackGroundManager::Update(void){

	// 生成した背景の更新
	for (int i = 0; i < BACKGROUND_MAX; i++){
		// 実体がないかアニメーションしないものは更新しない
		if (!(m_bAnimation[i]) || !(m_pSprite[i]))	continue;

		// 移動
		m_vPosition[i].x += m_fVelocityX[i];
		m_pSprite[i]->SetPos(m_vPosition[i]);

		// 方向ごとにループ処理
		if (m_bDirection[i] == BG_DIRECTION_LEFT){
			if (m_vPosition[i].x <= 0){
				m_vPosition[i] = BG_POS_TWICE_L;
			}
		}
		else{
			if (m_vPosition[i].x >= SCREEN_WIDTH){
				m_vPosition[i] = BG_POS_TWICE_R;
			}
		}
	}
}

//*****************************************************************************
// 背景の生成
//*****************************************************************************
bool CBackGroundManager::CreateBG(TEXTURE_TYPE texture){

	// 最大まで作っていたら失敗
	if (m_nBackGroundNum == BACKGROUND_MAX) return false;

	m_pSprite[m_nBackGroundNum] = new CScene2D(m_pDevice, TYPE_PRIORITY_BG);
	m_pSprite[m_nBackGroundNum]->Init(BG_POS, BG_WIDTH, BG_HEIGHT, texture, 1, 1);
	m_vPosition[m_nBackGroundNum] = BG_POS;

	m_nBackGroundNum++;

	return true;
}

//*****************************************************************************
// 背景の生成 UVアニメーションする方
//*****************************************************************************
bool CBackGroundManager::CreateBG(TEXTURE_TYPE texture, float velocity){

	// 最大まで作っていたら失敗
	if (m_nBackGroundNum == BACKGROUND_MAX) return false;

	m_pSprite[m_nBackGroundNum] = new CScene2D(m_pDevice, TYPE_PRIORITY_BG);
	if (velocity < 0){
		m_bDirection[m_nBackGroundNum] = BG_DIRECTION_LEFT;
		m_pSprite[m_nBackGroundNum]->Init(BG_POS_TWICE_L, BG_WIDTH * 2, BG_HEIGHT, texture, 1, 1);
		m_vPosition[m_nBackGroundNum] = BG_POS_TWICE_L;
	}
	else{
		m_bDirection[m_nBackGroundNum] = BG_DIRECTION_RIGHT;
		m_pSprite[m_nBackGroundNum]->Init(BG_POS_TWICE_R, BG_WIDTH * 2, BG_HEIGHT, texture, 1, 1);
		m_vPosition[m_nBackGroundNum] = BG_POS_TWICE_R;
	}
	m_pSprite[m_nBackGroundNum]->SetUV(const_cast<UV_INDEX*>(&BG_UV_TWICE));
	m_fVelocityX[m_nBackGroundNum] = velocity;
	m_bAnimation[m_nBackGroundNum] = true;

	m_nBackGroundNum++;
	return true;
}

//----EOF-------