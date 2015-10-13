//=============================================================================
//
// COptionクラス [COption.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "COption.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneNumber.h"
//*****************************************************************************
// マクロ
//*****************************************************************************
static const float		 OPTION_MENU_WIDTH = SCREEN_WIDTH * 0.25f * 0.9f;	// オプションメニューの幅
static const float		 OPTION_MENU_HEIGHT = SCREEN_HEIGHT * 0.8f;			// オプションメニューの高さ

// オプションメニューの座標
static const D3DXVECTOR3 OPTION_MENU_POS[CControllerManager::MAX_CONTROLLER_NUM] =
{
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.375f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.625f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.875f, SCREEN_HEIGHT * 0.5f, 0),
};
static const D3DXVECTOR3 OPTION_BG_POS((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.f);// 背景の座標
static const float		 MOVE_BG_POS_X = 1.5f;								// 背景の動く速度

// キーコード表示のベース座標
static const D3DXVECTOR3 BASE_KEY_CORD_POS[CInputGamePad::PAD_KEY_MAX - 2] =
{
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f - OPTION_MENU_HEIGHT * 0.38f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f - OPTION_MENU_HEIGHT * 0.22f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f - OPTION_MENU_HEIGHT * 0.09f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f + OPTION_MENU_HEIGHT * 0.05f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f + OPTION_MENU_HEIGHT * 0.21f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.125f + OPTION_MENU_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f + OPTION_MENU_HEIGHT * 0.37f, 0),
};
static const float		KEY_CORD_HEIGHT = OPTION_MENU_HEIGHT / 16.f;	// キーコードの高さ
static const float		KEY_CORD_WIDTH = OPTION_MENU_WIDTH * 0.125f;	// キーコードの幅

// カーソルのデフォルト座標
static const D3DXVECTOR3 DEFAULT_CURSOL_POS[CControllerManager::MAX_CONTROLLER_NUM] =
{
	D3DXVECTOR3(OPTION_MENU_POS[0].x, BASE_KEY_CORD_POS[0].y, 0),
	D3DXVECTOR3(OPTION_MENU_POS[1].x, BASE_KEY_CORD_POS[0].y, 0),
	D3DXVECTOR3(OPTION_MENU_POS[2].x, BASE_KEY_CORD_POS[0].y, 0),
	D3DXVECTOR3(OPTION_MENU_POS[3].x, BASE_KEY_CORD_POS[0].y, 0),
};
static const float MOV_POW_CURSOL_ALFA = D3DX_PI * 0.06f;
static const float DEFAULT_CURSOL_ALFA = D3DX_PI * 0.5f;

static const D3DXVECTOR3 DEFAULT_RETURN_LOGO_POS(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.95f, 0.f);	// 戻るアイコンの位置
static const float RETURN_LOGO_WIDTH = KEY_CORD_WIDTH * 4.f;									// 戻るアイコンの幅
static const float RETURN_LOGO_HEIGHT = KEY_CORD_HEIGHT * 4.f;										// 戻るアイコンの高さ

// プレイヤー番号の座標
static const D3DXVECTOR3 DEFAULT_PLAYER_NUMBER_POS[CControllerManager::MAX_CONTROLLER_NUM] =
{
	D3DXVECTOR3(OPTION_MENU_POS[0].x, SCREEN_HEIGHT * 0.04f, 0),
	D3DXVECTOR3(OPTION_MENU_POS[1].x, SCREEN_HEIGHT * 0.04f, 0),
	D3DXVECTOR3(OPTION_MENU_POS[2].x, SCREEN_HEIGHT * 0.04f, 0),
	D3DXVECTOR3(OPTION_MENU_POS[3].x, SCREEN_HEIGHT * 0.04f, 0),
};
// プレイヤー番号のUV座標
static const UV_INDEX DEFAULT_PLAYER_NUMBER_UV[CControllerManager::MAX_CONTROLLER_NUM] =
{
	{ 0.f, 0.5f, 0.f, 0.5f},
	{ 0.5f, 1.f, 0.f, 0.5f},
	{ 0.f, 0.5f, 0.5f, 1.f},
	{ 0.5f, 1.f, 0.5f, 1.f},
};
static const float PLAYER_NUMBER_WIDTH = KEY_CORD_WIDTH * 2.f;				// プレイヤー番号の幅
static const float PLAYER_NUMBER_HEIGHT = KEY_CORD_HEIGHT * 2.f;				// プレイヤー番号の高さ
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
COption::COption(void)
{
	m_mode = OPTION_MODE_KEYCONFIG;
	m_setButton = CInputGamePad::PAD_KEY_CHANGE_NORMAL;
	m_nControllerID = 0;
	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		m_pOptionMenu[i] = NULL;
		m_pCursol[i] = NULL;
		m_nSelectCounter[i] = 0;
	}
	m_pOptionBG = NULL;
	for (int i = 0; i < MOVE_BG_NUM; ++i)
	{
		m_pOptionBGMove[i] = NULL;
	}

	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		for (int j = 0; j < CInputGamePad::PAD_KEY_MAX; ++j)
		{
			m_KeyNumber[i][j].DigitOne = NULL;
			m_KeyNumber[i][j].DigitTen = NULL;
		}
	}
	m_pReturn = NULL;
	m_fCursolAlfaRad = DEFAULT_CURSOL_ALFA;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
