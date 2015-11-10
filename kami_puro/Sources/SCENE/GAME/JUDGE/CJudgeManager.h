//=============================================================================
//
// CJudgeManagerクラス [CJudgeManager.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CJUDGEMANAGER_H_
#define _CJUDGEMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "CJudge.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
class CJudge;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CJudgeManager
{
public:
	// コンストラクタ
	CJudgeManager( void );

	// デストラクタ
	~CJudgeManager( void );

	// 初期化
	void Init( CManager* pManager );

	// 終了
	void Uninit( void );

	// 更新
	void Update( void );

	// 戦闘モードをセット
	void SetBattleMode( BATTLE_MODE battleMode ) { m_pJudge->SetBattleMode( battleMode ); };

	// 戦闘モードをゲット
	BATTLE_MODE GetBattleMode( void ) { return m_pJudge->GetBattleMode(); };

private:
	CJudge* m_pJudge;
};

#endif
//----EOF----