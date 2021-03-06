//=============================================================================
//
// CDirectBattleResultクラス [CDirectBattleResult.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CDIRECTBATTLERESULT_H_
#define _CDIRECTBATTLERESULT_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "CDirect.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDirectBattleResult : public CDirect
{
public:
	// コンストラクタ
	CDirectBattleResult(CManager *pManager, CGame *pGame);

	// デストラクタ
	~CDirectBattleResult(void);

	// 初期化
	void Init(PLAYER_ID playerID);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

private:

};

#endif
//----EOF----