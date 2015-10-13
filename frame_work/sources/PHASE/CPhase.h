//=============================================================================
//
// CPhaseクラス [CPhase.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CPHASE_H_
#define _CPHASE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"
#include "../FADE/CFade.h"
#include "../INPUT/CInputKeyboard.h"
#include "../INPUT/CInputGamePad.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;

//*****************************************************************************
// 列挙体
//*****************************************************************************
// フェーズの種類
typedef enum
{
	MODE_PHASE_NULL,
	MODE_PHASE_TITLE,
	MODE_PHASE_TUTORIAL,
	MODE_PHASE_CHARA_SELECT,
	MODE_PHASE_STAGE_SELECT,
	MODE_PHASE_GAME,
	MODE_PHASE_RESULT,
	MODE_PHASE_OPTION,
	MODE_PHASE_MAX
}MODE_PHASE;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPhase
{
public:
	// コンストラクタ
	CPhase(void);

	// デストラクタ
	~CPhase(void);

	// 終了
	virtual void Uninit(void);

	// 更新
	virtual void Update(void);

	// 描画
	virtual void Draw(void){};

	//======================================================
	// クリエイト
	// 引数: フェーズの種類、デバイス、マネージャーのポインタ
	// 戻り値: 作ったフェイズのポインタ
	//======================================================
	CPhase* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice, CManager* pManager);

private:

protected:
	//======================================================
	// 初期化
	// 引数: デバイス、フェーズの種類
	//======================================================
	virtual void Init(LPDIRECT3DDEVICE9* pD3DDevice, MODE_PHASE mode);

	CFade*					m_pFade;		// フェードポインタ
	LPDIRECT3DDEVICE9*		m_pD3DDevice;	// デバイス
	static CManager*		m_pManager;		// マネージャー
};
#endif
//----EOF----