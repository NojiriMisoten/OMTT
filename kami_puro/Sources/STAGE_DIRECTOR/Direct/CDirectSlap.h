//=============================================================================
//
// CDirectSlapクラス [CDirectSlap.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CDIRECTSLAP_H_
#define _CDIRECTSLAP_H_

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
class CDirectSlap : public CDirect
{
public:
	// コンストラクタ
	CDirectSlap( CManager *pManager, CGame *pGame );

	// デストラクタ
	~CDirectSlap( void );

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