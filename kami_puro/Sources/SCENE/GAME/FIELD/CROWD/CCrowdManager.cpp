//=============================================================================
//
// CCrowdManagerクラス [CCrowdManager.cpp]
// Author : 塚本俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CCrowdManager.h"
#include "CCrowd.h"
#include "../../../MANAGER/CManager.h"
#include "../../../RENDERER/CRenderer.h"
#include "../../../CAMERA/CameraManager.h"
#include "../../../SHADER/CShader.h"

//*****************************************************************************
// 定数
//*****************************************************************************
// 2Fのポリゴンの大きさ
static const float UPPER_WIDTH = 350;
static const float UPPER_HEIGHT = 50;
// 1Fのポリゴンの大きさ
static const float LOWER_WIDTH = 140;
static const float LOWER_HEIGHT = 60;

// 座標きめるやつううううう　つかれたあああ
static const D3DXVECTOR3 UPPWER_FRONT_POS_START = D3DXVECTOR3(-762, 50, 618);
static const D3DXVECTOR3 UPPWER_FRONT_POS_OFFSET = D3DXVECTOR3(381, 20, 52);
static const D3DXVECTOR3 UPPWER_LEFT_POS_START = D3DXVECTOR3(-650, 70, -800);
static const D3DXVECTOR3 UPPWER_LEFT_POS_OFFSET = D3DXVECTOR3(-50, 18, 400);
static const D3DXVECTOR3 UPPWER_RIGHT_POS_START = D3DXVECTOR3(792, 160, 730);
static const D3DXVECTOR3 UPPWER_RIGHT_POS_OFFSET = D3DXVECTOR3(-30, -18, -375);
static const D3DXVECTOR3 LOWER_FRONT_LEFT_POS_START = D3DXVECTOR3(-332, -30, 290);
static const D3DXVECTOR3 LOWER_FRONT_RIGHT_POS_START = D3DXVECTOR3(160, -30, 290);
static const D3DXVECTOR3 LOWER_FRONT_POS_OFFSET = D3DXVECTOR3(160, 0, 52);
static const D3DXVECTOR3 LOWER_RIGHT_POS_START = D3DXVECTOR3(322, -30, 470);
static const D3DXVECTOR3 LOWER_RIGHT_POS_OFFSET = D3DXVECTOR3(57, 0, -160);
static const D3DXVECTOR3 LOWER_LEFT_POS_START = D3DXVECTOR3(-322, -30, 470);
static const D3DXVECTOR3 LOWER_LEFT_POS_OFFSET = D3DXVECTOR3(-57, 0, -160);
static const D3DXVECTOR3 LOWER_BACK_POS_START = D3DXVECTOR3(-332, -30, 470);
static const D3DXVECTOR3 LOWER_BACK_POS_OFFSET = D3DXVECTOR3(-160, 0, -52);

