//=============================================================================
//
// CPauseクラス [CPause.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CPause.h"
#include "../../INPUT/CInputKeyboard.h"
#include "../../FADE/CFade.h"
#include "../../MANAGER/CManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const D3DXVECTOR3	RETURN_CURSOL_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.56f, 500.f, 0);// 「戻る」選択中のカーソルの位置
static const D3DXVECTOR3	TITLE_CURSOL_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.56f, 380.f, 0);	// 「タイトルヘ」選択中のカーソルの位置
static const D3DXVECTOR3	RETRY_CURSOL_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.56f, 260.f, 0);	// 「リトライ」選択中のカーソルの位置
static const float			CURSOL_WIDTH = 60.f;											// カーソルの幅
static const float			CURSOL_HEIGHT = 60.f;											// カーソルの高さ
static const float			DEFAULT_CURSOL_LENGTH_OF_LEFT_TO_RIGHT = 550.f;					// カーソルの左右の距離

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
bool CPause::m_bPause = false;
bool CPause::m_bRetry = false;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPause ::CPause(LPDIRECT3DDEVICE9 *pDevice, int nPriority):CScene2D(pDevice, nPriority)
{
	m_bPause = false;
	m_pCursol = NULL;
	m_bReturnTititle = false;
	m_bRetry = false;
	m_nChoiceMenu = PAUSE_RETURN;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPause ::~CPause(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CPause :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock )
{
	// ポーズの初期化
	CScene2D::Init(pos, width, height, texType);
	
	// カーソルの初期化
	//m_pCursol = m_pCursol->Create(m_pD3DDevice, RETURN_CURSOL_POS, CURSOL_WIDTH, CURSOL_HEIGHT, TEXTURE_NULL, DEFAULT_CURSOL_LENGTH_OF_LEFT_TO_RIGHT);

	m_bPause = false;
	m_bRetry = false;

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CPause :: Uninit(void)
{
	CScene2D::Uninit();
	m_bPause = false;
	m_bRetry = false;
	m_nChoiceMenu = PAUSE_RETURN;
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CPause :: Update(void)
{
	if(m_bPause && !m_bRetry && !m_bReturnTititle)
	{
		// カーソル更新
		m_pCursol->SetDrawFlag(true);
		m_pCursol->Update();

		// カーソル上に移動
		if(CInputKeyboard::GetKeyboardTrigger(DIK_W)
			|| CInputKeyboard::GetKeyboardTrigger(DIK_UP))
		{
			m_nChoiceMenu--;

			// 一番上だったら一番下に移動
			if(m_nChoiceMenu < PAUSE_RETRY)
			{
				m_nChoiceMenu = PAUSE_RETURN;
			}
		}

		// カーソル下に移動
		if(CInputKeyboard::GetKeyboardTrigger(DIK_S)
			|| CInputKeyboard::GetKeyboardTrigger(DIK_DOWN))
		{
			m_nChoiceMenu++;

			// 一番下だったら一番上に移動
			if(m_nChoiceMenu >= PAUSE_MAX)
			{
				m_nChoiceMenu = PAUSE_RETRY;
			}
		}

		switch (m_nChoiceMenu)
		{
			// リトライ選択中
			case PAUSE_RETRY:

				// 座標指定
				m_pCursol->SetVertexPolygon(RETRY_CURSOL_POS, CURSOL_WIDTH, CURSOL_HEIGHT);
			
				// 決定キー押されたら
				if(CInputKeyboard::GetKeyboardTrigger(DIK_Z) 
					|| CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
				{
					// リトライ
					m_bRetry = true;
					m_pCursol->SetDrawFlag(false);
				}

			break;

			// タイトル選択中
			case PAUSE_TITLE:

				// 座標指定
				m_pCursol->SetVertexPolygon(TITLE_CURSOL_POS, CURSOL_WIDTH, CURSOL_HEIGHT);

				// 決定キー押されたら
				if(CInputKeyboard::GetKeyboardTrigger(DIK_Z)
					|| CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
				{
					// タイトルへ
					m_bReturnTititle = true;
					m_pCursol->SetDrawFlag(false);
				}

			break;

			// 戻る選択中
			case PAUSE_RETURN:

				// 座標指定
				m_pCursol->SetVertexPolygon(RETURN_CURSOL_POS, CURSOL_WIDTH, CURSOL_HEIGHT);

				// 決定キー押されたら
				if(CInputKeyboard::GetKeyboardTrigger(DIK_Z)
					|| CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
				{
					// ポーズ終了
					m_bPause = false;
					m_pCursol->SetDrawFlag(false);
				}
			break;
		}
	}
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CPause ::Draw(void)
{
	if(m_bPause)
	{
		CScene2D::Draw();
		m_pCursol->Draw();
	}
}
//----EOF----