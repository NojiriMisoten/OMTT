//=============================================================================
//
// CControllerManagerクラス [ CControllerManager.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "CControllerManager.h"
#include "CController.h"

//=============================================================================
// マクロ
//=============================================================================
static const char* KEY_COFING_FILE[CInputGamePad::MAX_PAD_NUM] = 
{
	"data/KEY_CONFIG/controller1.txt",
	"data/KEY_CONFIG/controller2.txt",
};

//=============================================================================
// staticメンバ変数
//=============================================================================
CController* CControllerManager::m_pController[CInputGamePad::MAX_PAD_NUM];

//=============================================================================
// コンストラクタ
//=============================================================================
CControllerManager::CControllerManager(void)
{
	for (int i = 0; i < CInputGamePad::MAX_PAD_NUM; ++i)
	{
		m_pController[i] = NULL;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CControllerManager::~CControllerManager(void)
{
	for (int i = 0; i < CInputGamePad::MAX_PAD_NUM; ++i)
	{
		m_pController[i] = NULL;
	}
}

//=============================================================================
// 初期化
//=============================================================================
void CControllerManager::Init(void)
{
	for (int i = 0; i < CInputGamePad::MAX_PAD_NUM; ++i)
	{
		m_pController[i] = CController::Create(i, KEY_COFING_FILE[i]);
	}
}

//=============================================================================
// 終了
//=============================================================================
void CControllerManager::Uninit(void)
{
	for (int i = 0; i < CInputGamePad::MAX_PAD_NUM; ++i)
	{
		if (m_pController[i])
		{
			m_pController[i]->Uninit();
			delete m_pController[i];
			m_pController[i] = NULL;
		}
	}
}

//=============================================================================
// クリエイト
//=============================================================================
CControllerManager* CControllerManager::Create(void)
{
	CControllerManager* p = new CControllerManager;

	p->Init();

	return p;
}

//=============================================================================
// トリガーキー取得
//=============================================================================
bool CControllerManager::GetTriggerKey(CInputGamePad::CONTROLLER_BUTTON key, int ID)
{
	return m_pController[ID]->GetTriggerKey(key);
}

//=============================================================================
// プレスキー取得
//=============================================================================
bool CControllerManager::GetPressKey(CInputGamePad::CONTROLLER_BUTTON key, int ID)
{
	return m_pController[ID]->GetPressKey(key);
}

//=============================================================================
// リリースキー取得
//=============================================================================
bool CControllerManager::GetReleaseKey(CInputGamePad::CONTROLLER_BUTTON key, int ID)
{
	return m_pController[ID]->GetReleaseKey(key);
}

//=============================================================================
// リピートキー取得
//=============================================================================
bool CControllerManager::GetRepeatKey(CInputGamePad::CONTROLLER_BUTTON key, int ID)
{
	return m_pController[ID]->GetRepeatKey(key);
}

//=============================================================================
// キーコフィングファイルのセーブ
//=============================================================================
void CControllerManager::SaveKeyCofingFile(void)
{
	for (int i = 0; i < CInputGamePad::MAX_PAD_NUM; ++i)
	{
		m_pController[i]->SaveKeyCofingFile(KEY_COFING_FILE[i]);
	}
}

//======================================
// キーコフィングファイルのセット
//======================================
void CControllerManager::SetKeyCord(int ID, CInputGamePad::CONTROLLER_BUTTON key, int cord)
{
	m_pController[ID]->SetKeyCord(key, cord);
}

//======================================
// キーコフィング情報の更新
//======================================
void CControllerManager::UpdateKeyCord(void)
{
	for (int i = 0; i < CInputGamePad::MAX_PAD_NUM; ++i)
	{
		m_pController[i]->UpdateKeyCord();
	}
}

//======================================
// キーコフィング情報のゲット
//======================================
int CControllerManager::GetKeyCord(int ID, CInputGamePad::CONTROLLER_BUTTON key)
{
	return m_pController[ID]->GetKeyCord(key);
}

//-----EOF-----