COption ::~COption(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void COption::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// モードの初期化
	m_mode = OPTION_MODE_KEYCONFIG;

	// 背景
	m_pOptionBG = CScene2D::Create(pDevice, OPTION_BG_POS, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, TEXTURE_OPTION_BG);
	for (int i = 0; i < MOVE_BG_NUM; ++i)
	{
		m_pOptionBGMove[i] = CScene2D::Create(pDevice, OPTION_BG_POS, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, TEXTURE_BG_1);
		m_pOptionBGMove[i]->SetPos(OPTION_BG_POS.x + (float)SCREEN_WIDTH * (float)i, OPTION_BG_POS.y, OPTION_BG_POS.z);
		m_pOptionBGMove[i]->SetColorPolygon(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
	}

	// メニューの表示
	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		m_pOptionMenu[i] = CScene2D::Create(pDevice, OPTION_MENU_POS[i], OPTION_MENU_WIDTH, OPTION_MENU_HEIGHT, TEXTURE_OPTION_MENU);
	}

	// キーコード表示
	int buttonLoopMax = CInputGamePad::PAD_KEY_MAX - 2;
	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		for (int j = 0; j < buttonLoopMax; ++j)
		{
			CInputGamePad::PAD_BUTTON button = (CInputGamePad::PAD_BUTTON)j;
			int keyCord = CControllerManager::GetKeyCord(i, button);
			int ten = 0;
			int one = 0;
			CuclDigit(&ten, &one, keyCord);
			D3DXVECTOR3 pos = BASE_KEY_CORD_POS[j];
			pos.x += i * (OPTION_MENU_WIDTH + KEY_CORD_WIDTH * 0.9f);
			m_KeyNumber[i][j].DigitTen = CSceneNumber::Create(pDevice, pos, KEY_CORD_WIDTH, KEY_CORD_HEIGHT, ten, TEXTURE_NUMBER_WHITE);
			pos.x += KEY_CORD_WIDTH;
			m_KeyNumber[i][j].DigitOne = CSceneNumber::Create(pDevice, pos, KEY_CORD_WIDTH, KEY_CORD_HEIGHT, one, TEXTURE_NUMBER_WHITE);
		}
	}

	// 戻る表示
	m_pReturn = CScene2D::Create(pDevice, DEFAULT_RETURN_LOGO_POS, RETURN_LOGO_WIDTH, RETURN_LOGO_HEIGHT, TEXTURE_RETURN);

	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		// カーソル表示
		m_pCursol[i] = CScene2D::Create(pDevice, DEFAULT_CURSOL_POS[i], OPTION_MENU_WIDTH * 0.95f, KEY_CORD_HEIGHT * 1.6f, TEXTURE_CURSOL, TYPE_PRIORITY_GOAL);
		m_pCursol[i]->SetColorPolygon(D3DXCOLOR(1.f, 1.f, 1.f, sinf(m_fCursolAlfaRad)));

		// プレイヤー番号の表示
		CScene2D* p = NULL;
		p = CScene2D::Create(pDevice, DEFAULT_PLAYER_NUMBER_POS[i], PLAYER_NUMBER_WIDTH, PLAYER_NUMBER_HEIGHT, TEXTURE_PLAYER_NUM, TYPE_PRIORITY_GOAL);
		UV_INDEX uv = DEFAULT_PLAYER_NUMBER_UV[i];
		p->SetUV(&uv);
	}
}

