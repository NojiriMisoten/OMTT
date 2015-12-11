//=============================================================================
//
// CDirectChopクラス [CDirectChop.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CDIRECTCHOP_H_
#define _CDIRECTCHOP_H_

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
class CDirectChop : public CDirect
{
public:
	// コンストラクタ
	CDirectChop( CManager *pManager, CGame *pGame );

	// デストラクタ
	~CDirectChop( void );

	// 初期化
	void Init( PLAYER_ID playerID );

	// 終了
	void Uninit( void );

	// 更新
	void Update( void );

private:

};

#endif
//----EOF----