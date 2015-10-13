//=============================================================================
//
// COptionクラス [COption.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _COPTION_H_
#define _COPTION_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"
#include "../../CONTROLLER/CControllerManager.h"
#include "../../INPUT/CInputGamePad.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CSceneNumber;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class COption : public CPhase
{
public:
	// コンストラクタ
	COption(void);

	// デストラクタ
	~COption(void);

	//=================================================================
	// 初期化関数
	// 引数: シーンのモード, デバイス
	//=================================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//=================================================================
	// クリエイト関数
	// 引数: シーンのモード, デバイス
	// 戻り値: このシーンのアドレス
	//=================================================================
	static COption* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

private:
	// モード
	typedef enum
	{
		OPTION_MODE_KEYCONFIG,
		OPTION_MODE_NEXT_SCENE,
		OPTION_MODE_MAX
	}OPTION_MODE;

	//=================================================================
	// キーコンフィグ関数
	//=================================================================
	void KeyConfig(int padID, CInputGamePad::PAD_BUTTON button);

	//=================================================================
	// 桁割出関数
	// 引数:(out)１０の位、(out)１の位、元の数字
	//=================================================================
	void CuclDigit(int* ten, int* one, int org);

	//=================================================================
	// カーソル更新関数
	//=================================================================
	void UpdateCursol(void);

	// 動く背景の数
	static const int MOVE_BG_NUM = 2;

	// キーコード表示用
	typedef struct
	{
		CSceneNumber* DigitTen;	// 10の位
		CSceneNumber* DigitOne;	// 1の位
	}PRINT_KEY_CORD;

	OPTION_MODE					m_mode;
	CInputGamePad::PAD_BUTTON	m_setButton;											// 何のボタンか
	int							m_nControllerID;										// コントローラー番号
	CScene2D*					m_pOptionMenu[CControllerManager::MAX_CONTROLLER_NUM];	// オプションメニュー
	CScene2D*					m_pOptionBG;											// オプションBG
	CScene2D*					m_pOptionBGMove[MOVE_BG_NUM];							// オプション動くBG
	PRINT_KEY_CORD				m_KeyNumber[CControllerManager::MAX_CONTROLLER_NUM][CInputGamePad::PAD_KEY_MAX];
	CScene2D*					m_pCursol[CControllerManager::MAX_CONTROLLER_NUM];		// カーソル
	float						m_fCursolAlfaRad;										// カーソルの透明値
	int							m_nSelectCounter[CControllerManager::MAX_CONTROLLER_NUM];// どこを選んでいるか
	CScene2D*					m_pReturn;												// 戻る
};

#endif
//----EOF----