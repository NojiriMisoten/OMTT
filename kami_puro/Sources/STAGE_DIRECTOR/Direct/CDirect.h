//=============================================================================
//
// CDirectクラス [CDirect.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CDIRECT_H_
#define _CDIRECT_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../MANAGER/CManager.h"
#include "../../SCENE/GAME/PLAYER/CPlayerManager.h"
#include "../../SCENE/GAME/UI/CUiManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
const D3DXVECTOR3 INVERSE_XZ[PLAYER_MAX] = {
	D3DXVECTOR3( +1.0f, +1.0f, +1.0f ),
	D3DXVECTOR3( -1.0f, +1.0f, -1.0f )
};

const float DAMAGE_AMP = 2;			// DEFAULT : 2
const float TENSION_AMP = 8;		// DEFAULT : 8


//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CCameraManager;
class CPlayerManager;
class CUIManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDirect
{
public:
	// コンストラクタ
	CDirect( CManager *pManager, CGame *pGame );

	// デストラクタ
	~CDirect( void );

	// 初期化
	virtual void Init( PLAYER_ID playerID ) = 0;

	// 終了
	virtual void Uninit( void ) = 0;

	// 更新
	virtual void Update( void ) = 0;

	// 演出が終了したかチェック
	void CheckEnd( void );

	void SetPlayerID( PLAYER_ID playerID );

	D3DXVECTOR3 TranslateCoord( PLAYER_ID playerID, D3DXVECTOR3 vec );

protected:
	CManager		*m_pManager;
	CPlayerManager	*m_pPlayerManager;
	CCameraManager	*m_pCameraManager;
	CUiManager		*m_pUIManager;
	CGame			*m_pGame;

	PLAYER_ID m_Player;				// メインとなるプレイヤー
	PLAYER_ID m_Enemy;				// 相手となるプレイヤー
	int m_FrameCount;				// 演出の現在フレーム
	int m_TotalFrame;				// 演出の総フレーム数
};

#endif
//----EOF----