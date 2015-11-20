//=============================================================================
//
// CDirectorManagerクラス [CDirectorManager.h]
// Author : 池島　大樹
//
//=============================================================================
#ifndef _CDIRECTORMANAGER_H_
#define _CDIRECTORMANAGER_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "CDirector.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CManager;
class CDirector;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDirectorManager
{
public:
	// コンストラクタ
	CDirectorManager( CManager* pManager );

	// デストラクタ
	~CDirectorManager( void );

	// 初期化
	void Init( void );

	// 終了
	void Uninit( void );

	// 更新
	void Update( void );

	// ディレクターに再生する演出を強制的に送る		使いたくない
	void SendDirector( DIRECT_ID directingID, PLAYER_ID playerID ) { m_pDirector->SendDirector( directingID, playerID ); };

	// ディレクターに再生する演出を送る
	void Direct( DIRECT_ID cueDirectingID, PLAYER_ID cuePlayerID ) { m_pDirector->Direct( cueDirectingID, cuePlayerID); };

	// 現在再生中かどうかをゲット、-1なら再生していない
	DIRECT_ID GetIsDirecting( void ) { return m_pDirector->GetIsDirecting(); };

	// 現在再生中かどうかをセット、-1なら再生していない
	void SetIsDirecting( DIRECT_ID directingID ) { m_pDirector->SetIsDirecting( directingID ); };

	// 再生終了
	void SetEndDirecting( void ) { m_pDirector->SetEndDirecting(); };

private:
	CManager*	m_pManager;
	CDirector*	m_pDirector;
};

#endif
//----EOF----