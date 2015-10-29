//=============================================================================
//
// CControllerクラス [ CController.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "CController.h"
#include <stdio.h>

//=============================================================================
// マクロ
//=============================================================================
static const int DEFAULT_KEY_CORD[CInputGamePad::CONTROLLER_BUTTON_MAX] = { 0, 1, 2, 3, 7};


//=============================================================================
// コンストラクタ
//=============================================================================
CController::CController(void)
{
	m_nID = -1;

	for (int i = 0; i < CInputGamePad::CONTROLLER_BUTTON_MAX; ++i)
	{
		m_nButtonKeyCord[i] = DEFAULT_KEY_CORD[i];
	}
	
}

//=============================================================================
// デストラクタ
//=============================================================================
CController::~CController(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
void CController::Init(int ID, const char* keyCofingFile)
{
	if (keyCofingFile)
	{
		LoadKeyCofingFile(keyCofingFile);
	}

	m_nID = ID;

	// キー情報セット
	UpdateKeyCord();
}

//=============================================================================
// 終了
//=============================================================================
void CController::Uninit(void)
{

}

//=============================================================================
// クリエイト
//=============================================================================
CController* CController::Create(int ID, const char* keyCofingFile)
{
	CController* p = new CController;

	p->Init(ID, keyCofingFile);

	return p;
}

//=============================================================================
// トリガーキー取得
//=============================================================================
bool CController::GetTriggerKey(CInputGamePad::CONTROLLER_BUTTON key)
{
	return CInputGamePad::GetGamePadTrigger(key, m_nID);
}

//=============================================================================
// プレスキー取得
//=============================================================================
bool CController::GetPressKey(CInputGamePad::CONTROLLER_BUTTON key)
{
	return CInputGamePad::GetGamePadPress(key, m_nID);
}

//=============================================================================
// リリースキー取得
//=============================================================================
bool CController::GetReleaseKey(CInputGamePad::CONTROLLER_BUTTON key)
{
	return CInputGamePad::GetGamePadRelease(key, m_nID);
}

//=============================================================================
// リピートキー取得
//=============================================================================
bool CController::GetRepeatKey(CInputGamePad::CONTROLLER_BUTTON key)
{
	return CInputGamePad::GetGamePadRepeat(key, m_nID);
}

//=============================================================================
// キーコフィングファイルのロード
//=============================================================================
void CController::LoadKeyCofingFile(const char* keyCofingFile)
{
	// 読み込み用ファイル
	FILE* fpr = fopen(keyCofingFile, "r");

	// 失敗したら
	if (fpr == NULL)
	{
//		fclose(fpr);
		return;
		//MessageBox(NULL, "キーコンフィグファイルの読み込みに失敗しました\n", "エラー", MB_OK | MB_ICONERROR);
	}

	// 成功したら
	else
	{
		for (int i = 0; i < CInputGamePad::CONTROLLER_BUTTON_MAX; ++i)
		{
			fscanf(fpr, "%d", &m_nButtonKeyCord[i]);
		}

	}
	fclose(fpr);
}

//=============================================================================
// キーコフィングファイルのセーブ
//=============================================================================
void CController::SaveKeyCofingFile(const char* keyCofingFile)
{
	// 書き込み用ファイル
	FILE* fpw = fopen(keyCofingFile, "w");

	// 失敗したら
	if (fpw == NULL)
	{
		fclose(fpw);
		return;
		//MessageBox(NULL, "キーコンフィグファイルの読み込みに失敗しました\n", "エラー", MB_OK | MB_ICONERROR);
	}

	// 成功したら
	else
	{
		for (int i = 0; i < CInputGamePad::CONTROLLER_BUTTON_MAX; ++i)
		{
			fprintf(fpw, "%d\n", m_nButtonKeyCord[i]);
		}

	}
	fclose(fpw);
}

//======================================
// キーコフィング情報の更新
//======================================
void CController::UpdateKeyCord(void)
{
	CInputGamePad::SetKeyCofingInfo(m_nID, &m_nButtonKeyCord[0]);
}

//-----EOF-----