// リング後方の観客の位置
static const D3DXVECTOR3 CROWD_POS_BACK = D3DXVECTOR3(0, 30, -120);

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCrowdManager::CCrowdManager(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	m_pD3DDevice = pDevice;
	m_pManager = pManager;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCrowdManager ::~CCrowdManager(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
void CCrowdManager::Init()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posStart;
	D3DXVECTOR3 posOffset;

	// 2Fの正面
	posStart = UPPWER_FRONT_POS_START;
	posOffset = UPPWER_FRONT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < UPPER_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pUpperFront[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				UPPER_WIDTH, UPPER_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
			m_pUpperFront[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}

	// 2Fの左側
	posStart = UPPWER_LEFT_POS_START;
	posOffset = UPPWER_LEFT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < UPPER_SIDE_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_SIDE_CHAIR_ROW_MAX; n++){
			pos.z += posOffset.z;
			m_pUpperLeft[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				UPPER_WIDTH, UPPER_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
			m_pUpperLeft[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));
		}
		pos.y += posOffset.y;
		pos.x += posOffset.x;
		pos.z = posStart.z;
	}

	// 2Fの右側
	posStart = UPPWER_RIGHT_POS_START;
	posOffset = UPPWER_RIGHT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < UPPER_SIDE_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_SIDE_CHAIR_ROW_MAX; n++){
			pos.z += posOffset.z;
			m_pUpperRight[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				UPPER_WIDTH, UPPER_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
			m_pUpperRight[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));
		}
		pos.y += posOffset.y;
		pos.x += posOffset.x;
		pos.z = posStart.z;
	}

	// 1Fの前側の左の観客
	posStart = LOWER_FRONT_LEFT_POS_START;
	posOffset = LOWER_FRONT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerFrontLeft[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
			m_pLowerFrontLeft[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}
	// 左端の角
	m_pLowerFrontLeft[0][1]->SetWidth(LOWER_WIDTH * 0.5f);
	m_pLowerFrontLeft[0][1]->AddPos(D3DXVECTOR3(15,0,0));
	m_pLowerFrontLeft[0][0]->SetWidth(LOWER_WIDTH * 0.25f);
	m_pLowerFrontLeft[0][0]->AddPos(D3DXVECTOR3(20, 0, 0));

	// 1Fの前側の右の観客
	posStart = LOWER_FRONT_RIGHT_POS_START;
	posOffset = LOWER_FRONT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerFrontRight[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
			m_pLowerFrontRight[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, 0, 0));
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}
	// 右端の角
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][1]->SetWidth(LOWER_WIDTH * 0.5f);
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][1]->AddPos(D3DXVECTOR3(-15, 0, 0));
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][0]->SetWidth(LOWER_WIDTH * 0.25f);
	m_pLowerFrontRight[LOWER_FRONT_CHAIR_COL_MAX - 1][0]->AddPos(D3DXVECTOR3(-20, 0, 0));

	// 1Fの右側の観客
	posStart = LOWER_RIGHT_POS_START;
	posOffset = LOWER_RIGHT_POS_OFFSET;

	pos = posStart;
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerRight[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
			m_pLowerRight[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0));
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}
	// 角
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->SetWidth(LOWER_WIDTH * 0.5f);
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->AddPos(D3DXVECTOR3(-15, 0, 0));
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->SetWidth(LOWER_WIDTH * 0.25f);
	m_pLowerRight[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->AddPos(D3DXVECTOR3(-20, 0, 0));

	// 1Fの左側の観客
	posStart = LOWER_LEFT_POS_START;
	posOffset = LOWER_LEFT_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerLeft[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
			m_pLowerLeft[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, -D3DX_PI * 0.5f, 0));
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}
	// 角
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->SetWidth(LOWER_WIDTH * 0.5f);
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][1]->AddPos(D3DXVECTOR3(-15, 0, 0));
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->SetWidth(LOWER_WIDTH * 0.25f);
	m_pLowerLeft[LOWER_RIGHT_CHAIR_COL_MAX - 1][0]->AddPos(D3DXVECTOR3(-20, 0, 0));

	// 1Fの後ろの観客
	posStart = LOWER_BACK_POS_START;
	posOffset = LOWER_BACK_POS_OFFSET;
	pos = posStart;
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			pos.y += posOffset.y;
			pos.z += posOffset.z;
			m_pLowerBack[i][n] = CCrowd::Create(m_pD3DDevice,
				D3DXVECTOR3(pos),
				LOWER_WIDTH, LOWER_HEIGHT, TEXTURE_CROWD_GAGE_HUMAN, m_pManager);
			m_pLowerBack[i][n]->SetRot(D3DXVECTOR3(-D3DX_PI * 0.5f, D3DX_PI, 0));
		}
		pos.x += posOffset.x;
		pos.y = posStart.y;
		pos.z = posStart.z;
	}

}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CCrowdManager::Uninit(void)
{
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CCrowdManager::Update(void)
{
}


//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CCrowdManager* CCrowdManager::Create(LPDIRECT3DDEVICE9 *pDevice, CManager *pManager)
{
	// 作成
	CCrowdManager* p = new CCrowdManager(pDevice, pManager);

	// 初期化
	p->Init();

	return p;

}

//*****************************************************************************
// 描画しない
//*****************************************************************************
void CCrowdManager::SetImvisible(void)
{
	for (int i = 0; i < UPPER_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_CHAIR_ROW_MAX; n++){
			m_pUpperFront[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerFrontLeft[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerFrontRight[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerRight[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerLeft[i][n]->SetDrawFlag(false);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerBack[i][n]->SetDrawFlag(false);
		}
	}
}

//*****************************************************************************
// 描画する
//*****************************************************************************
void CCrowdManager::SetVisible(void)
{
	for (int i = 0; i < UPPER_CHAIR_COL_MAX; i++){
		for (int n = 0; n < UPPER_CHAIR_ROW_MAX; n++){
			m_pUpperFront[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerFrontLeft[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_FRONT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerFrontRight[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerRight[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerLeft[i][n]->SetDrawFlag(true);
		}
	}
	for (int i = 0; i < LOWER_RIGHT_CHAIR_COL_MAX; i++){
		for (int n = 0; n < LOWER_CHAIR_ROW_MAX; n++){
			m_pLowerBack[i][n]->SetDrawFlag(true);
		}
	}
}
//----EOF----