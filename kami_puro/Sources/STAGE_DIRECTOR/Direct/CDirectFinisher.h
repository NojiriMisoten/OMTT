//=============================================================================
//
// CDirectFinisherクラス [CDirectFinisher.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CDIRECTFINISHER_H_
#define _CDIRECTFINISHER_H_

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
// 前方宣言
//*****************************************************************************
class CEffect;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDirectFinisher : public CDirect
{
public:
	// コンストラクタ
	CDirectFinisher( CManager *pManager, CGame *pGame );

	// デストラクタ
	~CDirectFinisher( void );

	// 初期化
	void Init( PLAYER_ID playerID );

	// 終了
	void Uninit( void );

	// 更新
	void Update( void );

private:
	CEffect *m_pEffect;
};

#endif
//----EOF----