//*****************************************************************************
// 終了
//*****************************************************************************
void COption::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void COption::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	// 背景の更新
	for (int i = 0; i < MOVE_BG_NUM; ++i)
	{
		D3DXVECTOR3 pos = m_pOptionBGMove[i]->GetPos();
		pos.x -= MOVE_BG_POS_X;
		if (pos.x < -SCREEN_WIDTH * 0.5f)
		{
			pos.x = SCREEN_WIDTH * 1.5f;
		}
		m_pOptionBGMove[i]->SetPos(pos);
	}

	// カーソルの更新
	UpdateCursol();

	// 次のシーンに行くなら
	if (m_mode == OPTION_MODE_NEXT_SCENE)
	{
		// キーコンフィグ情報のセーブ
		CControllerManager::SaveKeyCofingFile();

		// キーコンフィグ情報の更新
		CControllerManager::UpdateKeyCord();

		// フェードアウト開始
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// キャラ選択へ戻る
		m_pManager->SetNextPhase(MODE_PHASE_CHARA_SELECT);
	}

	// デバッグでENTERでキャラ選択へ
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN)){
		m_mode = OPTION_MODE_NEXT_SCENE;
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
COption* COption::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	COption* pTitle = NULL;

	// 作成
	pTitle = new COption;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
// キーコンフィグ
//*****************************************************************************
void COption::KeyConfig(int padID, CInputGamePad::PAD_BUTTON button)
{
	int pCord = -1;
	if (CInputGamePad::CheckConectPad(padID))
	{
		if (CInputGamePad::CheckTriggerAnyKey(padID, &pCord))
		{
			// キーコードセット
			CControllerManager::SetKeyCord(padID, button, pCord);

			// キーコード変更
			int keyCord = pCord;
			int ten = 0;
			int one = 0;
			CuclDigit(&ten, &one, keyCord);
			m_KeyNumber[padID][button].DigitTen->SetNumber(ten);
			m_KeyNumber[padID][button].DigitOne->SetNumber(one);

			CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);

		}
	}
}

//=================================================================
// 桁割出関数
//=================================================================
void COption::CuclDigit(int* ten, int* one, int org)
{
	if (org > 99)
	{
		org = 99;
	}
	if (org < 0)
	{
		org = 0;
	}
	*ten = org / 10;
	*one = org - (*ten * 10);

}

//=================================================================
// カーソル更新関数
//=================================================================
void COption::UpdateCursol(void)
{
	for (int i = 0; i < CControllerManager::MAX_CONTROLLER_NUM; ++i)
	{
		if (!CInputGamePad::CheckConectPad(i))
		{
			continue;
		}
		if (CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_UP, i))
		{
			m_nSelectCounter[i]--;
			if (m_nSelectCounter[i] < (int)CInputGamePad::PAD_KEY_CHANGE_NORMAL)
			{
				m_nSelectCounter[i] = (int)CInputGamePad::PAD_KEY_START;
			}
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}
		else if (CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_DOWN, i))
		{
			m_nSelectCounter[i]++;
			if (m_nSelectCounter[i] > (int)CInputGamePad::PAD_KEY_START)
			{
				m_nSelectCounter[i] = (int)CInputGamePad::PAD_KEY_CHANGE_NORMAL;
			}
			CManager::PlaySoundA(SOUND_LABEL_SE_CURSOL_MOVE);
		}

		// 戻る選択中なら
		if (m_nSelectCounter[i] == (int)CInputGamePad::PAD_KEY_START)
		{
			D3DXVECTOR3 pos(DEFAULT_RETURN_LOGO_POS);
			m_pCursol[i]->SetPos(pos);
			m_pCursol[i]->SetWidth(RETURN_LOGO_WIDTH);
			int pCord = -1;
			
			if (CInputGamePad::CheckTriggerAnyKey(i, &pCord))
			{
				// 戻る押したら
				m_mode = OPTION_MODE_NEXT_SCENE;
				CManager::PlaySoundA(SOUND_LABEL_SE_ENTER);
			}
			
		}

		// それ以外なら
		else
		{
			D3DXVECTOR3 pos = m_pCursol[i]->GetPos();
			pos.y = BASE_KEY_CORD_POS[m_nSelectCounter[i]].y;
			pos.x = DEFAULT_CURSOL_POS[i].x;
			m_pCursol[i]->SetPos(pos);
			m_pCursol[i]->SetWidth(OPTION_MENU_WIDTH * 0.95f);

			// キーコンフィグ
			KeyConfig(i, (CInputGamePad::PAD_BUTTON)m_nSelectCounter[i]);
		}
	}
}

//----EOF----